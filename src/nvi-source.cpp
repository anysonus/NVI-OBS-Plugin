

#include <obs-module.h>
#include <NVI/API.h>
#include <thread>
#include "obs-nvi.h"
#include "concurrentqueue.h"
#include <qstring.h>
#include <Windows.h>

#define PROP_SOURCE "NVI Sources"

using namespace moodycamel;


struct nvi_source {
	bool is_running;
	bool should_quit;
	obs_source_t *source;
	NVI_RECVER recver = 0;
	std::thread* pthread = 0;
	QString cur_nvi_sites_alias;
	ConcurrentQueue<std::function<void()>>* task_queue;
	uint8_t audio_buffer[16][48000 * 4];
};
	
const char *nvi_source_getname(void *data)
{
	UNUSED_PARAMETER(data);
	return "NVI Source";
}

void nvi_reconnect(void *data, QString sites_alias)
{
	auto s = (nvi_source *)data;
	s->is_running = false;
	int idx = 0;
	bool found = false;
	for (; idx < g_nvi_streams.size(); idx++) {
		QString str = QString(g_nvi_streams[idx].sites) + ":" + QString(g_nvi_streams[idx].alias);
		if (str == sites_alias) {
			found = true;
			break;
		}
	}
	if (!found)
		return;
	if (s->recver) {
		NVIRecvFree(s->recver);
	}
	NVIRecvAllocParam param{};
	param.local = nullptr;
	param.remote = g_nvi_streams[idx].uri;
	s->recver = NVIRecvAlloc(g_nvi_ctx, &param);
	s->cur_nvi_sites_alias = sites_alias;
	s->is_running = true;
}

static speaker_layout channel_count_to_layout(int channels)
{
	switch (channels) {
	case 1:
		return SPEAKERS_MONO;
	case 2:
		return SPEAKERS_STEREO;
	case 3:
		return SPEAKERS_2POINT1;
	case 4:
#if LIBOBS_API_VER >= MAKE_SEMANTIC_VERSION(21, 0, 0)
		return SPEAKERS_4POINT0;
#else
		return SPEAKERS_QUAD;
#endif
	case 5:
		return SPEAKERS_4POINT1;
	case 6:
		return SPEAKERS_5POINT1;
	case 8:
		return SPEAKERS_7POINT1;
	default:
		return SPEAKERS_UNKNOWN;
	}
}

void nvi_source_poll(void *data)
{
	auto s = (nvi_source *)data;

	obs_source_audio obs_audio_frame = {0};
	obs_source_frame obs_video_frame = {0};

	while (!s->should_quit) {
		std::function<void()> func;
		auto ret = s->task_queue->try_dequeue(func);
		if (ret) {
			func();
			func = nullptr;
		}
		if (!s->is_running || !s->recver) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		NVIRecvFrameOut param{};
		param.timeout_ms = 16;
		int nError = NVIRecvFrame(s->recver, &param);
		if (nError < 0) {
			nvi_reconnect(data, s->cur_nvi_sites_alias);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			continue;
		} else {
			if (param.image_out) {
				if (param.image_out->buffer.format == _NVIPixelFormat::NVIPixel_422P) {
					obs_video_frame.format = VIDEO_FORMAT_I422;
					obs_video_frame.width = param.image_out->info.width;
					obs_video_frame.height = param.image_out->info.height;
					obs_video_frame.data[0] = (uint8_t *)param.image_out->buffer.planes[0];
					obs_video_frame.linesize[0] = param.image_out->buffer.strides[0];
					obs_video_frame.data[1] = (uint8_t *)param.image_out->buffer.planes[1];
					obs_video_frame.linesize[1] = param.image_out->buffer.strides[1];
					obs_video_frame.data[2] = (uint8_t *)param.image_out->buffer.planes[2];
					obs_video_frame.linesize[2] = param.image_out->buffer.strides[2];
				} else if (param.image_out->buffer.format == _NVIPixelFormat::NVIPixel_NV12) {
					obs_video_frame.format = VIDEO_FORMAT_NV12;
					obs_video_frame.width = param.image_out->info.width;
					obs_video_frame.height = param.image_out->info.height;
					obs_video_frame.data[0] = (uint8_t *)param.image_out->buffer.planes[0];
					obs_video_frame.linesize[0] = param.image_out->buffer.strides[0];
					obs_video_frame.data[1] = (uint8_t *)param.image_out->buffer.planes[1];
					obs_video_frame.linesize[1] = param.image_out->buffer.strides[1];
				} else {
					blog(LOG_INFO, "unknown format");
					continue;
				}

				// todo color sapce 601/709/2020
				// nvi has no 601
				// obs has no 2020
				video_format_get_parameters(VIDEO_CS_709, VIDEO_RANGE_FULL,
							    obs_video_frame.color_matrix,
							    obs_video_frame.color_range_min,
							    obs_video_frame.color_range_max);
				obs_source_output_video(s->source, &obs_video_frame);
			
				
			}
			if (param.wave_out) {
				obs_audio_frame.speakers = channel_count_to_layout(param.wave_out->info.channels);
				obs_audio_frame.samples_per_sec = param.wave_out->info.sample_rate;
				if (param.wave_out->info.depth == NVIWaveBit_F32)
					obs_audio_frame.format = AUDIO_FORMAT_FLOAT;
				else if (param.wave_out->info.depth == NVIWaveBit_16)
					obs_audio_frame.format = AUDIO_FORMAT_16BIT;
				else if (param.wave_out->info.depth == AUDIO_FORMAT_U8BIT)
					obs_audio_frame.format = AUDIO_FORMAT_U8BIT;
				else
					obs_audio_frame.format = AUDIO_FORMAT_UNKNOWN;
				obs_audio_frame.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
								    std::chrono::steady_clock::now().time_since_epoch())
								    .count(); //param.wave_out->info.time;
				obs_audio_frame.frames = param.wave_out->buffer.samples;
				auto sample_bytes = param.wave_out->buffer.align;
				auto chan_bytes = param.wave_out->buffer.samples * sample_bytes;
				/*for (int i = 0; i < param.wave_out->info.channels; i++) {
					obs_audio_frame.data[i] = param.wave_out->buffer.data + i * chan_bytes;
				}*/
				/*for (int i = 0; i < param.wave_out->info.channels; i++) {
					obs_audio_frame.data[i] = param.wave_out->buffer.data + i * chan_bytes;
				}*/
				obs_audio_frame.data[0] = param.wave_out->buffer.data;
				
				obs_source_output_audio(s->source, &obs_audio_frame);
			}
		}
	}
}

obs_properties_t *nvi_source_getproperties(void *data)
{
	auto s = (struct nvi_source *)data;
	obs_properties_t *props = obs_properties_create();
	obs_properties_set_flags(props, OBS_PROPERTIES_DEFER_UPDATE);

	obs_property_t *source_list = obs_properties_add_list(props, PROP_SOURCE, PROP_SOURCE,OBS_COMBO_TYPE_EDITABLE,OBS_COMBO_FORMAT_STRING);
	nvi_discovery();
	for (int i = 0; i < g_nvi_streams.size(); i++) {
		if (g_nvi_streams[i].alias) {
			QString str = QString(g_nvi_streams[i].sites) + ":" + QString(g_nvi_streams[i].alias);
			obs_property_list_add_string(source_list, str.toStdString().c_str(), str.toStdString().c_str());
		}
	}
	
	return props;
}
void nvi_source_getdefaults(obs_data_t *settings)
{
	
}


void nvi_source_update(void *data, obs_data_t *settings)
{
	auto s = (struct nvi_source *)data;
	if (!s->pthread) {
		s->pthread = new std::thread(nvi_source_poll, data);
		auto threadHandle = s->pthread->native_handle();

		#ifdef WIN32
		SetThreadPriority(threadHandle, THREAD_PRIORITY_HIGHEST);
		#endif

	}

	QString sites_alias = obs_data_get_string(settings, PROP_SOURCE);
	if (sites_alias.isEmpty())
		return;
	s->task_queue->enqueue([=]() {
		nvi_reconnect(data, sites_alias);
	});



	
}


void nvi_source_shown(void *data)
{
	auto s = (struct nvi_source *)data;
	
}

void nvi_source_hidden(void *data)
{
	auto s = (struct nvi_source *)data;
	
}

void nvi_source_activated(void *data)
{
	auto s = (struct nvi_source *)data;
	
}

void nvi_source_deactivated(void *data)
{
	auto s = (struct nvi_source *)data;


}

void *nvi_source_create(obs_data_t *settings, obs_source_t *source)
{
	auto s = (struct nvi_source *)bzalloc(sizeof(struct nvi_source));
	s->task_queue = new ConcurrentQueue<std::function<void()>>();
	s->source = source;
	nvi_source_update(s, settings);
	return s;
}

void nvi_source_destroy(void *data)
{
	auto s = (struct nvi_source *)data;
	s->is_running = false;
	s->task_queue->enqueue([=] {
		s->should_quit = true;
	});
	s->pthread->join();
	if (s->task_queue)
		delete s->task_queue;
	bfree(s);
}

struct obs_source_info create_nvi_source_info()
{
	struct obs_source_info nvi_source_info = {};
	nvi_source_info.id = "NVI Source";
	nvi_source_info.type = OBS_SOURCE_TYPE_INPUT;
	nvi_source_info.output_flags = OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_AUDIO | OBS_SOURCE_DO_NOT_DUPLICATE;
	nvi_source_info.get_name = nvi_source_getname;
	nvi_source_info.get_properties = nvi_source_getproperties;
	nvi_source_info.get_defaults = nvi_source_getdefaults;
	nvi_source_info.update = nvi_source_update;
	nvi_source_info.show = nvi_source_shown;
	nvi_source_info.hide = nvi_source_hidden;
	nvi_source_info.activate = nvi_source_activated;
	nvi_source_info.deactivate = nvi_source_deactivated;
	nvi_source_info.create = nvi_source_create;
	nvi_source_info.destroy = nvi_source_destroy;

	return nvi_source_info;
}

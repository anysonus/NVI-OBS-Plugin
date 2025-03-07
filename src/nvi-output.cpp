#include <obs-module.h>
#include "obs-nvi.h"
#include <chrono>
#include <qmessagebox.h>

struct nvi_output {
	const char *nvi_name;

	bool started;
	NVI_SENDER sender;

	uint32_t frame_width;
	uint32_t frame_height;
	video_format frame_format;
	double video_framerate;

	size_t audio_channels;
	uint32_t audio_samplerate;
	audio_format audiofmt;
	uint64_t tick_value; 
	uint8_t audio_buffer[4 * 1920000 * 12];
};


template<class T> void PlanarToPackedFloat(T **planars, float *packed, int sampleCountPerChan, int noChan)
{
	T max_value = std::numeric_limits<T>::max();
	for (int i = 0; i < sampleCountPerChan; i++) {
		for (int ch = 0; ch < noChan; ch++) {
			if (std::is_floating_point<T>::value) {
				packed[i * noChan + ch] = float(planars[ch][i]);
			} else {
				packed[i * noChan + ch] = float(planars[ch][i]) / float(max_value);
			}
		}
	}
}
const char *nvi_output_getname(void *data)
{
	UNUSED_PARAMETER(data);
	return "NVI Output";
}

obs_properties_t *nvi_output_getproperties(void *data)
{
	UNUSED_PARAMETER(data);

	obs_properties_t *props = obs_properties_create();
	obs_properties_set_flags(props, OBS_PROPERTIES_DEFER_UPDATE);

	obs_properties_add_text(props, "nvi_name", "NVI Output", OBS_TEXT_DEFAULT);

	return props;
}


void nvi_output_getdefaults(obs_data_t *settings)
{
}

bool nvi_output_start(void *data)
{
	auto o = (struct nvi_output *)data;

	uint32_t flags = 0;
	video_t *video = obs_output_video(main_out);
	audio_t *audio = obs_output_audio(main_out);

	if (!video && !audio) {
		QMessageBox::information(nullptr, "Error", "NVI Output start failed,no video&audio", QMessageBox::Ok);
		return false;
	}

	if (video) {
		video_format format = video_output_get_format(video);
		uint32_t width = video_output_get_width(video);
		uint32_t height = video_output_get_height(video);

		o->frame_format = format;

		o->frame_width = width;
		o->frame_height = height;
		o->video_framerate = video_output_get_frame_rate(video);
		flags |= OBS_OUTPUT_VIDEO;
	}

	if (audio) {
		o->audio_samplerate = audio_output_get_sample_rate(audio);
		o->audio_channels = audio_output_get_channels(audio);
		auto info = audio_output_get_info(audio);
		if (info)
			o->audiofmt = info->format;
		flags |= OBS_OUTPUT_AUDIO;
	}

	NVISendAllocParam param{};
	param.alias = "OBS";
	o->sender = NVISendAlloc(g_nvi_ctx, &param);
	

	if (o->sender) {
		o->started = obs_output_begin_data_capture(main_out, flags);
		if (o->started) {
			blog(LOG_INFO, "'%s': nvi output started", o->nvi_name);
		} else {

			QMessageBox::information(nullptr, "Error", "NVI Output capture start failed",
						 QMessageBox::Ok);
		}
	} else {
		QMessageBox::information(nullptr, "Error", "NVI sender create failed", QMessageBox::Ok);
	}

	return o->started;
}

void nvi_output_stop(void *data, uint64_t ts)
{
	auto o = (struct nvi_output *)data;

	o->started = false;
	obs_output_end_data_capture(main_out);

	

	o->frame_width = 0;
	o->frame_height = 0;
	o->video_framerate = 0.0;

	o->audio_channels = 0;
	o->audio_samplerate = 0;
}

void nvi_output_update(void *data, obs_data_t *settings)
{
}

void *nvi_output_create(obs_data_t *settings, obs_output_t *output)
{
	auto o = (struct nvi_output *)bzalloc(sizeof(nvi_output));
	nvi_output_update(o, settings);
	return o;
}

void nvi_output_destroy(void *data)
{
	auto o = (struct nvi_output *)data;
	bfree(o);
}

void nvi_output_video(void *data, struct video_data *frame)
{
	auto o = (struct nvi_output *)data;

	if (!o->started || !o->frame_width || !o->frame_height)
		return;

	uint32_t width = o->frame_width;
	uint32_t height = o->frame_height;

	NVIVideoImageFrame image{};
	image.side.bytes = nullptr;
	image.side.size = 0;
	image.updated = nullptr;
	image.info.codec = NVICodec_AVC;
	image.info.width = width;
	image.info.height = height;
	image.info.frame_rate_num = 30;
	image.info.frame_rate_den = 1;
	image.info.rotation = 0u;
	image.info.colorspace.primary = NVIPrimary_BT709;
	image.info.colorspace.transfer = NVITransfer_BT709;
	image.info.colorspace.matrix = NVIMatrix_BT709;
	image.info.colorspace.range = NVIRange_Limited;
	image.info.tick.value = 0u;
	image.info.tick.freq_num = 1u;
	image.info.tick.freq_den = 90000u;
	image.info.time = frame->timestamp;
	if (o->frame_format == VIDEO_FORMAT_I420) {
		image.buffer.format = NVIPixel_I420;
		image.buffer.strides[0] = frame->linesize[0];
		image.buffer.strides[1] = frame->linesize[1];
		image.buffer.strides[2] = frame->linesize[2];
		image.buffer.planes[0] = frame->data[0];
		image.buffer.planes[1] = frame->data[1];
		image.buffer.planes[2] = frame->data[2];
	} else if (o->frame_format == VIDEO_FORMAT_I422) {
		image.buffer.format = NVIPixel_422P; 
		image.buffer.strides[0] = frame->linesize[0];
		image.buffer.strides[1] = frame->linesize[1];
		image.buffer.strides[2] = frame->linesize[2];
		image.buffer.planes[0] = frame->data[0];
		image.buffer.planes[1] = frame->data[1];
		image.buffer.planes[2] = frame->data[2];

	} else if (o->frame_format == VIDEO_FORMAT_NV12) {
		image.buffer.format = NVIPixel_NV12; /// see NVIPixelFormat
		image.buffer.strides[0] = frame->linesize[0];
		image.buffer.strides[1] = frame->linesize[1];
		image.buffer.planes[0] = frame->data[0];
		image.buffer.planes[1] = frame->data[1];
	} else {
		blog(LOG_INFO, "unsupport video format");
	}
	image.info.time = std::chrono::duration_cast<std::chrono::microseconds>(
				  std::chrono::steady_clock::now().time_since_epoch())
				  .count();
	image.buffer.type = NVIBuffer_HOST;
	//blog(LOG_INFO, "send video");
	NVISendVideo(o->sender, &image);

	
}

void nvi_output_audio(void *data, struct audio_data *frame)
{
	auto o = (struct nvi_output *)data;

	if (!o->started || !o->audio_samplerate || !o->audio_channels)
		return;

	NVIAudioWaveFrame wave{};
	wave.info.depth = NVIWaveBit_F32;
	wave.buffer.align = 4;
	if (o->audiofmt == AUDIO_FORMAT_FLOAT_PLANAR) {
		PlanarToPackedFloat<float>((float **)frame->data, (float *)o->audio_buffer, (int)frame->frames,
					   (int)o->audio_channels);
		
	} else if (o->audiofmt == AUDIO_FORMAT_16BIT_PLANAR) {
		PlanarToPackedFloat<int16_t>((int16_t **)frame->data, (float *)o->audio_buffer, (int)frame->frames,
					     (int)o->audio_channels);
	} else if (o->audiofmt == AUDIO_FORMAT_U8BIT_PLANAR) {
		PlanarToPackedFloat<uint8_t>((uint8_t **)frame->data, (float *)o->audio_buffer, (int)frame->frames,
					     (int)o->audio_channels);
	} else {
		blog(LOG_INFO, "unsupport audio format: %u", o->audiofmt);
		return;
	}
	
	wave.info.codec = NVICodec_LPCM;
	wave.info.sample_rate = o->audio_samplerate;
	wave.info.channels =(uint16_t) o->audio_channels;
	wave.info.tick.value = o->tick_value;
	wave.info.tick.freq_num = 1u;
	wave.info.tick.freq_den = wave.info.sample_rate;
	wave.info.time = frame->timestamp;
	wave.buffer.samples = frame->frames;
	wave.buffer.data = o->audio_buffer;
	wave.buffer.size = frame->frames * wave.info.channels * 4;
	o->tick_value += wave.buffer.samples;

	
	
	//blog(LOG_INFO, "send audio");
	NVISendAudio(o->sender, &wave);
}


struct obs_output_info create_nvi_output_info()
{
	struct obs_output_info nvi_output_info = {};
	nvi_output_info.id = "nvi_output";
	nvi_output_info.flags = OBS_OUTPUT_AV;
	nvi_output_info.get_name = nvi_output_getname;
	nvi_output_info.get_properties = nvi_output_getproperties;
	nvi_output_info.get_defaults = nvi_output_getdefaults;
	nvi_output_info.create = nvi_output_create;
	nvi_output_info.destroy = nvi_output_destroy;
	nvi_output_info.update = nvi_output_update;
	nvi_output_info.start = nvi_output_start;
	nvi_output_info.stop = nvi_output_stop;
	nvi_output_info.raw_video = nvi_output_video;
	nvi_output_info.raw_audio = nvi_output_audio;

	return nvi_output_info;
}

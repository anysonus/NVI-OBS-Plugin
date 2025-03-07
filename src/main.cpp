#include <obs-module.h>
#include <obs-frontend-api.h>
#include <QLibrary>
#include <nvi/API.h>
#include <QMainWindow>
#include "obs-nvi.h"
#include <qmessagebox.h>
OBS_DECLARE_MODULE()

NVI_CONTEXT g_nvi_ctx = nullptr;
std::vector<NVINetworkStream> g_nvi_streams;
struct obs_source_info nvi_source_info;
struct obs_output_info nvi_output_info;
obs_output_t *main_out = nullptr;

MODULE_EXPORT const char *obs_module_description(void)
{
    return "nvi-plugin";
}


bool obs_module_load(void)
{
	auto loaded_lib = new QLibrary("nvi.dll", nullptr);
	if (loaded_lib->load()) {

		nvi_source_info = create_nvi_source_info();
		obs_register_source(&nvi_source_info);

		nvi_output_info = create_nvi_output_info();
		obs_register_output(&nvi_output_info);

		
		QMainWindow *main_window = (QMainWindow *)obs_frontend_get_main_window();
		QAction *menu_action = (QAction *)obs_frontend_add_tools_menu_qaction("Start NVI Output");

		auto menu_cb = [] {
			if (main_out)
				return;
			obs_data_t *settings = obs_data_create();
			obs_data_set_string(settings, "nvi_name", "obs");
			main_out = obs_output_create("nvi_output", "NVI Main Output", settings, nullptr);
			obs_data_release(settings);
			if (!obs_output_start(main_out)) {
				QMessageBox::information(nullptr, "Error", "NVI Output Failed", QMessageBox::Ok);
			}
		};
		menu_action->connect(menu_action, &QAction::triggered, menu_cb);
		blog(LOG_INFO, "nvi loaded successfully");
		g_nvi_ctx = NVIContextCreate(nullptr);
		nvi_discovery();
		return true;
	}

	blog(LOG_ERROR, "Can't find the NDI library");
	return false;
}

void obs_module_post_load(void)
{
	
}

void obs_module_unload(void)
{
	obs_output_release(main_out);
}

void nvi_discovery()
{
	g_nvi_streams.resize(10);
	NVINetworkEnumParam enumparam{};
	enumparam.streams = g_nvi_streams.data();
	enumparam.streams_size = (uint32_t)g_nvi_streams.size();
	enumparam.timeout_ms = 1500;
	NVINetworkEnumStream(g_nvi_ctx, &enumparam);
}

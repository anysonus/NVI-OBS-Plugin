#pragma once
#include <nvi/API.h>
#include <vector>

extern void nvi_discovery();
extern NVI_CONTEXT g_nvi_ctx;
extern std::vector<NVINetworkStream> g_nvi_streams;

extern struct obs_source_info create_nvi_source_info();
extern struct obs_output_info create_nvi_output_info();
extern obs_output_t *main_out;

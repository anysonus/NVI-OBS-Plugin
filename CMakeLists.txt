cmake_minimum_required(VERSION 3.28...3.30)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED Core Widgets)


# set(nvi_headers
# 	src/obs-nvi.h)

add_library(nvi-plugin MODULE)
add_library(OBS::nvi ALIAS nvi-plugin)

target_sources(
	nvi-plugin
	PRIVATE
	src/main.cpp
  src/nvi-output.cpp
  src/nvi-source.cpp
  src/obs-nvi.h
)
include_directories(
	"nvi/include"
  "lockfreequeue"
	"${LIBOBS_INCLUDE_DIR}/../UI/obs-frontend-api"
	${Qt6Core_INCLUDES}
	${Qt6Widgets_INCLUDES})

target_link_libraries(nvi-plugin
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/nvi/lib/nvi.lib
	OBS::libobs
	OBS::frontend-api
	Qt6::Core
	Qt6::Widgets)

set_target_properties_obs(nvi-plugin PROPERTIES FOLDER plugins/nvi-plugin PREFIX "")


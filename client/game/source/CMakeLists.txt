add_subdirectory(cmp)
add_subdirectory(cmp_load)
add_subdirectory(scripts)
add_subdirectory(sdl)
add_subdirectory(rapidjson_scene_parser)

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

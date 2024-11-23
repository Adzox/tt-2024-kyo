set(LIBRARY_NAME Engine)

add_library(${LIBRARY_NAME} STATIC)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)

add_subdirectory(source/Components)
add_subdirectory(source/ComponentLoaders)
add_subdirectory(source/Assets)
add_subdirectory(source/Events)
add_subdirectory(source/Input)
add_subdirectory(source/Log)
add_subdirectory(source/Math)
add_subdirectory(source/Graphics)
add_subdirectory(source/SceneParser)
add_subdirectory(source/Scenes)
add_subdirectory(source/Base)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
get_target_property(HEADER_LIST ${LIBRARY_NAME} HEADER_SET)
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PREFIX "Header Files"
    FILES ${HEADER_LIST}
)

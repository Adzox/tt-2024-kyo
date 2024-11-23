add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_definitions(-DTARGET_NAME="Game")
add_definitions(-DTARGET_WIDTH=960)
add_definitions(-DTARGET_HEIGHT=540)

add_subdirectory(source)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
target_compile_definitions(${PROJECT_NAME} PUBLIC SDL_MAIN_USE_CALLBACKS)

include(../externals/rapidjson.cmake)

if (WIN32)
    # Move DLLs for dependencies to output dir
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        VERBATIM
    )
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_image::SDL3_image>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        VERBATIM
    )

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:PNG::PNG>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        VERBATIM
    )

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:zlib>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        VERBATIM
    )
endif()

# Move resources to output dir
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/res" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
    VERBATIM
)


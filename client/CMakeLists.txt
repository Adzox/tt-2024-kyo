cmake_minimum_required(VERSION 3.24)

project(
	GameClient
	LANGUAGES CXX)
		
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
	set(CMAKE_CXX_STANDARD 23)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# Set up HTML-specific files
if(CMAKE_SYSTEM_NAME MATCHES Emscripten)
	set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
	configure_file(res/web/index.html ${CMAKE_BINARY_DIR}/web/index.html COPYONLY)
	configure_file(res/web/style.css ${CMAKE_BINARY_DIR}/web/style.css COPYONLY)
	configure_file(res/web/favicon.ico ${CMAKE_BINARY_DIR}/web/favicon.ico COPYONLY)
endif()

file(COPY res/common/ DESTINATION ${CMAKE_BINARY_DIR}/res/common)

# Add internal library
add_subdirectory(engine)

# Add external dependencies
add_subdirectory(externals/SDL EXCLUDE_FROM_ALL)

include(externals/SDL_image.cmake)
add_subdirectory(externals/SDL_image EXCLUDE_FROM_ALL)

# Add main app
add_subdirectory(game)

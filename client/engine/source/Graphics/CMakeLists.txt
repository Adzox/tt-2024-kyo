target_sources(${LIBRARY_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS "../../include/Graphics" FILES 
    "../../include/Graphics/Color.hpp"
    "../../include/Graphics/IDrawApi.hpp"
    "../../include/Graphics/ImageAsset.hpp"
    "../../include/Graphics/NineSliceData.hpp")
target_sources(${LIBRARY_NAME} PRIVATE
    "Color.cpp"
    "ImageAsset.cpp"
    "NineSliceData.cpp")

target_sources(${LIBRARY_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS "../../include/Math" FILES
    "../../include/Math/IntRect.hpp"
    "../../include/Math/Point2d.hpp"
    "../../include/Math/Utils.hpp"
    "../../include/Math/Vector.hpp")
target_sources(${LIBRARY_NAME} PRIVATE
    "IntRect.cpp"
    "Point2d.cpp"
    "Utils.cpp")

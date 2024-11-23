#include "Graphics/NineSliceData.hpp"

namespace Graphics {

NineSliceData::NineSliceData(int width, int height, Math::IntPoint2d topLeft, Math::IntPoint2d bottomRight)
    : topLeft(0, 0, topLeft.GetX(), topLeft.GetY()),
      topCenter(topLeft.GetX(), 0, width - topLeft.GetX() - bottomRight.GetX(), topLeft.GetY()),
      topRight(bottomRight.GetX(), 0, topLeft.GetX(), topLeft.GetY()),
      middleLeft(0, topLeft.GetY(), topLeft.GetX(), topLeft.GetY()),
      middleCenter(topLeft.GetX(), topLeft.GetY(), topLeft.GetX(), topLeft.GetY()),
      middleRight(topLeft.GetX() * 2, 0, topLeft.GetX(), topLeft.GetY()),
      bottomLeft(0, bottomRight.GetY(), topLeft.GetX(), height - bottomRight.GetY()),
      bottomCenter(topLeft.GetX(),
                   bottomRight.GetY(),
                   bottomRight.GetX() - topLeft.GetX(),
                   height - bottomRight.GetY()),
      bottomRight(bottomRight.GetX(), bottomRight.GetY(), width - bottomRight.GetX(), height - bottomRight.GetY()),
      minWidth(width),
      minHeight(height) {}

}  // namespace Graphics

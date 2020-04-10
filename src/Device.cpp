#include <ShitGraph/Device.hpp>

namespace ShitGraph {
	int Device::GetWidth() const noexcept {
		return static_cast<int>(GetRectangle().RightBottom.X);
	}
	int Device::GetHeight() const noexcept {
		return static_cast<int>(GetRectangle().RightBottom.Y);
	}
}
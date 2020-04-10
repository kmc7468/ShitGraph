#pragma once

#include <ShitGraph/CoreType.hpp>

namespace ShitGraph {
	class Device {
	public:
		Device() noexcept = default;
		Device(const Device&) = delete;
		virtual ~Device() = default;

	public:
		Device& operator=(const Device&) = delete;

	public:
		virtual Rectangle GetRectangle() const noexcept = 0;
		virtual void UpdateRectangle() noexcept = 0;
		int GetWidth() const noexcept;
		int GetHeight() const noexcept;
	};
}
#pragma once

#include <ShitGraph/CoreType.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>

namespace ShitGraph {
	struct Color final {
		std::uint8_t R = 0;
		std::uint8_t G = 0;
		std::uint8_t B = 0;
	};
}

namespace ShitGraph {
	class GraphicObject {
	public:
		GraphicObject() noexcept = default;
		GraphicObject(const GraphicObject&) = delete;
		virtual ~GraphicObject() = default;

	public:
		GraphicObject& operator=(const GraphicObject&) = delete;

	public:
		virtual void* GetHandle() const noexcept = 0;
	};

	class SolidBrush : public GraphicObject {
	private:
		Color m_Color;

	public:
		SolidBrush() noexcept = default;
		SolidBrush(const Color& color) noexcept;
		SolidBrush(const SolidBrush&) = delete;
		virtual ~SolidBrush() override = default;

	public:
		SolidBrush& operator=(const SolidBrush&) = delete;

	public:
		Color GetColor() const noexcept;
	};

	class Pen : public GraphicObject {
	private:
		Color m_Color;
		Scalar m_Width = 1;

	public:
		Pen() noexcept = default;
		Pen(const Color& color) noexcept;
		Pen(const Color& color, Scalar width) noexcept;
		Pen(const Pen&) = delete;
		virtual ~Pen() override = default;

	public:
		Pen& operator=(const Pen&) = delete;

	public:
		Color GetColor() const noexcept;
		Scalar GetWidth() const noexcept;
	};
}

namespace ShitGraph {
	class GraphicDevice {
	private:
		std::vector<GraphicObject*> m_Objects;

	public:
		GraphicDevice() noexcept = default;
		GraphicDevice(const GraphicDevice&) = delete;
		virtual ~GraphicDevice();

	public:
		GraphicDevice& operator=(const GraphicDevice&) = delete;

	public:
		virtual Rectangle GetRectangle() const noexcept = 0;
		virtual void UpdateRectangle() noexcept = 0;
		int GetWidth() const noexcept;
		int GetHeight() const noexcept;

		ShitGraph::SolidBrush* SolidBrush(const Color& color);
		ShitGraph::Pen* Pen(const Color& color);
		ShitGraph::Pen* Pen(const Color& color, Scalar width);
		void Delete(GraphicObject* object);

		virtual void DrawLines(const ShitGraph::Pen* pen, Point* points, std::size_t size) = 0;

	protected:
		virtual ShitGraph::SolidBrush* CreateSolidBrush(const Color& color) = 0;
		virtual ShitGraph::Pen* CreatePen(const Color& color, Scalar width) = 0;
	};
}
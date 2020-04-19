#pragma once

#include <ShitGraph/CoreType.hpp>

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

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

	class Brush : public GraphicObject {
	public:
		Brush() noexcept = default;
		Brush(const Brush&) = delete;
		virtual ~Brush() override = default;

	public:
		Brush& operator=(const Brush&) = delete;
	};

	class SolidBrush : public Brush {
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

	class Font : public GraphicObject {
	private:
		std::string m_Name;
		Scalar m_Size = 9;

	public:
		Font() noexcept = default;
		Font(std::string name, Scalar size) noexcept;
		Font(const Font&) = delete;
		virtual ~Font() override = default;

	public:
		Font& operator=(const Font&) = delete;

	public:
		std::string_view GetName() const noexcept;
		Scalar GetSize() const noexcept;
	};
}

namespace ShitGraph {
	class GraphicDevice {
	private:
		std::vector<GraphicObject*> m_Objects;
		ShitGraph::Rectangle m_Rectangle;

	public:
		explicit GraphicDevice(const ShitGraph::Rectangle& rectangle) noexcept;
		GraphicDevice(const GraphicDevice&) = delete;
		virtual ~GraphicDevice();

	public:
		GraphicDevice& operator=(const GraphicDevice&) = delete;

	public:
		Rectangle GetRectangle() const noexcept;
		int GetWidth() const noexcept;
		int GetHeight() const noexcept;

		ShitGraph::SolidBrush* SolidBrush(const Color& color);
		ShitGraph::Pen* Pen(const Color& color);
		ShitGraph::Pen* Pen(const Color& color, Scalar width);
		ShitGraph::Font* Font(std::string name, Scalar size);
		void Delete(GraphicObject* object);

		virtual void DrawLines(const ShitGraph::Pen* pen, const Point* points, std::size_t size) = 0;
		virtual void DrawPoint(const ShitGraph::Brush* brush, const Point& point, Scalar radius) = 0;
		virtual void DrawString(const ShitGraph::Font* font, const ShitGraph::Brush* brush, const Point& location, const std::string& string) = 0;

	protected:
		virtual ShitGraph::SolidBrush* CreateSolidBrush(const Color& color) = 0;
		virtual ShitGraph::Pen* CreatePen(const Color& color, Scalar width) = 0;
		virtual ShitGraph::Font* CreateFont(std::string name, Scalar size) = 0;
	};

	template<typename T>
	class ManagedGraphicObject final {
	private:
		GraphicDevice* m_Device = nullptr;
		GraphicObject* m_Object = nullptr;

	public:
		ManagedGraphicObject(GraphicDevice& device, GraphicObject* object) noexcept
			: m_Device(&device), m_Object(object) {}
		ManagedGraphicObject(ManagedGraphicObject&& object) noexcept
			: m_Device(object.m_Device), m_Object(object.m_Object) {
			object.m_Device = nullptr;
			object.m_Object = nullptr;
		}
		~ManagedGraphicObject() {
			Reset();
		}

	public:
		ManagedGraphicObject& operator=(ManagedGraphicObject&& object) noexcept {
			m_Device = object.m_Device;
			m_Object = object.m_Object;

			object.m_Device = nullptr;
			object.m_Object = nullptr;
			return *this;
			return *this;
		}
		operator T*() const noexcept {
			return static_cast<T*>(Get());
		}

	public:
		void Reset() noexcept {
			if (m_Device) {
				m_Device->Delete(m_Object);
			}
		}
		GraphicObject* Release() const noexcept {
			m_Device = nullptr;
			return m_Object;
		}
		GraphicObject* Get() const noexcept {
			return m_Object;
		}
	};
}
#pragma once

namespace ShitGraph {
	class Window;
	class GraphicDevice;

	struct EventArgs {
		ShitGraph::Window& Window;
	};

	struct PaintEventArgs final {
		GraphicDevice& Device;
	};

	enum class MouseButton {
		None,

		LButton,
	};

	struct MouseEventArgs final {
		int X, Y;
		MouseButton Button;
	};

	struct MouseWheelEventArgs final : EventArgs {
		int Delta;
	};

	struct KeyEventArgs final {
		int Key;
	};
}

namespace ShitGraph {
	class EventAdaptor {
	public:
		EventAdaptor() noexcept = default;
		EventAdaptor(const EventAdaptor&) = delete;
		virtual ~EventAdaptor() = default;

	public:
		EventAdaptor& operator=(const EventAdaptor&) = delete;

	public:
		virtual void Paint(PaintEventArgs e) = 0;
		virtual void Destroy() = 0;

		virtual void MouseDown(MouseEventArgs e) = 0;
		virtual void MouseUp(MouseEventArgs e) = 0;
		virtual void MouseMove(MouseEventArgs e) = 0;
		virtual void MouseWheel(MouseWheelEventArgs e) = 0;

		virtual void KeyDown(KeyEventArgs e) = 0;
	};
}

namespace ShitGraph {
	class Window {
	private:
		EventAdaptor* m_EventAdaptor = nullptr;

	public:
		explicit Window(EventAdaptor* eventAdaptor) noexcept;
		Window(const Window&) = delete;
		virtual ~Window();

	public:
		Window& operator=(const Window&) = delete;
	};
}
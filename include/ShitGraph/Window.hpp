#pragma once

namespace ShitGraph {
	class Window;
	class GraphicDevice;

	struct EventArgs {
		ShitGraph::Window& Window;
	};

	struct PaintEventArgs final : EventArgs {
		GraphicDevice& Device;
	};

	enum class MouseButton {
		None,

		Left,
	};

	struct MouseEventArgs final : EventArgs {
		int X, Y;
		MouseButton Button;
	};

	struct MouseWheelEventArgs final : EventArgs {
		int Delta;
	};

	struct KeyEventArgs final : EventArgs {
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

	protected:
		void Paint(GraphicDevice& device);
		void Destroy();

		void MouseDown(int x, int y, MouseButton button);
		void MouseUp(int x, int y, MouseButton button);
		void MouseMove(int x, int y);
		void MouseWheel(int delta);

		void KeyDown(int key);
	};
}
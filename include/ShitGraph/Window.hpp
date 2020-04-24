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
		virtual void Paint(PaintEventArgs e);
		virtual void Destroy();

		virtual void MouseDown(MouseEventArgs e);
		virtual void MouseUp(MouseEventArgs e);
		virtual void MouseMove(MouseEventArgs e);
		virtual void MouseWheel(MouseWheelEventArgs e);

		virtual void KeyDown(KeyEventArgs e);
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

	public:
		virtual void Show() = 0;
		virtual void Hide() = 0;

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

namespace ShitGraph {
	class Application {
	public:
		Application() noexcept = default;
		Application(const Application&) = delete;
		virtual ~Application() = default;

	public:
		Application& operator=(const Application&) = delete;

	public:
		virtual int Run() = 0;
	};
}
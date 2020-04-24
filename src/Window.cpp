#include <ShitGraph/Window.hpp>

#include <cassert>

namespace ShitGraph {
	void EventAdaptor::Paint(PaintEventArgs) {}
	void EventAdaptor::Destroy() {}

	void EventAdaptor::MouseDown(MouseEventArgs) {}
	void EventAdaptor::MouseUp(MouseEventArgs) {}
	void EventAdaptor::MouseMove(MouseEventArgs) {}
	void EventAdaptor::MouseWheel(MouseWheelEventArgs) {}

	void EventAdaptor::KeyDown(KeyEventArgs) {}
}

namespace ShitGraph {
	Window::Window(EventAdaptor* eventAdaptor) noexcept
		: m_EventAdaptor(eventAdaptor) {
		assert(eventAdaptor);
	}
	Window::~Window() {
		delete m_EventAdaptor;
	}

	void Window::Paint(GraphicDevice& device) {
		PaintEventArgs e{ *this, device };
		m_EventAdaptor->Paint(e);
	}
	void Window::Destroy() {
		m_EventAdaptor->Destroy();
	}

	void Window::MouseDown(int x, int y, MouseButton button) {
		MouseEventArgs e{ *this, x, y, button };
		m_EventAdaptor->MouseDown(e);
	}
	void Window::MouseUp(int x, int y, MouseButton button) {
		MouseEventArgs e{ *this, x, y, button };
		m_EventAdaptor->MouseUp(e);
	}
	void Window::MouseMove(int x, int y) {
		MouseEventArgs e{ *this, x, y, MouseButton::None };
		m_EventAdaptor->MouseMove(e);
	}
	void Window::MouseWheel(int delta) {
		MouseWheelEventArgs e{ *this, delta };
		m_EventAdaptor->MouseWheel(e);
	}

	void Window::KeyDown(int key) {
		KeyEventArgs e{ *this, key };
		m_EventAdaptor->KeyDown(e);
	}
}
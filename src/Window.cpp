#include <ShitGraph/Window.hpp>

#include <cassert>

namespace ShitGraph {
	void EventAdaptor::Paint(PaintEventArgs) {}
	void EventAdaptor::Destroy(EventArgs) {}

	void EventAdaptor::MouseDown(MouseEventArgs) {}
	void EventAdaptor::MouseUp(MouseEventArgs) {}
	void EventAdaptor::MouseMove(MouseEventArgs) {}
	void EventAdaptor::MouseWheel(MouseWheelEventArgs) {}

	void EventAdaptor::KeyDown(KeyEventArgs) {}
}

namespace ShitGraph {
	Control::Control(EventAdaptor* eventAdaptor) noexcept
		: m_EventAdaptor(eventAdaptor) {
		assert(eventAdaptor);
	}
	Control::~Control() {
		delete m_EventAdaptor;
	}

	void Control::Paint(GraphicDevice& device) {
		PaintEventArgs e{ *this, device };
		m_EventAdaptor->Paint(e);
	}
	void Control::Destroy() {
		EventArgs e{ *this };
		m_EventAdaptor->Destroy(e);
	}

	void Control::MouseDown(int x, int y, MouseButton button) {
		MouseEventArgs e{ *this, x, y, button };
		m_EventAdaptor->MouseDown(e);
	}
	void Control::MouseUp(int x, int y, MouseButton button) {
		MouseEventArgs e{ *this, x, y, button };
		m_EventAdaptor->MouseUp(e);
	}
	void Control::MouseMove(int x, int y) {
		MouseEventArgs e{ *this, x, y, MouseButton::None };
		m_EventAdaptor->MouseMove(e);
	}
	void Control::MouseWheel(int delta, bool controlKey) {
		MouseWheelEventArgs e{ *this, delta, controlKey };
		m_EventAdaptor->MouseWheel(e);
	}

	void Control::KeyDown(int key) {
		KeyEventArgs e{ *this, key };
		m_EventAdaptor->KeyDown(e);
	}
}

namespace ShitGraph {
	Window::Window(EventAdaptor* eventAdaptor) noexcept
		: Control(eventAdaptor) {}
}
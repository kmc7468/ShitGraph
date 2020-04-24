#include <ShitGraph/Window.hpp>

#include <cassert>

namespace ShitGraph {
	Window::Window(EventAdaptor* eventAdaptor) noexcept
		: m_EventAdaptor(eventAdaptor) {
		assert(eventAdaptor);
	}
	Window::~Window() {
		delete m_EventAdaptor;
	}
}
#include <ShitGraph/Window.hpp>

namespace ShitGraph {
	EventType Event::GetType() const noexcept {
		return std::visit([](const auto& event) noexcept {
			return event.Type;
		}, m_Event);
	}
}
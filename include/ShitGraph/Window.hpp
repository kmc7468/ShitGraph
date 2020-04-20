#pragma once

#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/Graphic.hpp>

#include <variant>

namespace ShitGraph {
	class Window;

	enum class EventType {
		None,

		Paint,
		Destroy,

		MouseDown,
		MouseUp,
		MouseMove,
		MouseWheel,

		KeyDown,
	};

	struct EventBase {
		EventType Type = EventType::None;
		ShitGraph::Window* Window = nullptr;
	};
}

namespace ShitGraph {
	struct PaintEvent final : EventBase {
		ShitGraph::GraphicDevice* Device = nullptr;
	};
}

namespace ShitGraph {
	enum class MouseState {
		None,

		Down,
		Up,
		Move,
	};

	struct MouseEvent final : EventBase {
		MouseState State = MouseState::None;
		Point Cursor;
	};

	struct MouseWheelEvent final : EventBase {
		int Delta;
	};
}

namespace ShitGraph {
	enum class KeyState {
		None,

		Down,
	};

	struct KeyEvent final : EventBase {
		KeyState State = KeyState::None;
		int Key = 0;
	};
}

namespace ShitGraph {
	struct EmptyEvent final : EventBase {};

	class Event final {
	private:
		std::variant<EmptyEvent,
			PaintEvent,
			MouseEvent, MouseWheelEvent,
			KeyEvent> m_Event;

	public:
		Event() noexcept = default;
		Event(const Event&) = delete;
		~Event() = default;

	public:
		Event& operator=(const Event&) = delete;

	public:
		EventType GetType() const noexcept;
	};
}
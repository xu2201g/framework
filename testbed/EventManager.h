#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <memory>
#include <functional>

enum class EventType
{
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	
	Keyboard = sf::Event::Count + 1, //to avoid collisions with sfml event codes
	Mouse,
	Joystick
};

//stores the event code - key codes for keyboard events for example
struct EventInfo
{
	EventInfo()
		:
		m_code(0)
	{}
	EventInfo(int code)
		:
		m_code(code)
	{}

	union
	{
		int m_code;
	};
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct EventDetails
{
	EventDetails(const std::string& bindName)
		:
		m_name(bindName)
	{
		Clear();
	}

	std::string m_name;

	sf::Vector2i m_size;
	sf::Uint32 m_textEntered;
	sf::Vector2i m_mouse;

	int m_mouseWheelDelta;
	int m_keyCode;

	void Clear()
	{
		m_size = sf::Vector2i(0, 0);
		m_textEntered = 0;
		m_mouse = sf::Vector2i(0, 0);
		m_mouseWheelDelta = 0;
		m_keyCode = -1;
	}
};

struct Binding
{
	Binding(const std::string& name)
		:
		m_name(name),
		m_details(name),
		c(0)
	{}

	void BindEvent(EventType type, EventInfo info)
	{
		m_events.emplace_back(type, info);
	}

	Events m_events;
	std::string m_name;
	int c; //count of events

	EventDetails m_details;
};

//store bindings to its key defined by a string - like MOVE_LEFT or TOGGLE_FULLSCREEN
using Bindings = std::unordered_map<std::string, std::unique_ptr<Binding>>;

//callbacks used to call the function bound to the key string - like MOVE_LEFT , &Player::MoveLeft
using Callbacks = std::unordered_map<std::string, std::function<void(EventDetails*)>>;

class EventManager
{
public:

	EventManager();
	~EventManager();

	bool AddBinding(std::unique_ptr<Binding> binding);
	bool RemoveBinding(const std::string& name);

	void SetFocus(const bool& focus);
	
	//callbacks
	template<class T>
	bool AddCallback(const std::string& name, void(T::*func)(EventDetails*), T* instance)
	{
		auto tmp = std::bind(func, instance, std::placeholders::_1);
		return m_callbacks.emplace(name, tmp).second;
	}

	void RemoveCallback(const std::string& name)
	{
		m_callbacks.erase(name);
	}

	void HandleEvent(sf::Event& event);
	void Update();

	sf::Vector2i GetMousePos(sf::RenderWindow* window = nullptr)
	{
		return window ? sf::Mouse::getPosition(*window) : sf::Mouse::getPosition();
	}

private:

	void LoadBindings();

	Bindings m_bindings;
	Callbacks m_callbacks;
	bool m_hasFocus;
};
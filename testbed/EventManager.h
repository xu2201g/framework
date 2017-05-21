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

	//a way to enable key combinations is to count events taking place 
	//like shift + w to sprint 

	//for key combinations it is important to mention that exactly one of those key strokes is handled by 
	//a sfml event, because like in common text editors there is a little delay after you hold a key down
	//so that you get only one key per keystroke into your text unless you hold the key long enough

	//if you would use two or more sfml key events for that it is almost impossible to get both events 
	//at exact the same time 

	//if you would use two or more keyboard events handled in real time - like Keyboard, Mouse and Joystick 
	//you would invoke the callback function probably several times 

	EventDetails m_details;
};

//store bindings to its key defined by a string - like MOVE_LEFT or TOGGLE_FULLSCREEN
using Bindings = std::unordered_map<std::string, std::unique_ptr<Binding>>;

//callbacks used to call the function bound to the key string - like MOVE_LEFT , &Player::MoveLeft
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
enum class StateType;
//set of callbacks stored in a callbackcontainer re mapped to the related state identified by the statetype enum  
using Callbacks = std::unordered_map<StateType, CallbackContainer>;

class EventManager
{
public:

	EventManager();
	~EventManager();

	bool AddBinding(std::unique_ptr<Binding> binding);
	bool RemoveBinding(const std::string& name);

	void SetFocus(const bool& focus);
	void SetCurrentState(const StateType& type);
	const StateType& GetCurrentState() { return m_currentState;	}

	//callbacks
	template<class T>
	bool AddCallback(StateType state, const std::string& name, void(T::*func)(EventDetails*), T* instance)
	{
		auto itr = m_callbacks.emplace(state, CallbackContainer()).first;

		auto tmp = std::bind(func, instance, std::placeholders::_1);
		return itr->second.emplace(name, tmp).second;
	}

	bool RemoveCallback(StateType state, const std::string& name)
	{
		//search for the state
		auto itrState = m_callbacks.find(state);

		//no match
		if (itrState == m_callbacks.end())
		{
			return false;
		}

		//search for the function
		auto itrFunc = itrState->second.find(name);

		//no match
		if (itrFunc == itrState->second.end())
		{
			return false;
		}

		//remove callback function
		itrState->second.erase(name);
		return true;
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
	StateType m_currentState;

	bool m_hasFocus;
	
};
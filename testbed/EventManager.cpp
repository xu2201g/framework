#include "EventManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "StateManager.h"

EventManager::EventManager()
	:
	m_hasFocus(true)
{
	LoadBindings();
}

EventManager::~EventManager()
{
	//no need to delete anything manually cause of used smartpointers
}

bool EventManager::AddBinding(std::unique_ptr<Binding> binding)
{
	//there is already a binding with the string key registered
	if (m_bindings.find(binding->m_name) != m_bindings.end())
	{
		return false;
	}

	//add the new binding
	return m_bindings.emplace(binding->m_name, std::move(binding)).second;
}

bool EventManager::RemoveBinding(const std::string& name)
{
	auto itr = m_bindings.find(name);

	//binding identified by name is not registered
	if (itr == m_bindings.end())
	{
		return false;
	}

	//remove binding
	m_bindings.erase(itr);
	return true;
}

void EventManager::SetFocus(const bool& focus)
{
	m_hasFocus = focus;
}

void EventManager::SetCurrentState(const StateType& type)
{
	m_currentState = type;
}

void EventManager::HandleEvent(sf::Event& event)
{
	//handling sfml events

	//iterate through all bindings
	for (auto &bind_itr : m_bindings)
	{
		//get a raw pointer to the binding the iterator is pointing to
		Binding* bind = bind_itr.second.get();

		//iterate through all events stored in that binding
		for (auto &event_itr : bind->m_events)
		{
			//skip not matching eventtypes
			EventType sfmlEvent = (EventType)event.type;
			if (sfmlEvent != event_itr.first)
			{
				continue;
			}

			//keystroke event
			if (sfmlEvent == EventType::KeyDown ||
				sfmlEvent == EventType::KeyUp)
			{
				//check for matching key values
				if (event_itr.second.m_code == event.key.code)
				{
					//matching event/keystroke

					//store event key code in event details
					bind->m_details.m_keyCode = event.key.code;
					
					//increase count
					++(bind->c);
					break;
				}
			}
			else
			//mouse button event
			if (sfmlEvent == EventType::MButtonDown ||
				sfmlEvent == EventType::MButtonUp)
			{
				//check for matching key values
				if (event_itr.second.m_code == event.mouseButton.button)
				{
					//matching event/mousekeystroke

					//store mouseposition in event details
					bind->m_details.m_mouse.x = event.mouseButton.x;
					bind->m_details.m_mouse.y = event.mouseButton.y;

					//store event key code in event details
					bind->m_details.m_keyCode = event.mouseButton.button;
					
					//increase count
					++(bind->c);
					break;
				}
			}
			else
			{
				//mouse wheel event
				if (sfmlEvent == EventType::MouseWheel)
				{
					bind->m_details.m_mouseWheelDelta = event.mouseWheel.delta;
				}
				else
				//window resized event
				if (sfmlEvent == EventType::WindowResized)
				{
					bind->m_details.m_size.x = event.size.width;
					bind->m_details.m_size.y = event.size.height;
				}
				else
				//text entered event
				if (sfmlEvent == EventType::TextEntered)
				{
					bind->m_details.m_textEntered = event.text.unicode;
				}

				//increase count
				++(bind->c);
			}
		}
	}
}

void EventManager::Update()
{
	//check focus
	if (!m_hasFocus)
	{
		return;
	}
	
	//handling my events

	//iterate through all bindings
	for (auto &bind_itr : m_bindings)
	{
		//get a raw pointer to the binding the iterator is pointing to
		Binding* bind = bind_itr.second.get();

		//iterate through all events stored in that binding
		for (auto &event_itr : bind->m_events)
		{
			switch (event_itr.first)
			{
			case EventType::Keyboard:
				//matching key code - like 89 (sfml code for F5) - may vary with future sfml versions
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(event_itr.second.m_code)))
				{
					//store event key code in event details
					bind->m_details.m_keyCode = event_itr.second.m_code;
					
					++(bind->c);
				}
				break;
			case EventType::Mouse:
				//matching key code - like 89 (sfml code for F5) - may vary with future sfml versions
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button(event_itr.second.m_code)))
				{
					//store event key code in event details
					bind->m_details.m_keyCode = event_itr.second.m_code;
					
					++(bind->c);
				}
				break;
			case EventType::Joystick:
				//may be implemented
				break;
			}
		}

		//process stored events through invoking bound callback functions
		if (bind->m_events.size() == bind->c)
		{
			//state related callbacks
			auto stateCallbacks = m_callbacks.find(m_currentState);

			//global callbacks - like functionality to close the window by 
			//a key/key combination defined one time instead of defining it in 
			//every state we re using in the game
			auto otherCallbacks = m_callbacks.find(StateType(0));

			//handling state callbacks
			if (stateCallbacks != m_callbacks.end())
			{
				//search for the callbackfunction 
				auto callItr = stateCallbacks->second.find(bind->m_name);
				
				//match founds
				if (callItr != stateCallbacks->second.end())
				{
					//invoke the callback function with the passed eventdetailss
					callItr->second(&bind->m_details);
				}
			}

			//handling global callbacks
			if (otherCallbacks != m_callbacks.end())
			{
				//search for the callbackfunction 
				auto callItr = otherCallbacks->second.find(bind->m_name);

				//match founds
				if (callItr != otherCallbacks->second.end())
				{
					//invoke the callback function with the passed eventdetailss
					callItr->second(&bind->m_details);
				}
			}
		}

		bind->c = 0;
		bind->m_details.Clear();
	}
}

void EventManager::LoadBindings()
{
	//define delimiter used in the .cfg file
	std::string  delimiter = ":";

	//open .cfg file
	std::ifstream fstream_bindings;
	fstream_bindings.open("assets//keys.cfg");

	if (!fstream_bindings.is_open())
	{
		std::cout << "! Failed to load keys.cfg" << std::endl;
		return;
	}

	//process file line for line
	std::string line;
	while (std::getline(fstream_bindings, line))
	{
		//store callbackname
		std::stringstream keystream(line);
		std::string callbackName;
				
		keystream >> callbackName;

		std::unique_ptr<Binding> bind(new Binding(callbackName));

		while (!keystream.eof())
		{
			//store keyValue
			std::string keyValue;
			keystream >> keyValue;

			int start = 0;
			int end = keyValue.find(delimiter);
			
			//no valid end position
			if (end == std::string::npos)
			{
				break;
			}

			//extract eventtype and eventcode from keyvalue
			EventType type = EventType(stoi(keyValue.substr(start, end - start)));
			int code = stoi(keyValue.substr(end + delimiter.length(), keyValue.find(delimiter, end + delimiter.length())));

			EventInfo eventInfo;
			eventInfo.m_code = code;

			//bind event
			bind->BindEvent(type, eventInfo);
		}

		if (!AddBinding(std::move(bind)))
		{
			std::cout << "! Failed to add Binding" << std::endl;
		};
	}
	fstream_bindings.close();
}

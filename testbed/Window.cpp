#include "Window.h"

const std::string Window::defaultWindowTitle = "Window";


Window::Window()
{
	Setup(defaultWindowTitle, sf::Vector2u(defaultWindowWidth, defaultWindowHeight));
}

Window::Window(const std::string& title, const sf::Vector2u& size)
{
	Setup(title, size);
}

Window::~Window()
{
	Destroy();
}

void Window::BeginDraw()
{
	//clear the window
	m_window.clear(sf::Color::Black);
}

void Window::EndDraw()
{
	//display on screen what has been rendered to the window so far
	m_window.display();
}

void Window::Update()
{
	//process all window events by polling one after another
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		
		if (event.type == sf::Event::LostFocus)
		{
			m_isFocused = false;
			m_eventManager.SetFocus(false);
		}
		else
		if (event.type == sf::Event::GainedFocus)
		{
			m_isFocused = true;
			m_eventManager.SetFocus(true);
		}
		m_eventManager.HandleEvent(event);
	}

	m_eventManager.Update();
}

bool Window::IsDone()
{
	return m_isDone;
}

bool Window::IsFullscreen()
{
	return m_isFullscreen;
}

bool Window::IsFocused()
{
	return m_isFocused;
}

sf::Vector2u Window::GetWindowSize()
{
	return m_windowSize;
}

EventManager* Window::GetEventManager()
{
	return &m_eventManager;
}

void Window::ToggleFullscreen(EventDetails* details)
{
	//set the flag
	m_isFullscreen = !m_isFullscreen;

	//recreate the window
	Destroy();
	Create();
}

void Window::Close(EventDetails* details)
{
	m_isDone = true;
}

void Window::Draw(sf::Drawable& drawable)
{
	m_window.draw(drawable);
}

void Window::Setup(const std::string& title, const sf::Vector2u& size)
{
	//set members
	m_windowTitle = title;
	m_windowSize = size;
	m_isFullscreen = false;
	m_isDone = false;
	m_isFocused = true;

	m_eventManager.AddCallback("Fullscreen_toggle", &Window::ToggleFullscreen, this);
	m_eventManager.AddCallback("Window_close", &Window::Close, this);

	//create the window
	Create();
}

void Window::Destroy()
{
	m_window.close();
}

void Window::Create()
{
	//set style
	auto style = m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default;

	//create sfml window
	unsigned int channel = 32;
	m_window.create({ m_windowSize.x, m_windowSize.y, channel }, m_windowTitle, style);
}

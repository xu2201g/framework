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
		//set is done flag if we get a close event
		if (event.type == sf::Event::Closed)
		{
			// so it can be closed properly and not suddenly
			m_isDone = true;
		}
		else
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5)
			{
				ToggleFullscreen();
			}
	}
}

bool Window::IsDone()
{
	return m_isDone;
}

bool Window::IsFullscreen()
{
	return m_isFullscreen;
}

sf::Vector2u Window::GetWindowSize()
{
	return m_windowSize;
}

void Window::ToggleFullscreen()
{
	//set the flag
	m_isFullscreen = !m_isFullscreen;

	//recreate the window
	Destroy();
	Create();
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

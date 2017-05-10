#pragma once

#include <SFML/Graphics.hpp>

class Window
{
public:

	Window();
	Window(const std::string& title, const sf::Vector2u& size);
	~Window();

	//clear window
	void BeginDraw();

	//display changes
	void EndDraw();

	//change the model and handle events
	void Update();

	//window needs to be closed?
	bool IsDone();

	bool IsFullscreen();

	sf::Vector2u GetWindowSize();

	void ToggleFullscreen();

	void Draw(sf::Drawable& drawable);

private:

	void Setup(const std::string& title, const sf::Vector2u& size);

	void Destroy();
	void Create();

	static const std::string defaultWindowTitle;
	static constexpr unsigned int defaultWindowWidth = 640;
	static constexpr unsigned int defaultWindowHeight = 480;

	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;

	bool m_isDone;
	bool m_isFullscreen;

};
#pragma once

#include <SFML/Graphics.hpp>

using MessageContainer = std::vector<std::string>;

class Textbox
{
public:

	Textbox();
	Textbox(int visibleLines, int charSize, int width, sf::Vector2f screenPos);

	~Textbox();

	void Setup(int visibleLines, int charSize, int width, sf::Vector2f screenPos);
	void Add(std::string message);
	void Clear();

	void Render(sf::RenderWindow& window);

private:

	MessageContainer m_messages;
	int m_visibleLines;

	sf::RectangleShape m_backdrop;
	sf::Font m_font;
	sf::Text m_content;

};
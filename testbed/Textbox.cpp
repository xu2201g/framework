#include "Textbox.h"

Textbox::Textbox()
{
	Setup(5, 9, 200, sf::Vector2f(16, 16));
}

Textbox::Textbox(int visibleLines, int charSize, int width, sf::Vector2f screenPos)
{
	Setup(visibleLines, charSize, width, screenPos);
}

Textbox::~Textbox()
{
	Clear();
}

void Textbox::Setup(int visibleLines, int charSize, int width, sf::Vector2f screenPos)
{
	m_visibleLines = visibleLines;
	
	sf::Vector2f offset(2.0f, 2.0f);

	m_font.loadFromFile("assets//fonts//arial.ttf");

	m_content.setFont(m_font);	
	m_content.setString("");
	m_content.setCharacterSize(charSize);
	m_content.setFillColor(sf::Color::White);
	m_content.setPosition(screenPos + offset);

	m_backdrop.setSize(sf::Vector2f(width, visibleLines * charSize * 1.2f));
	m_backdrop.setFillColor(sf::Color(90, 90, 90, 90));
	m_backdrop.setPosition(screenPos);
}

void Textbox::Add(std::string message)
{
	m_messages.push_back(message);

	//textbox holds only 5 messages at once
	if (m_messages.size() < 6) { return; }
	m_messages.erase(m_messages.begin());
}

void Textbox::Clear()
{
	m_messages.clear();
}

void Textbox::Renderer(sf::RenderWindow& window)
{
	std::string content;

	//store messages in content string
	for (auto &itr : m_messages)
	{
		content.append(itr + "\n");
	}

	if (content != "")
	{
		m_content.setString(content);
		window.draw(m_backdrop);
		window.draw(m_content);
	}
}

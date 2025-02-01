///////////////////////////////////////////////////////////////////////////////
///
/// MIT License
///
/// Copyright(c) 2025 TekyoDrift
///
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following coditions:
///
/// The above copyright notice and this permission notice shall be included
/// in all copies or substantial portions of the Software?
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Dependencies
///////////////////////////////////////////////////////////////////////////////
#include "GraphicalMonitor.hpp"
#include <cmath>
#include <iostream>
///////////////////////////////////////////////////////////////////////////////
GraphicalMonitor::GraphicalMonitor(void) : IMonitorDisplay(), m_selected(0)
{
}

///////////////////////////////////////////////////////////////////////////////
GraphicalMonitor::~GraphicalMonitor()
{
}

///////////////////////////////////////////////////////////////////////////////
/*
Todo:
- navigation bar ✅
- button creation system ✅
- getinputs ✅
- mouse inputsgi
- graphs
- getinfo
- get processor info
- get mem info
- geet network info



creat boxes that change colors when pressed
*/

void GraphicalMonitor::createBackground(sf::RenderWindow &window,
    const sf::Vector2f &size, const sf::Vector2f &pos, sf::Color fillColor,
    float outline, sf::Color outlineColor) const
{
    sf::RectangleShape rect(size);
    rect.setPosition(pos);
    rect.setFillColor(fillColor);
    rect.setOutlineThickness(outline);
    rect.setOutlineColor(outlineColor);

    window.draw(rect);
}

void GraphicalMonitor::createBackground(sf::RenderWindow &window,
    const sf::Vector2f &size, const sf::Vector2f &pos,
    sf::Color fillColor) const
{
    sf::RectangleShape rect(size);
    rect.setPosition(pos);
    rect.setFillColor(fillColor);

    window.draw(rect);
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::textbox(sf::RenderWindow &window,
    const sf::Vector2f &size, const sf::Vector2f &pos, sf::Color fillColor,
    const std::string &textString) const
{
    // Create the rectangle (button)
    sf::RectangleShape rect(size);
    rect.setPosition(pos);
    rect.setFillColor(fillColor);

    sf::Font font;
    if (!font.loadFromFile(
            "/usr/share/fonts/google-carlito-fonts/Carlito-Regular.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    // Create the text
    sf::Text text;
    text.setFont(font);
    text.setString(textString);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color(32, 32, 32));

    text.setOrigin(0, 0);

    // Position the text in the rectangle (left-aligned)
    text.setPosition(pos.x + 10.f, pos.y + (size.y - text.getLocalBounds().height) / 4.f);

    // Draw both
    window.draw(rect);
    window.draw(text);
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::textbox(sf::RenderWindow &window,
    const sf::Vector2f &size, const sf::Vector2f &pos, sf::Color fillColor,
    const std::string &textString, float outline, sf::Color outlineColor) const
{
    // Create the rectangle (button)
    sf::RectangleShape rect(size);
    rect.setPosition(pos);
    rect.setFillColor(fillColor);
    rect.setOutlineThickness(outline);
    rect.setOutlineColor(outlineColor);

    sf::Font font;
    if (!font.loadFromFile(
            "/usr/share/fonts/google-carlito-fonts/Carlito-Regular.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    // Create the text
    sf::Text text;
    text.setFont(font);
    text.setString(textString);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color(32, 32, 32));

    text.setOrigin(0, 0);

    // Position the text in the rectangle (left-aligned)
    text.setPosition(pos.x + 10.f, pos.y + (size.y - text.getLocalBounds().height) / 3.f);

    // Draw both
    window.draw(rect);
    window.draw(text);
}

/*
info:
- taskbar is 240/1115 aka 22% of the x axis
- buttons are 36/680 about 5% of y axis and 20% x
- title is 50/677 about 7.5%;
*/


///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::backgroundBuild(sf::RenderWindow &window) const
{
    static const char* contentItems[] = {
        "Information", "Processor", "Memory", "Network", "Credits"
    };

    sf::Vector2f sidebar_size = {static_cast<float>(window.getSize().x)
            * 0.22f,
        static_cast<float>(window.getSize().y)};
    sf::Vector2f sidebar_pos = {0, 0};

    sf::Vector2f content_size = {static_cast<float>(window.getSize().x) - sidebar_pos.x,
        static_cast<float>(window.getSize().y)};
    sf::Vector2f content_pos = {sidebar_size.x, 0};

        sf::Vector2f title_size = {content_size.x,
            static_cast<float>(window.getSize().y * 0.07f)};
    sf::Vector2f title_pos = {sidebar_size.x, 0.f};

    
    //side bar
    createBackground(window, sidebar_size, sidebar_pos, sf::Color(192, 192, 192));
    //content
    createBackground(window, content_size, content_pos, sf::Color(215, 215, 215));
    //titlecard
    textbox(window, title_size, title_pos, sf::Color::Transparent, contentItems[m_selected], 1.f, sf::Color(164, 164, 164));
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::PrintNavBar(sf::RenderWindow &window) const
{
    static const char *menuItems[] = {"Information", "Processor", "Memory",
        "Network", "Credits"};

    backgroundBuild(window);
    const sf::Vector2 buttonSize = {static_cast<float>(window.getSize().x)
            * 0.21f,
        window.getSize().x * 0.04f};

    const sf::Vector2 firstButtonPos = {2.f, 2.f};

    for (size_t i = 0; i < sizeof(menuItems) / sizeof(menuItems[0]); i++) {
        float yOffset = window.getSize().y * 0.06f * i;
        if (i == m_selected) {
            textbox(window, buttonSize,
                {firstButtonPos.x, firstButtonPos.y + yOffset},
                sf::Color(164, 164, 164), menuItems[i]);
        } else {
            textbox(window, buttonSize,
                {firstButtonPos.x, firstButtonPos.y + yOffset},
                sf::Color(192, 192, 192), menuItems[i]);
        }
    }
}



///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::handleInput(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up && m_selected > 0)
            m_selected--;
        if (event.key.code == sf::Keyboard::Down && m_selected < 4)
            m_selected++;
        // std::cout << m_selected << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
int GraphicalMonitor::loop(void)
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "MyGKrellm",
        sf::Style::Close);
    sf::Event event;

    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            handleInput(event);
        }

        refresh();

        window.clear();

        // TEMPORARY
        PrintNavBar(window);
        // !TEMPORARY

        window.display();
    }
    return (0);
}

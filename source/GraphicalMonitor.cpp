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
- navigation bar
- button creation system
- getinputs
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
void GraphicalMonitor::drawButton(sf::RenderWindow &window,
    const sf::Vector2f &size, const sf::Vector2f &pos, sf::Color fillColor,
    const std::string &textString) const
{
    // Create the rectangle (button)
    sf::RectangleShape rect(size);
    rect.setPosition(pos);
    rect.setFillColor(fillColor);

    sf::Font font;
    if (!font.loadFromFile(
            "/usr/share/fonts/dejavu-sans-fonts/DejaVuSans.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    // Create the text
    sf::Text text;
    text.setFont(font);
    text.setString(textString);
    text.setCharacterSize(12);
    text.setFillColor(sf::Color::White);

    // Center text within the rectangle
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    text.setPosition(pos.x + size.x / 3.f, pos.y + size.y / 2.f);

    // Draw both
    window.draw(rect);
    window.draw(text);
}

/*
info:
- taskbar is 240/1115 aka 22% of the x axis
- buttons are 36/680 about 5% of y axis and 20% x
*/
///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::PrintNavBar(sf::RenderWindow &window) const
{
    sf::Vector2f sidebar_size = {static_cast<float>(window.getSize().x)
            * 0.22f,
        static_cast<float>(window.getSize().y)};
    sf::Vector2f sidebar_pos = {0, 0};

    const sf::Vector2 buttonSize = {static_cast<float>(window.getSize().x)
            * 0.21f,
        static_cast<float>(window.getSize().y) * 0.05f};
    const sf::Vector2 firstButtonPos = {2.f, 2.f};

    createBackground(window, sidebar_size, sidebar_pos,
        sf::Color(192, 192, 192));

    drawButton(window, buttonSize, firstButtonPos, sf::Color(64, 64, 64),
        "long ass name");

}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::handleInput(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::Up)
            m_selected++;
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

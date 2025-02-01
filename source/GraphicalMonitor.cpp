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
GraphicalMonitor::GraphicalMonitor(void)
    : IMonitorDisplay(), m_selected(0), m_buttons(), _sidebarSize(),
      _titlecardSize()
{
    if (!m_font.loadFromFile(
            "/usr/share/fonts/google-carlito-fonts/Carlito-Regular.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
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
- mouse inputsgi ✅
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

///////////////////////////////////////////////////////////////////////////////
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
void GraphicalMonitor::drawTextBox(sf::RenderWindow &window,
    sf::RectangleShape &rect, sf::Text &text, const sf::Vector2f &size,
    const sf::Vector2f &pos, sf::Color fillColor, const std::string &textString)
{
    // Create the rectangle (button or textbox)
    rect.setSize(size);
    rect.setPosition(pos);
    rect.setFillColor(fillColor);

    // Create the text
    text.setFont(m_font);
    text.setString(textString);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color(32, 32, 32));

    text.setOrigin(0, 0);

    // Position the text in the rectangle (left-aligned)
    text.setPosition(pos.x + 10.f,
        pos.y + (size.y - text.getLocalBounds().height) / 4.f);

    // Draw both
    window.draw(rect);
    window.draw(text);
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::button(sf::RenderWindow &window,
    const sf::Vector2f &size, const sf::Vector2f &pos, sf::Color fillColor,
    const std::string &textString)
{
    Button button;
    // Call the drawTextBox function to handle the common drawing logic
    drawTextBox(window, button.rect, button.text, size, pos, fillColor, textString);
    m_buttons.push_back(button);
}

void GraphicalMonitor::textbox(sf::RenderWindow &window,
    const sf::Vector2f &size, const sf::Vector2f &pos, sf::Color fillColor,
    const std::string &textString)
{
    sf::RectangleShape rect;
    sf::Text text;
    // Call the drawTextBox function to handle the common drawing logic
    drawTextBox(window, rect, text, size, pos, fillColor, textString);
}


// //outlined textbox /no button with outline
// ///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::textbox(sf::RenderWindow &window,
    const sf::Vector2f &size, const sf::Vector2f &pos, sf::Color fillColor,
    const std::string &textString, float outline, sf::Color outlineColor) const
{
    sf::RectangleShape rect(size);
    rect.setPosition(pos);
    rect.setFillColor(fillColor);
    rect.setOutlineThickness(outline);
    rect.setOutlineColor(outlineColor);

    // Create the text
    sf::Text text;
    text.setFont(m_font);
    text.setString(textString);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color(32, 32, 32));

    text.setOrigin(0, 0);

    // Position the text in the rectangle (left-aligned)
    text.setPosition(pos.x + 10.f,
        pos.y + (size.y - text.getLocalBounds().height) / 3.f);

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
void GraphicalMonitor::backgroundBuild(sf::RenderWindow &window)
{
    static const char *contentItems[] = {"Information", "Processor", "Memory",
        "Network", "Credits"};

    //side bar
    _sidebarSize = {static_cast<float>(window.getSize().x) * 0.22f,
        static_cast<float>(window.getSize().y)};
    sf::Vector2f sidebar_pos = {0, 0};
    createBackground(window, _sidebarSize, sidebar_pos,
        sf::Color(192, 192, 192));

    //content
    sf::Vector2f content_size = {static_cast<float>(window.getSize().x)
            - _sidebarSize.x,
        static_cast<float>(window.getSize().y)};
    sf::Vector2f content_pos = {_sidebarSize.x, 0};
    createBackground(window, content_size, content_pos,
        sf::Color(225, 225, 225));

    //titlecard
    _titlecardSize = {content_size.x,
        static_cast<float>(window.getSize().y * 0.07f)};
    sf::Vector2f title_pos = {_sidebarSize.x, 0.f};

    textbox(window, _titlecardSize, title_pos, sf::Color::Transparent,
        contentItems[m_selected], 1.f, sf::Color(164, 164, 164));
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::PrintNavBar(sf::RenderWindow &window)
{
    static const char *menuItems[] = {"Information", "Processor", "Memory",
        "Network", "Credits"};

    backgroundBuild(window);
    const sf::Vector2 buttonSize = {static_cast<float>(window.getSize().x)
            * 0.21f,
        window.getSize().x * 0.04f};

    const sf::Vector2 firstButtonPos = {2.f, 2.f};

    if (m_buttons.size() != 0)
        m_buttons.clear();
    for (size_t i = 0; i < sizeof(menuItems) / sizeof(menuItems[0]); i++) {
        float yOffset = window.getSize().y * 0.06f * i;
        sf::Color fillColor = (i == m_selected) ? sf::Color(164, 164, 164)
                                                : sf::Color(192, 192, 192);
        button(window, buttonSize,
            {firstButtonPos.x, firstButtonPos.y + yOffset}, fillColor,
            menuItems[i]);
    }
}

///////////////////////////////////////////////////////////////////////////////
sf::Vector2f GraphicalMonitor::subTitle(sf::RenderWindow &window,
    const std::string subtitle, float yPos)
{
    sf::Text text(subtitle, m_font, 16);
    sf::FloatRect textBounds = text.getLocalBounds();

    sf::Vector2f title_pos = {_sidebarSize.x, yPos};
    sf::Vector2f title_size = {textBounds.width, _titlecardSize.y};

    textbox(window, title_size, title_pos, sf::Color::Transparent, subtitle);

    // Create the line
    float marginFromRight = static_cast<float>(window.getSize().y * 0.05f);
    float line_start_x = title_pos.x + title_size.x;
    float line_width = window.getSize().x - line_start_x - marginFromRight;

    // Position for the line next to the title
    sf::Vector2f line_pos = {line_start_x, title_pos.y + title_size.y / 2.f};
    sf::Vector2f line_size = {line_width, 2.f};

    createBackground(window, line_size, line_pos, sf::Color(164, 164, 164));
    return title_size;
}

///////////////////////////////////////////////////////////////////////////////
sf::Vector2f GraphicalMonitor::textPrepper(sf::RenderWindow &window,
    const std::string words, sf::Vector2f pos)
{
    sf::Text text(words, m_font, 14);
    sf::FloatRect textBounds = text.getLocalBounds();

    sf::Vector2f size = {textBounds.width, _titlecardSize.y};
    textbox(window, size, pos, sf::Color::Transparent, words);
    return size;
}

// add the y pos every time together and send it.
///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::printInfo(sf::RenderWindow &window)
{
    float margine = static_cast<float>(window.getSize().y * 0.01f);

    sf::Vector2f pos = {_sidebarSize.x, _titlecardSize.y};

    pos.y += subTitle(window, "[t] time module", pos.y).y;
    if (m_time.isEnabled()) {
        pos.y += textPrepper(window, std::string("\tCurrent Date: ") + m_time.getTime().c_str(), pos).y;
        pos.y += margine;
    }

    pos.y += subTitle(window, "[u] User module", pos.y).y;
    if (m_user.isEnabled()) {
        pos.y += textPrepper(window, std::string("\tUser Name: ") + m_user.getUserName().c_str(), pos).y;
        pos.y += textPrepper(window, std::string("\tHost Name: ") + m_user.getHostName().c_str(), pos).y;
        pos.y += margine;
    }
    pos.y += subTitle(window, "[o] Operating System Module", pos.y).y;
    if (m_os.isEnabled()) {
        pos.y += textPrepper(window, std::string("\tOs Name: ") + m_os.getOs().c_str(), pos).y;
        pos.y += textPrepper(window, std::string("\tKernel Name: ") + m_os.getKernel().c_str(), pos).y;
        pos.y += margine;
    }
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::handlekeys(sf::Event event, sf::RenderWindow &window)
{
    if (event.key.code == sf::Keyboard::Up && m_selected > 0)
        m_selected--;
    if (event.key.code == sf::Keyboard::Down && m_selected < 4)
        m_selected++;
    if (event.key.code == sf::Keyboard::U)
        m_user.setEnabled(!m_user.isEnabled());
    if (event.key.code == sf::Keyboard::M)
        m_memory.setEnabled(!m_memory.isEnabled());
    if (event.key.code == sf::Keyboard::O)
        m_os.setEnabled(!m_os.isEnabled());
    if (event.key.code == sf::Keyboard::P)
        m_cpu.setEnabled(!m_cpu.isEnabled());
    if (event.key.code == sf::Keyboard::N)
        m_network.setEnabled(!m_network.isEnabled());
    if (event.key.code == sf::Keyboard::T)
        m_time.setEnabled(!m_time.isEnabled());
    if (event.key.code == sf::Keyboard::Q)
        window.close();
}
///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::handlemouse(sf::Event event)
{
    if (event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = {static_cast<float>(event.mouseButton.x),
            static_cast<float>(event.mouseButton.y)};
        for (size_t i = 0; i < m_buttons.size(); i++) {
            if (m_buttons[i].rect.getGlobalBounds().contains(mousePos))
                m_selected = i;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::handleInput(sf::Event event, sf::RenderWindow &window)
{
    if (event.type == sf::Event::KeyPressed)
        handlekeys(event, window);
    handlemouse(event);
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
            handleInput(event, window);
        }

        refresh();

        window.clear();

        // TEMPORARY
        PrintNavBar(window);
        if (m_selected == 0)
            printInfo(window);
        // !TEMPORARY

        window.display();
    }
    return (0);
}

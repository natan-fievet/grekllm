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
- getinfo ✅
- graphs
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
    const sf::Vector2f &pos, sf::Color fillColor,
    const std::string &textString, float outline, sf::Color outlineColor)
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
    rect.setOutlineThickness(outline);
    rect.setOutlineColor(outlineColor);
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
    drawTextBox(window, button.rect, button.text, size, pos, fillColor,
        textString, 0.f, sf::Color::Transparent);
    m_buttons.push_back(button);
}

void GraphicalMonitor::textbox(sf::RenderWindow &window,
    const sf::Vector2f &size, const sf::Vector2f &pos, sf::Color fillColor,
    const std::string &textString)
{
    sf::RectangleShape rect;
    sf::Text text;
    // Call the drawTextBox function to handle the common drawing logic
    drawTextBox(window, rect, text, size, pos, fillColor, textString, 0.f,
        sf::Color::Transparent);
}

// //outlined textbox /no button with outline
// ///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::textbox(sf::RenderWindow &window,
    const sf::Vector2f &size, const sf::Vector2f &pos, sf::Color fillColor,
    const std::string &textString, float outline, sf::Color outlineColor)
{
    sf::RectangleShape rect;
    sf::Text text;
    drawTextBox(window, rect, text, size, pos, fillColor, textString, outline,
        outlineColor);
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
    sf::Text text(words, m_font, 28);
    sf::FloatRect textBounds = text.getLocalBounds();

    // sf::Vector2f size = {textBounds.width, _titlecardSize.y};

    sf::Vector2f size = {textBounds.width,
        _titlecardSize.y - (_titlecardSize.y - textBounds.height)};
    textbox(window, size, pos, sf::Color::Transparent, words);
    return size;
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
void GraphicalMonitor::printInfo(sf::RenderWindow &window)
{
    float margine = static_cast<float>(window.getSize().y * 0.01f);

    sf::Vector2f pos = {_sidebarSize.x, _titlecardSize.y};

    pos.y += subTitle(window, "[t] time module", pos.y).y;
    if (m_time.isEnabled()) {
        pos.y += textPrepper(window,
            std::string("\tCurrent Date: ") + m_time.getTime().c_str(), pos).y;
        pos.y += margine;
    }

    pos.y += subTitle(window, "[u] User module", pos.y).y;
    if (m_user.isEnabled()) {
        pos.y += textPrepper(window,
            std::string("\tUser Name: ") + m_user.getUserName().c_str(), pos).y;
        pos.y += textPrepper(window,
            std::string("\tHost Name: ") + m_user.getHostName().c_str(), pos).y;
        pos.y += margine;
    }
    pos.y += subTitle(window, "[o] Operating System Module", pos.y).y;
    if (m_os.isEnabled()) {
        pos.y += textPrepper(window,
            std::string("\tOs Name: ") + m_os.getOs().c_str(), pos).y;
        pos.y += textPrepper(window,
            std::string("\tKernel Name: ") + m_os.getKernel().c_str(), pos).y;
        pos.y += margine;
    }
}

void drawMemoryGraph(sf::RenderWindow &window, const std::list<MemoryModule::Data> &graph,
    const sf::Vector2f &size, const sf::Vector2f &position, bool isSwap)
{
    if (graph.empty()) return;

    sf::VertexArray lineGraph(sf::LineStrip, graph.size());
    sf::VertexArray fillGraph(sf::TriangleStrip, graph.size() * 2);

    float maxHeight = size.y; // Graph height
    float maxWidth = size.x;  // Graph width

    // Get the last `size.x` data points
    std::vector<float> values;
    auto it = graph.end();
    int dataPoints = std::min(static_cast<int>(graph.size()), static_cast<int>(size.x));

    for (int i = 0; i < dataPoints; ++i) {
        --it;
        values.insert(values.begin(), isSwap ? it->swap : it->mem);
        if (it == graph.begin()) break;
    }

    // Normalize and map values to the graph size
    for (size_t i = 0; i < values.size(); i++) {
        float value = values[i]; // Value in percentage (0-100)
        float x = position.x + (i * (maxWidth / values.size()));
        float y = position.y + maxHeight - (value / 100.f * maxHeight);

        // Line Graph (Top Border)
        lineGraph[i].position = sf::Vector2f(x, y);
        lineGraph[i].color = sf::Color::Green; // Customize color

        // Filled Area (Triangle Strip)
        fillGraph[i * 2].position = sf::Vector2f(x, y);
        fillGraph[i * 2].color = sf::Color(0, 255, 0, 150); // Semi-transparent green

        fillGraph[i * 2 + 1].position = sf::Vector2f(x, position.y + maxHeight);
        fillGraph[i * 2 + 1].color = sf::Color(0, 255, 0, 50); // Lighter green for fade effect
    }

    // Draw the filled area first, then the line graph
    window.draw(fillGraph);
    window.draw(lineGraph);
}


std::string formatFloat(float value) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    return stream.str();
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::printmem(sf::RenderWindow &window)
{
    float margine = static_cast<float>(window.getSize().y * 0.01f);

    sf::Vector2f pos = {_sidebarSize.x, _titlecardSize.y};
    pos.y += textPrepper(window, "[m] Memory Usage", pos).y;

    sf::Vector2f graphSize = {
        (static_cast<float>(window.getSize().x) - _sidebarSize.x) * 0.8f,
        static_cast<float>(window.getSize().y * 0.25f)};
    sf::Vector2f graphPos = {_sidebarSize.x
            + static_cast<float>(window.getSize().x * 0.05f),
        pos.y};

    createBackground(window, graphSize, graphPos, sf::Color::White, 2.f,
            sf::Color::Magenta);
    drawMemoryGraph(window, m_memory.getGraph(), {graphSize.x + 2.f, graphSize.y}, {graphPos.x + 2.f, graphPos.y}, false);
    pos.y += graphSize.y;
    pos.y += textPrepper(window, "\t\t\tMemory " + formatFloat(m_memory.getMemoryUsed()) + "%", pos).y;
    pos.y += margine;
    createBackground(window, graphSize, {graphPos.x, pos.y}, sf::Color::White, 2.f,
            sf::Color::Magenta);
    drawMemoryGraph(window, m_memory.getGraph(), {graphSize.x + 2.f, graphSize.y}, {graphPos.x + 2.f, pos.y}, true);
    pos.y += graphSize.y;
    pos.y += textPrepper(window, "\t\t\tMemory " + formatFloat(m_memory.getSwapUsed()) + "%", pos).y;
    pos.y += margine;

    sf::Vector2f memPos = pos;
    sf::Vector2f memSize = pos;

    memSize += textPrepper(window, "Memory Usage", memPos);
    memPos.y = memSize.y;
    memPos.y += margine;
    float memMargine = memPos.y;

    memPos.y += textPrepper(window, "Total: " + formatFloat(m_memory.getMemoryTotal() / 1024.f / 1024.f) + "GB", memPos).y;
    memMargine = memPos.y - memMargine;
    textPrepper(window, "Used: " + formatFloat((m_memory.getMemoryTotal() * m_memory.getMemoryUsed() / 100.f) / 1024.f / 1024.f) + "GB" + " (" + formatFloat(m_memory.getMemoryUsed()) + "%)", memPos);
    memPos.y += memMargine;
    textPrepper(window, "Free: " + formatFloat(m_memory.getMemoryFree() / 1024.f / 1024.f) + "GB", memPos);
    memPos.y += memMargine;

    sf::Vector2f swaPos = {memSize.x, pos.y};
    swaPos.y += textPrepper(window, "Swap Usage", swaPos).y;
    swaPos.y += margine;
    float swapMargine = swaPos.y;

    swaPos.y += textPrepper(window, "Total: " + formatFloat(m_memory.getSwapTotal() / 1024.f / 1024.f) + "GB", swaPos).y;
    swapMargine = swaPos.y - swapMargine;
    textPrepper(window, "Used: " + formatFloat((m_memory.getSwapTotal() * m_memory.getSwapUsed() / 100.f) / 1024.f / 1024.f) + "GB" + " (" + formatFloat(m_memory.getSwapUsed()) + "%)", swaPos);
    swaPos.y += swapMargine;
    textPrepper(window, "Free: " + formatFloat(m_memory.getSwapTotal() / 1024.f / 1024.f) + "GB", swaPos);
    swaPos.y += swapMargine;
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

        PrintNavBar(window);
        if (m_selected == 0)
            printInfo(window);
        if (m_selected == 2)
            printmem(window);

        window.display();
    }
    return (0);
}

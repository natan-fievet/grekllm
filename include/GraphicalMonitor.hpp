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
// Pragma
///////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Dependencies
///////////////////////////////////////////////////////////////////////////////
#include "IMonitorDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iomanip>
#include <sstream>
#include <vector>

struct Button {
    sf::RectangleShape rect;
    sf::Text text;
};

///////////////////////////////////////////////////////////////////////////////
/// \brief Utility class to handle graphical monitor
///////////////////////////////////////////////////////////////////////////////
class GraphicalMonitor : public IMonitorDisplay {

private:
    ///////////////////////////////////////////////////////////////////////////
    // Private properties
    ///////////////////////////////////////////////////////////////////////////
    size_t m_selected = 0;
    sf::Font m_font;
    std::vector<Button> m_buttons;// Store all buttons here
    sf::Vector2f _sidebarSize;
    sf::Vector2f _titlecardSize;

public:
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    GraphicalMonitor(void);

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ~GraphicalMonitor();

public:
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    int loop(void) override;

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    void PrintNavBar(sf::RenderWindow &window);
    void printInfo(sf::RenderWindow &window);
    void printProcessor(sf::RenderWindow &window);
    void printmem(sf::RenderWindow &window);
    void printNetwork(sf::RenderWindow &window);

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    void createBackground(sf::RenderWindow &window, const sf::Vector2f &size,
        const sf::Vector2f &pos, sf::Color fillColor, float outline,
        sf::Color outlineColor) const;
    void createBackground(sf::RenderWindow &window, const sf::Vector2f &size,
        const sf::Vector2f &pos, sf::Color fillColor) const;

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    void drawTextBox(sf::RenderWindow &window, sf::RectangleShape &rect,
        sf::Text &text, const sf::Vector2f &size, const sf::Vector2f &pos,
        sf::Color fillColor, const std::string &textString, float outline,
        sf::Color outlineColor);

    void textbox(sf::RenderWindow &window, const sf::Vector2f &size,
        const sf::Vector2f &pos, sf::Color fillColor,
        const std::string &textString);

    void button(sf::RenderWindow &window, const sf::Vector2f &size,
        const sf::Vector2f &pos, sf::Color fillColor,
        const std::string &textString);

    void textbox(sf::RenderWindow &window, const sf::Vector2f &size,
        const sf::Vector2f &pos, sf::Color fillColor,
        const std::string &textString, float outline, sf::Color outlineColor);

    void backgroundBuild(sf::RenderWindow &window);

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    sf::Vector2f textPrepper(sf::RenderWindow &window, const std::string words,
        sf::Vector2f pos);
    sf::Vector2f subTitle(sf::RenderWindow &window, const std::string subtitle,
        float yPos);

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    void handleInput(sf::Event event, sf::RenderWindow &window);
    void handlekeys(sf::Event event, sf::RenderWindow &window);
    void handlemouse(sf::Event event);

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    void drawCPUGraph(sf::RenderWindow &window, const std::list<double> &graph,
        const sf::Vector2f &size, const sf::Vector2f &position);
    void drawMemoryGraph(sf::RenderWindow &window,
        const std::list<MemoryModule::Data> &graph, const sf::Vector2f &size,
        const sf::Vector2f &position, bool isSwap);
    void drawNetworkGraph(sf::RenderWindow &window,
        const std::list<NetworkModule::Data> &graph, const sf::Vector2f &size,
        const sf::Vector2f &position, bool isUpload);
};

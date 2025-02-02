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
- graphs ✅
- get processor info ✅
- get mem info ✅
- geet network info ✅
- processus info ✅
- credit ✅



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
///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::textbox(sf::RenderWindow &window,
    const sf::Vector2f &size, const sf::Vector2f &pos, sf::Color fillColor,
    const std::string &textString, float outline, sf::Color outlineColor)
{
    sf::RectangleShape rect;
    sf::Text text;
    drawTextBox(window, rect, text, size, pos, fillColor, textString, outline,
        outlineColor);
}


void GraphicalMonitor::drawgrid(sf::RenderWindow &window, const sf::Vector2f &size, const sf::Vector2f &pos, sf::Color fillColor,
    float outline, sf::Color outlineColor, sf::Vector2f spacing)
{
    createBackground(window, size, pos, fillColor, outline, outlineColor);
    float verticalSpacing = size.x * spacing.x;
    float horizontalSpacing = size.y * spacing.y;

    size_t gridAmountX = static_cast<size_t>(size.x / verticalSpacing);
    size_t gridAmountY = static_cast<size_t>(size.y / horizontalSpacing);

    sf::Vector2f verticalGridLines = {1.f, size.y};
    sf::Vector2f horizontalGridLines = {size.x, 1.f};

    float verticalAxis = pos.x;
    float horizontalAxis = pos.y;

    for (size_t i = 0; i <= gridAmountX; i++){
        createBackground(window, verticalGridLines, {verticalAxis, pos.y}, sf::Color(164, 164, 164));
        verticalAxis += verticalSpacing;
    }

    // Draw horizontal grid lines
    for (size_t i = 0; i <= gridAmountY; i++){
        createBackground(window, horizontalGridLines, {pos.x, horizontalAxis}, sf::Color(164, 164, 164));
        horizontalAxis += horizontalSpacing;
    }
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
    static const char *contentItems[] = {"Information", "[p] Processor", "[m] Memory",
        "[e] Processus", "[n] Network", "[d] Disk", "Credits"};

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

    sf::Vector2f size = {textBounds.width,
        _titlecardSize.y - (_titlecardSize.y - textBounds.height)};
    textbox(window, size, pos, sf::Color::Transparent, words);
    return size;
}

///////////////////////////////////////////////////////////////////////////////
std::string formatFloat(float value) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    return stream.str();
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::PrintNavBar(sf::RenderWindow &window)
{
    static const char *menuItems[] = {"Information", "Processor", "Memory",
        "Processus", "Network", "Disk", "Credits"};

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

void drawGraph(sf::RenderWindow &window, const std::vector<float> &values,
               const sf::Vector2f &size, const sf::Vector2f &position,
               sf::Color lineColor, sf::Color fillColor)
{
    if (values.empty()) return;

    sf::VertexArray lineGraph(sf::LineStrip, values.size());
    sf::VertexArray fillGraph(sf::TriangleStrip, values.size() * 2);

    float maxHeight = size.y;
    float maxWidth = size.x;

    for (size_t i = 0; i < values.size(); i++) {
        float value = values[i]; // Value in percentage (0-100)
        float x = position.x + (i * (maxWidth / values.size()));
        float y = position.y + maxHeight - (value / 100.f * maxHeight);

        // Line Graph
        lineGraph[i].position = sf::Vector2f(x, y);
        lineGraph[i].color = lineColor;

        // Filled Area
        fillGraph[i * 2].position = sf::Vector2f(x, y);
        fillGraph[i * 2].color = fillColor;

        fillGraph[i * 2 + 1].position = sf::Vector2f(x, position.y + maxHeight);
        fillGraph[i * 2 + 1].color = sf::Color(fillColor.r, fillColor.g, fillColor.b, 50); // Lighter fill color
    }

    // Draw the filled area first, then the line graph
    window.draw(fillGraph);
    window.draw(lineGraph);
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::drawCPUGraph(sf::RenderWindow &window, const std::list<double> &graph,
                  const sf::Vector2f &size, const sf::Vector2f &position)
{
    if (graph.empty()) return;

    std::vector<float> values;
    auto it = graph.end();
    int dataPoints = std::min({static_cast<int>(graph.size()), static_cast<int>(size.x), 75});

    //convert data into a float list
    for (int i = 0; i < dataPoints; ++i) {
        --it;
        values.insert(values.begin(), static_cast<float>(*it));
        if (it == graph.begin()) break;
    }

    sf::Color line = (m_cpu.isEnabled()) ? sf::Color::Cyan : sf::Color(64, 64, 64);
    sf::Color fill = (m_cpu.isEnabled()) ? sf::Color(0, 255, 255, 150) : sf::Color(128, 128, 128);
    drawGraph(window, values, size, position, line, fill);
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::printProcessor(sf::RenderWindow &window)
{
    float margine = static_cast<float>(window.getSize().y * 0.01f);
    sf::Vector2f pos = {_sidebarSize.x, _titlecardSize.y};
    pos.y += margine * 4;

    sf::Vector2f graphSize = {
        (static_cast<float>(window.getSize().x) - _sidebarSize.x) * 0.8f,
        static_cast<float>(window.getSize().y * 0.5f)};
    sf::Vector2f graphPos = {_sidebarSize.x
            + static_cast<float>(window.getSize().x * 0.05f),
        pos.y};
    drawgrid(window, graphSize, graphPos, sf::Color::White, 2.f,
            sf::Color::Blue, {0.1f, 0.1f});
    drawCPUGraph(window, m_cpu.getGraph(), graphSize, graphPos);
    pos.y += graphSize.y;
    pos.y += textPrepper(window, "\t\t\t" + formatFloat(m_cpu.getUsage()) + "%", pos).y;

    pos.y += margine * 2;

    textPrepper(window, std::string("vendor: ") + m_cpu.getProcs()[0]["vendorId"].c_str(), pos);
    pos.y += margine * 4;
    textPrepper(window,( std::string("CPU Model: ") + m_cpu.getProcs()[0]["modelName"].c_str()), pos);
    pos.y += margine * 4;
    textPrepper(window, std::string("CPU Cores: ") + m_cpu.getProcs()[0]["cpuCores"].c_str(), pos);
    pos.y += margine * 4;
    textPrepper(window, std::string("Cache Size: ") + m_cpu.getProcs()[0]["cacheSize"].c_str(), pos);
    pos.y += margine * 4;
    textPrepper(window, std::string("Physical Cores: ") + std::to_string(m_cpu.getProcs().size()), pos);
    pos.y += margine * 4;
    textPrepper(window, std::string("Address Sizes: ") + m_cpu.getProcs()[0]["addressSizes"].c_str(), pos);
    pos.y += margine * 4;
}


///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::drawMemoryGraph(sf::RenderWindow &window, const std::list<MemoryModule::Data> &graph,
                     const sf::Vector2f &size, const sf::Vector2f &position, bool isSwap)
{
    if (graph.empty()) return;

    std::vector<float> values;
    auto it = graph.end();
    int dataPoints = std::min({static_cast<int>(graph.size()), static_cast<int>(size.x), 75});

    //convert data into a float list
    for (int i = 0; i < dataPoints; ++i) {
        --it;
        values.insert(values.begin(), isSwap ? it->swap : it->mem);
        if (it == graph.begin()) break;
    }

    sf::Color line = (m_memory.isEnabled()) ? sf::Color::Green : sf::Color(64, 64, 64);
    sf::Color fill = (m_memory.isEnabled()) ? sf::Color(0, 255, 0, 150) : sf::Color(128, 128, 128);
    drawGraph(window, values, size, position, line, fill);
}


///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::printMemory(sf::RenderWindow &window)
{
    float margine = static_cast<float>(window.getSize().y * 0.01f);

    sf::Vector2f pos = {_sidebarSize.x, _titlecardSize.y};
    pos.y += margine * 4;

    sf::Vector2f graphSize = {
        (static_cast<float>(window.getSize().x) - _sidebarSize.x) * 0.8f,
        static_cast<float>(window.getSize().y * 0.25f)};
    sf::Vector2f graphPos = {_sidebarSize.x
            + static_cast<float>(window.getSize().x * 0.05f),
        pos.y};

    drawgrid(window, graphSize, graphPos, sf::Color::White, 2.f,
            sf::Color::Magenta, {0.1f, 0.15f});
    drawMemoryGraph(window, m_memory.getGraph(), {graphSize.x + 2.f, graphSize.y}, {graphPos.x + 2.f, graphPos.y}, false);
    pos.y += graphSize.y;
    pos.y += textPrepper(window, "\t\t\tMemory " + formatFloat(m_memory.getMemoryUsed()) + "%", pos).y;
    pos.y += margine;
    drawgrid(window, graphSize, {graphPos.x, pos.y}, sf::Color::White, 2.f,
            sf::Color::Magenta, {0.1f, 0.15f});
    drawMemoryGraph(window, m_memory.getGraph(), {graphSize.x + 2.f, graphSize.y - (graphSize.y * 0.001f)}, {graphPos.x + 2.f, pos.y}, true);
    pos.y += graphSize.y;
    pos.y += textPrepper(window, "\t\t\tMemory " + formatFloat(m_memory.getSwapUsed()) + "%", pos).y;
    pos.y += margine;

    sf::Vector2f memPos = pos;
    sf::Vector2f memSize = pos;

    memSize += textPrepper(window, "Memory Usage", memPos);
    memPos.y += margine *4;
    textPrepper(window, "Total: " + formatFloat(m_memory.getMemoryTotal() / 1024.f / 1024.f) + "GB", memPos);
    memPos.y += margine *3;
    textPrepper(window, "Used: " + formatFloat((m_memory.getMemoryTotal() * m_memory.getMemoryUsed() / 100.f) / 1024.f / 1024.f) + "GB" + " (" + formatFloat(m_memory.getMemoryUsed()) + "%)", memPos);
    memPos.y += margine *3;
    textPrepper(window, "Free: " + formatFloat(m_memory.getMemoryFree() / 1024.f / 1024.f) + "GB", memPos);


    sf::Vector2f swaPos = {memSize.x, pos.y};
    textPrepper(window, "Swap Usage", swaPos).y;
    swaPos.y += margine * 4;
    textPrepper(window, "Total: " + formatFloat(m_memory.getSwapTotal() / 1024.f / 1024.f) + "GB", swaPos);
        swaPos.y += margine * 3;
    textPrepper(window, "Used: " + formatFloat((m_memory.getSwapTotal() * m_memory.getSwapUsed() / 100.f) / 1024.f / 1024.f) + "GB" + " (" + formatFloat(m_memory.getSwapUsed()) + "%)", swaPos);
    swaPos.y += margine * 3;
    textPrepper(window, "Free: " + formatFloat(m_memory.getSwapTotal() / 1024.f / 1024.f) + "GB", swaPos);
}


///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::drawNetworkGraph(sf::RenderWindow &window, const std::list<NetworkModule::Data> &graph,
    const sf::Vector2f &size, const sf::Vector2f &position, bool isUpload)
{
    if (graph.empty()) return;

    std::vector<float> values;
    auto it = graph.end();
    int dataPoints = std::min({static_cast<int>(graph.size()), static_cast<int>(size.x), 75});

    // Find the max value to scale the graph correctly
    float maxValue = 0;
    for (const auto &data : graph) {
        maxValue = std::max(maxValue, isUpload ? data.up : data.down);
    }
    maxValue = std::ceil(maxValue / 10.0f) * 10.0f; // Round up to the nearest multiple of 10

    // Convert data into a float list, scaled to percentage
    for (int i = 0; i < dataPoints; ++i) {
        --it;
        float value = isUpload ? it->up : it->down;
        values.insert(values.begin(), (maxValue > 0) ? (value / maxValue * 100.f) : 0.f); // Scale to 0-100%
        if (it == graph.begin()) break;
    }

    sf::Color line = (m_network.isEnabled()) ? sf::Color::Yellow : sf::Color(64, 64, 64);
    sf::Color fill = (m_network.isEnabled()) ? sf::Color(255, 0, 0, 150) : sf::Color(128, 128, 128);

    drawGraph(window, values, size, position, line, fill);
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::printNetwork(sf::RenderWindow &window)
{
    float margine = static_cast<float>(window.getSize().y * 0.01f);

    sf::Vector2f pos = {_sidebarSize.x, _titlecardSize.y};
    pos.y += margine * 4;
    sf::Vector2f graphSize = {
        (static_cast<float>(window.getSize().x) - _sidebarSize.x) * 0.8f,
        static_cast<float>(window.getSize().y * 0.25f)};
    sf::Vector2f graphPos = {_sidebarSize.x
            + static_cast<float>(window.getSize().x * 0.05f),
        pos.y};

    drawgrid(window, graphSize, graphPos, sf::Color::White, 2.f,
            sf::Color::Yellow, {0.1f, 0.15f});
    drawNetworkGraph(window, m_network.getGraph(), {graphSize.x + 2.f, graphSize.y}, {graphPos.x + 2.f, graphPos.y}, false);
    pos.y += graphSize.y;
    pos.y += textPrepper(window, "\t\t\tUpload: " + formatFloat(m_network.getUp()) + " Kb/s", pos).y;
    pos.y += margine;
    drawgrid(window, graphSize, {graphPos.x, pos.y}, sf::Color::White, 2.f,
            sf::Color::Yellow, {0.1f, 0.15f});
    drawNetworkGraph(window, m_network.getGraph(), {graphSize.x + 2.f, graphSize.y}, {graphPos.x + 2.f, pos.y}, true);
    pos.y += graphSize.y;
    pos.y += textPrepper(window, "\t\t\tDownload: " + formatFloat(m_network.getDown()) + " Kb/s", pos).y;
    pos.y += margine * 2;

    textPrepper(window, "Network Usage", pos);
    pos.y += margine * 4;
    textPrepper(window, std::string("Upload: ") + formatFloat(m_network.getUp()) + " Kb/s", pos);
    pos.y += margine * 3;
    textPrepper(window, std::string("Download: ") + formatFloat(m_network.getDown()) + " Kb/s", pos);
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::printDisk(sf::RenderWindow &window)
{
    sf::Vector2f margine = {static_cast<float>(window.getSize().x * 0.01f), static_cast<float>(window.getSize().y * 0.01f)};

    sf::Vector2f pos = {_sidebarSize.x, _titlecardSize.y};
    pos.y += margine.y * 4;

    std::vector<DiskModule::Data> data = m_disk.getDisks();
    sf::Vector2f diskSize = {
        (static_cast<float>(window.getSize().x) - _sidebarSize.x) * 0.43f,
        static_cast<float>(window.getSize().y * 0.05f)};

    sf::Vector2f diskPos = {_sidebarSize.x
        + static_cast<float>(window.getSize().x * 0.05f),
        pos.y};


    for (size_t i = 0; i < data.size(); i++){
        sf::Vector2f temp = diskPos;
        sf::Vector2f diskUsage = {diskSize.x * (data[i].usage /100.f) , diskSize.y};
        sf::Color percentage;

        if (data[i].usage <= 50)
            percentage = sf::Color::Green;
        else if (data[i].usage <= 75)
            percentage = sf::Color(255, 121, 0);
        else
            percentage = sf::Color::Red;

        if (i % 2 == 1){
            temp.x += diskSize.x;
            temp.x += margine.x * 3;
        }
        temp.y += textPrepper(window, data[i].mountpoint.c_str(), temp).y;
        createBackground(window, diskSize, temp, sf::Color::White, 2.f, sf::Color::Black);
        createBackground(window, diskUsage, temp, percentage);
        if (m_disk.isEnabled())
            textPrepper(window, formatFloat(data[i].usage) + " / " + formatFloat(data[i].total / 1024.f / 1024.f)+ "Gb", {temp.x + margine.x * 20, temp.y + margine.y});
        if (i % 2 == 1){
            diskPos.y += diskSize.y;
            diskPos.y += margine.y * 4;
        }
    }

}


void GraphicalMonitor::printProcessus(sf::RenderWindow &window)
{
    sf::Vector2f margine = {static_cast<float>(window.getSize().x * 0.01f), static_cast<float>(window.getSize().y * 0.01f)};

    sf::Vector2f pos = {_sidebarSize.x, _titlecardSize.y};
    pos.y += margine.y * 4;
    static const char *header[] {
        "PID", "USER", "CPU%", "MEM%", "RES", "STATE", "TIME", "COMMAND"};

    sf::Vector2f titleBoxSize = {(static_cast<float>(window.getSize().x) - _sidebarSize.x) * 0.1f, static_cast<float>(window.getSize().y * 0.05f)};

    sf::Vector2f verticalGridline = {1.f, static_cast<float>(window.getSize().y)};

    sf::Vector2f horizontalGridline = {(static_cast<float>(window.getSize().x) - (_sidebarSize.x + margine.x * 2)), 2.f};

    for (size_t i = 0; i < sizeof(header) / sizeof(header[0]); i++) {

        textbox(window, titleBoxSize, pos, sf::Color::Transparent, header[i], 1.f, sf::Color::Transparent);

        pos.x += titleBoxSize.x;
        if (i != sizeof(header) / sizeof(header[0]) - 1)
            createBackground(window,verticalGridline, pos, sf::Color(164, 164, 164));
        pos.x += margine.x;
    }
    pos.x = _sidebarSize.x;
    pos.y += margine.y * 4;
    createBackground(window, horizontalGridline, pos, sf::Color(164, 164, 164));

    std::vector<ProcessModule::Data> proccs = m_process.getProcesses();

    for (const auto& proc : proccs) {
        sf::Vector2f tempPos = pos;
        textbox(window, titleBoxSize, tempPos, sf::Color::Transparent, std::to_string(proc.pid));
        tempPos.x += titleBoxSize.x;
        tempPos.x += margine.x;
        textbox(window, titleBoxSize, tempPos, sf::Color::Transparent, proc.user.c_str());
        tempPos.x += titleBoxSize.x;
        tempPos.x += margine.x;
        textbox(window, titleBoxSize, tempPos, sf::Color::Transparent, formatFloat(proc.cpu));
        tempPos.x += titleBoxSize.x;
        tempPos.x += margine.x;
        textbox(window, titleBoxSize, tempPos, sf::Color::Transparent, formatFloat(proc.mem));
        tempPos.x += titleBoxSize.x;
        tempPos.x += margine.x;
        textbox(window, titleBoxSize, tempPos, sf::Color::Transparent, std::string(proc.res.c_str()));
        tempPos.x += titleBoxSize.x;
        tempPos.x += margine.x;
        textbox(window, titleBoxSize, tempPos, sf::Color::Transparent, std::to_string(proc.state));
        tempPos.x += titleBoxSize.x;
        tempPos.x += margine.x;
        textbox(window, titleBoxSize, tempPos, sf::Color::Transparent, std::string(proc.time.c_str()));
        tempPos.x += titleBoxSize.x;
        tempPos.x += margine.x;
        textbox(window, titleBoxSize, tempPos, sf::Color::Transparent, std::string(proc.command.c_str()));
        pos.y += margine.y * 3;
    }

}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::printCredit(sf::RenderWindow &window)
{
    sf::Vector2f margine = {static_cast<float>(window.getSize().x * 0.01f), static_cast<float>(window.getSize().y * 0.01f)};

    sf::Vector2f pos = {_sidebarSize.x, _titlecardSize.y};
    pos.y += margine.y * 4;

    textPrepper(window, "Nathan FIEVET", pos);
    textPrepper(window, "(natan-fievet)", {pos.x + (margine.x * 15.f), pos.y});
    pos.y += margine.y * 5;

    textPrepper(window, "Mallory SCOTTON", pos);
    textPrepper(window, "(mallory-scotton)", {pos.x + (margine.x * 15.f), pos.y});
    pos.y += margine.y * 5;

    textPrepper(window, "Ossan MSOILI", pos);
    textPrepper(window, "(Omegalpha28)", {pos.x + (margine.x * 15.f), pos.y});
    pos.y += margine.y * 5;

    textPrepper(window, "Hugo CATHELAIN", pos);
    textPrepper(window, "(Hugo-Cathelain)", {pos.x + (margine.x * 15.f), pos.y});
}

///////////////////////////////////////////////////////////////////////////////
void GraphicalMonitor::handlekeys(sf::Event event, sf::RenderWindow &window)
{
    if (event.key.code == sf::Keyboard::Up && m_selected > 0)
        m_selected--;
    if (event.key.code == sf::Keyboard::Down && m_selected < 6)
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
    if (event.key.code == sf::Keyboard::D)
        m_disk.setEnabled(!m_disk.isEnabled());
    if (event.key.code == sf::Keyboard::E)
        m_process.setEnabled(!m_process.isEnabled());
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
void GraphicalMonitor::handlePrint(sf::RenderWindow &window)
{
    PrintNavBar(window);

    if (m_selected == 0)
        printInfo(window);
    if (m_selected == 1)
        printProcessor(window);
    if (m_selected == 2)
        printMemory(window);
    if (m_selected == 3)
        printProcessus(window);
    if (m_selected == 4)
        printNetwork(window);
    if (m_selected == 5)
        printDisk(window);
    if (m_selected == 6)
        printCredit(window);

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


        handlePrint(window);

        window.display();
    }
    return (0);
}

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
#include "TextMonitor.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

///////////////////////////////////////////////////////////////////////////////
TextMonitor::TextMonitor(void)
    : IMonitorDisplay()
    , m_selected(0)
{}

///////////////////////////////////////////////////////////////////////////////
TextMonitor::~TextMonitor()
{}

///////////////////////////////////////////////////////////////////////////////
bool TextMonitor::getInput(void)
{
    int c = getch();

    if (c == (int)'u') m_user.setEnabled(!m_user.isEnabled());
    if (c == (int)'m') m_memory.setEnabled(!m_memory.isEnabled());
    if (c == (int)'o') m_os.setEnabled(!m_os.isEnabled());
    if (c == (int)'p') m_cpu.setEnabled(!m_cpu.isEnabled());
    if (c == (int)'n') m_network.setEnabled(!m_network.isEnabled());
    if (c == (int)'t') m_time.setEnabled(!m_time.isEnabled());
    if (c == (int)'d') m_disk.setEnabled(!m_disk.isEnabled());
    if (c == (int)'q') return (false);
    if (c == KEY_UP && m_selected > 0) { m_selected--; clear(); }
    if (c == KEY_DOWN && m_selected < 7) { m_selected++; clear(); }

    return (true);
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printNavBar(WINDOW* Wcontent) const
{
    static const char* menuItems[] = {
        "Information   ", "Processor     ",
        "Memory        ", "Services      ",
        "Network       ", "Disks         ",
        "Battery       ", "Credits       ",
    };
    static const char* contentItems[] = {
        "Information", "[p] Processor", "[m] Memory", "Services", "[n] Network", "[d] Disks", "Battery", "Credits"
    };
    WINDOW *Wnavbar = subwin(stdscr, m_height - 2, 19, 1, 2);

    box(Wnavbar, ACS_VLINE, ACS_HLINE);
    mvwprintw(Wnavbar, 0, 4, " Navigation ");

    mvwprintw(Wnavbar, m_height - 6, 3, "/\\ Move up");
    mvwprintw(Wnavbar, m_height - 5, 3, "\\/ Move down");

    for (size_t i = 0; i < sizeof(menuItems) / sizeof(menuItems[0]); i++) {
        if (i == m_selected) {
            wattron(Wnavbar, A_REVERSE);
            mvwprintw(Wnavbar, 2 + i * 2, 3, "%s", menuItems[i]);
            wattroff(Wnavbar, A_REVERSE);
        } else {
            mvwprintw(Wnavbar, 2 + i * 2, 3, "%s", menuItems[i]);
        }
    }
    wrefresh(Wnavbar);
    mvwprintw(Wcontent, 0, 4, " %s ", contentItems[m_selected]);
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printInformation(WINDOW* Wcontent) const
{
    (void)Wcontent;
    {
        WINDOW* Wtime = subwin(stdscr, 5, m_width - 25, 3, 23);

        if (!m_time.isEnabled()) wattron(Wtime, WA_DIM);
        box(Wtime, ACS_VLINE, ACS_HLINE);
        mvwprintw(Wtime, 0, 2, " [t] Time Module ");
        mvwprintw(Wtime, 2, 3, "Current Date: %s", m_time.getTime().c_str());
        if (!m_time.isEnabled()) wattroff(Wtime, WA_DIM);
    }

    {
        WINDOW* Wuser = subwin(stdscr, 7, m_width - 25, 9, 23);

        if (!m_user.isEnabled()) wattron(Wuser, WA_DIM);
        box(Wuser, ACS_VLINE, ACS_HLINE);
        mvwprintw(Wuser, 0, 2, " [u] User Module ");
        mvwprintw(Wuser, 2, 3, "User Name: %s", m_user.getUserName().c_str());
        mvwprintw(Wuser, 4, 3, "Host Name: %s", m_user.getHostName().c_str());
        if (!m_user.isEnabled()) wattroff(Wuser, WA_DIM);
    }

    {
        WINDOW* Wos = subwin(stdscr, 7, m_width - 25, 17, 23);

        if (!m_os.isEnabled()) wattron(Wos, WA_DIM);
        box(Wos, ACS_VLINE, ACS_HLINE);
        mvwprintw(Wos, 0, 2, " [o] Operating System Module ");
        mvwprintw(Wos, 2, 3, "Os Name: %s", m_os.getOs().c_str());
        mvwprintw(Wos, 4, 3, "Kernel Name: %s", m_os.getKernel().c_str());
        if (!m_os.isEnabled()) wattroff(Wos, WA_DIM);
    }
}

///////////////////////////////////////////////////////////////////////////////
static void drawCPUGraph(WINDOW* Wgraph, const std::list<double>& graph, int width, int height, double current)
{
    if (graph.empty()) {
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                mvwaddch(Wgraph, y, x, ' ');
            }
        }
        return;
    }

    const int graphHeight = height - 2;
    const int graphWidth = width - 2;

    std::vector<std::vector<char>> buffer(graphHeight, std::vector<char>(graphWidth, ' '));

    int dataPoints = std::min(static_cast<int>(graph.size()), graphWidth);

    std::vector<double> values;
    auto it = graph.end();
    for (int i = 0; i < dataPoints; ++i) {
        --it;
        values.insert(values.begin(), *it);
        if (it == graph.begin()) break;
    }

    for (int x = 0; x < (int)values.size(); x++) {
        double value = values[x];
        int barHeight = static_cast<int>((value / 100.0) * (graphHeight - 1));
        barHeight = std::min(barHeight, graphHeight - 1);
        
        for (int y = 0; y < barHeight; y++) {
            int invertedY = graphHeight - 1 - y;
            
            if (y == barHeight - 1) buffer[invertedY][x] = '^';
            else if (value > 75)    buffer[invertedY][x] = '#';
            else if (value > 50)    buffer[invertedY][x] = '=';
            else if (value > 25)    buffer[invertedY][x] = '-';
            else                    buffer[invertedY][x] = '.';
        }
    }

    mvwprintw(Wgraph, 1, 1, "100%%");
    mvwprintw(Wgraph, height/2, 1, "50%%");
    mvwprintw(Wgraph, height-2, 1, "0%%");

    for (int y = 0; y < graphHeight; y++) {
        wmove(Wgraph, y + 1, 1);
        for (int x = 0; x < graphWidth; x++) {
            mvwaddch(Wgraph, y + 1, x + 1, buffer[y][x]);
        }
    }

    mvwprintw(Wgraph, height-1, 1, " %.2lf%% ", current);
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printProcessor(WINDOW* Wcontent) const
{
    if (!m_cpu.isEnabled()) wattron(Wcontent, WA_DIM);
    {
        int width = m_width - 25, height = 22;
        WINDOW* Wgraph = subwin(stdscr, height, width, 3, 23);
        std::list<double> graph = m_cpu.getGraph();

        wclear(Wgraph);
        if (!m_cpu.isEnabled()) wattron(Wgraph, WA_DIM);
        box(Wgraph, ACS_VLINE, ACS_HLINE);
        mvwprintw(Wgraph, 0, 2, " Usage ");
        drawCPUGraph(Wgraph, graph, width, height, m_cpu.getUsage());
        if (!m_cpu.isEnabled()) wattroff(Wgraph, WA_DIM);
    }
    mvwprintw(Wcontent, 25, 3, "Vendor: %s", m_cpu.getProcs()[0]["vendorId"].c_str());
    mvwprintw(Wcontent, 27, 3, "CPU Model: %s", m_cpu.getProcs()[0]["modelName"].c_str());
    mvwprintw(Wcontent, 29, 3, "CPU Cores: %s", m_cpu.getProcs()[0]["cpuCores"].c_str());
    mvwprintw(Wcontent, 31, 3, "Cache Size: %s", m_cpu.getProcs()[0]["cacheSize"].c_str());
    mvwprintw(Wcontent, 33, 3, "Physical Cores: %ld", m_cpu.getProcs().size());
    mvwprintw(Wcontent, 35, 3, "Address Sizes: %s", m_cpu.getProcs()[0]["addressSizes"].c_str());
    if (!m_cpu.isEnabled()) wattroff(Wcontent, WA_DIM);
}

///////////////////////////////////////////////////////////////////////////////
void drawMemoryGraph(WINDOW* Wgraph, const std::list<MemoryModule::Data>& graph, int width, int height, bool isSwap, float current)
{
    if (graph.empty()) {
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                mvwaddch(Wgraph, y, x, ' ');
            }
        }
        return;
    }

    const int graphHeight = height - 2;
    const int graphWidth = width - 2;

    std::vector<std::vector<char>> buffer(graphHeight, std::vector<char>(graphWidth, ' '));
    int dataPoints = std::min(static_cast<int>(graph.size()), graphWidth);

    std::vector<float> values;
    auto it = graph.end();
    for (int i = 0; i < dataPoints; ++i) {
        --it;
        values.insert(values.begin(), isSwap ? it->swap : it->mem);
        if (it == graph.begin()) break;
    }

    for (int x = 0; x < (int)values.size(); x++) {
        float value = values[x];
        int barHeight = static_cast<int>((value / 100.0) * (graphHeight - 1));
        barHeight = std::min(barHeight, graphHeight - 1);
        
        for (int y = 0; y < barHeight; y++) {
            int invertedY = graphHeight - 1 - y;
            
            if (y == barHeight - 1) {
                buffer[invertedY][x] = '^';
            } else if (value > 75) {
                buffer[invertedY][x] = '#';
            } else if (value > 50) {
                buffer[invertedY][x] = '=';
            } else if (value > 25) {
                buffer[invertedY][x] = '-';
            } else {
                buffer[invertedY][x] = '.';
            }
        }
    }

    mvwprintw(Wgraph, 1, 1, "100%%");
    mvwprintw(Wgraph, height/2, 1, "50%%");
    mvwprintw(Wgraph, height-2, 1, "0%%");

    for (int y = 0; y < graphHeight; y++) {
        for (int x = 0; x < graphWidth; x++) {
            mvwaddch(Wgraph, y + 1, x + 1, buffer[y][x]);
        }
    }

    mvwprintw(Wgraph, height-1, 1, " %.2f%% ", current);
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printMemory(WINDOW* Wcontent) const
{
    if (!m_memory.isEnabled()) wattron(Wcontent, WA_DIM);
    
    mvwprintw(Wcontent, 24, 2, "Memory Usage");
    mvwprintw(Wcontent, 26, 2, "Total: %.2f GB", m_memory.getMemoryTotal() / 1024.f / 1024.f);
    mvwprintw(Wcontent, 27, 2, "Used:  %.2f GB (%.1f%%)", 
        (m_memory.getMemoryTotal() * m_memory.getMemoryUsed() / 100.f) / 1024.f / 1024.f,
        m_memory.getMemoryUsed());
    mvwprintw(Wcontent, 28, 2, "Free:  %.2f GB", m_memory.getMemoryFree() / 1024.f / 1024.f);
    
    mvwprintw(Wcontent, 24, 32, "Swap Usage");
    mvwprintw(Wcontent, 26, 32, "Total: %.2f GB", m_memory.getSwapTotal() / 1024.f / 1024.f);
    mvwprintw(Wcontent, 27, 32, "Used:  %.2f GB (%.1f%%)",
        (m_memory.getSwapTotal() * m_memory.getSwapUsed() / 100.f) / 1024.f / 1024.f,
        m_memory.getSwapUsed());
    mvwprintw(Wcontent, 28, 32, "Free:  %.2f GB", m_memory.getSwapFree() / 1024.f / 1024.f);

    int width = m_width - 25;
    int graphHeight = 10;
    
    WINDOW* WmemGraph = subwin(stdscr, graphHeight, width, 3, 23);
    std::list<MemoryModule::Data> graph = m_memory.getGraph();

    wclear(WmemGraph);
    if (!m_memory.isEnabled()) wattron(WmemGraph, WA_DIM);
    box(WmemGraph, ACS_VLINE, ACS_HLINE);
    mvwprintw(WmemGraph, 0, 2, " Memory Usage ");
    drawMemoryGraph(WmemGraph, graph, width, graphHeight, false, m_memory.getMemoryUsed());
    if (!m_memory.isEnabled()) wattroff(WmemGraph, WA_DIM);

    WINDOW* WswapGraph = subwin(stdscr, graphHeight, width, graphHeight + 4, 23);
    wclear(WswapGraph);
    if (!m_memory.isEnabled()) wattron(WswapGraph, WA_DIM);
    box(WswapGraph, ACS_VLINE, ACS_HLINE);
    mvwprintw(WswapGraph, 0, 2, " Swap Usage ");
    drawMemoryGraph(WswapGraph, graph, width, graphHeight, true, m_memory.getSwapUsed());
    if (!m_memory.isEnabled()) wattroff(WswapGraph, WA_DIM);

    if (!m_memory.isEnabled()) wattroff(Wcontent, WA_DIM);
}

///////////////////////////////////////////////////////////////////////////////
void drawNetworkGraph(WINDOW* Wgraph, const std::list<NetworkModule::Data>& graph, int width, int height, bool isUpload, float current)
{
    if (graph.empty()) {
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                mvwaddch(Wgraph, y, x, ' ');
            }
        }
        return;
    }

    const int graphHeight = height - 2;
    const int graphWidth = width - 2;

    float maxValue = 0;
    for (const auto& data : graph) {
        maxValue = std::max(maxValue, isUpload ? data.up : data.down);
    }
    maxValue = std::ceil(maxValue / 10.0) * 10.0;

    std::vector<std::vector<char>> buffer(graphHeight, std::vector<char>(graphWidth, ' '));
    int dataPoints = std::min(static_cast<int>(graph.size()), graphWidth);

    std::vector<float> values;
    auto it = graph.end();
    for (int i = 0; i < dataPoints; ++i) {
        --it;
        values.insert(values.begin(), isUpload ? it->up : it->down);
        if (it == graph.begin()) break;
    }

    for (int x = 0; x < (int)values.size(); x++) {
        float value = values[x];
        int barHeight = static_cast<int>((value / maxValue) * (graphHeight - 1));
        barHeight = std::min(barHeight, graphHeight - 1);
        
        for (int y = 0; y < barHeight; y++) {
            int invertedY = graphHeight - 1 - y;
            
            if (y == barHeight - 1) {
                buffer[invertedY][x] = '^';
            } else if (value > (maxValue * 0.75)) {
                buffer[invertedY][x] = '#';
            } else if (value > (maxValue * 0.5)) {
                buffer[invertedY][x] = '=';
            } else if (value > (maxValue * 0.25)) {
                buffer[invertedY][x] = '-';
            } else {
                buffer[invertedY][x] = '.';
            }
        }
    }

    mvwprintw(Wgraph, 1, 1, "%.0f", maxValue);
    mvwprintw(Wgraph, height/2, 1, "%.0f", maxValue/2);
    mvwprintw(Wgraph, height-2, 1, "0");

    for (int y = 0; y < graphHeight; y++) {
        for (int x = 0; x < graphWidth; x++) {
            mvwaddch(Wgraph, y + 1, x + 1, buffer[y][x]);
        }
    }

    mvwprintw(Wgraph, height-1, 1, " %.2f KB/s ", current);
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printNetwork(WINDOW* Wcontent) const
{
    if (!m_network.isEnabled()) wattron(Wcontent, WA_DIM);

    mvwprintw(Wcontent, 24, 2, "Network Usage");
    mvwprintw(Wcontent, 26, 2, "Upload:   %.2f KB/s", m_network.getUp());
    mvwprintw(Wcontent, 27, 2, "Download: %.2f KB/s", m_network.getDown());

    int width = m_width - 25;
    int graphHeight = 10;
    
    WINDOW* WupGraph = subwin(stdscr, graphHeight, width, 3, 23);
    std::list<NetworkModule::Data> graph = m_network.getGraph();

    wclear(WupGraph);
    if (!m_network.isEnabled()) wattron(WupGraph, WA_DIM);
    box(WupGraph, ACS_VLINE, ACS_HLINE);
    mvwprintw(WupGraph, 0, 2, " Upload (KB/s) ");
    drawNetworkGraph(WupGraph, graph, width, graphHeight, true, m_network.getUp());
    if (!m_network.isEnabled()) wattroff(WupGraph, WA_DIM);

    WINDOW* WdownGraph = subwin(stdscr, graphHeight, width, graphHeight + 4, 23);
    wclear(WdownGraph);
    if (!m_network.isEnabled()) wattron(WdownGraph, WA_DIM);
    box(WdownGraph, ACS_VLINE, ACS_HLINE);
    mvwprintw(WdownGraph, 0, 2, " Download (KB/s) ");
    drawNetworkGraph(WdownGraph, graph, width, graphHeight, false, m_network.getDown());
    if (!m_network.isEnabled()) wattroff(WdownGraph, WA_DIM);

    if (!m_network.isEnabled()) wattroff(Wcontent, WA_DIM);
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printDisks(WINDOW* Wcontent) const
{
    (void)Wcontent;
    std::vector<DiskModule::Data> data = m_disk.getDisks();
    int width = (m_width - 25) / 2 - 1;

    for (size_t i = 0; i < data.size(); i++) {
        WINDOW* Wdisk = subwin(stdscr, 3, width, 3 + (int)(i / 2) * 3, 23 + (i % 2 ? width + 1 : 0));

        if (!m_disk.isEnabled()) wattron(Wdisk, WA_DIM);
        box(Wdisk, ACS_VLINE, ACS_HLINE);
        mvwprintw(Wdisk, 0, 2, " %s ", data[i].mountpoint.c_str());

        int color_pair;
        if (data[i].usage <= 50)      color_pair = 1;
        else if (data[i].usage <= 75) color_pair = 2;
        else                          color_pair = 3;

        int endpoint = (int)((data[i].usage / 100.f) * ((float)width - 20.f));
        for (int i = 0; i < width - 20; i++) {
            if (i < endpoint) {
                wattron(Wdisk, COLOR_PAIR(color_pair));
                mvwaddch(Wdisk, 1, 1 + i, ACS_CKBOARD);
                wattroff(Wdisk, COLOR_PAIR(color_pair));
            } else mvwaddch(Wdisk, 1, 1 + i, ACS_BULLET);
        }

        mvwprintw(Wdisk, 1, width - 19, " %.f%% %.2f GB ", data[i].usage, data[i].total / 1024.f / 1024.f);
        if (!m_disk.isEnabled()) wattroff(Wdisk, WA_DIM);
    }
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printBattery(WINDOW *Wcontent) const
{
    (void)Wcontent;
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printServices(WINDOW *Wcontent) const
{
    (void)Wcontent;
    std::vector<ServiceModule::Data> services = m_service.getServices();
    int numCols = 3;
    int lenght = 0;
    int colWidth = 50;
    int i = 0;

    for (const auto& service : services) {
        int row = i / numCols;
        int col = ((i % numCols) * colWidth) + lenght;

        mvwprintw(Wcontent, row + 2, col + 2, " %s | %s ", service.name.c_str(), service.status.c_str());
        lenght += (strlen(service.name.c_str()) + strlen(service.status.c_str()) + 6) / 2;
        i += 1;
        if (i % numCols == 0)
            lenght = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printCredits(WINDOW* Wcontent) const
{
    mvwprintw(Wcontent, 2, 3, "Made by:");
    wattron(Wcontent, WA_DIM);  mvwprintw(Wcontent, 4, 5, "Mallory SCOTTON \t(mallory-scotton)"); wattroff(Wcontent, WA_DIM);
    wattron(Wcontent, WA_BOLD); mvwprintw(Wcontent, 4, 5, "Mallory SCOTTON"); wattroff(Wcontent, WA_BOLD);

    wattron(Wcontent, WA_DIM);  mvwprintw(Wcontent, 6, 5, "Hugo CATHELAIN \t(Hugo-Cathelain)"); wattroff(Wcontent, WA_DIM);
    wattron(Wcontent, WA_BOLD); mvwprintw(Wcontent, 6, 5, "Hugo CATHELAIN"); wattroff(Wcontent, WA_BOLD);

    wattron(Wcontent, WA_DIM);  mvwprintw(Wcontent, 8, 5, "Nathan FIEVET \t(natan-fievet)"); wattroff(Wcontent, WA_DIM);
    wattron(Wcontent, WA_BOLD); mvwprintw(Wcontent, 8, 5, "Nathan FIEVET"); wattroff(Wcontent, WA_BOLD);

    wattron(Wcontent, WA_DIM);  mvwprintw(Wcontent, 10, 5, "Ossan MSOILI \t(Omegalpha28)"); wattroff(Wcontent, WA_DIM);
    wattron(Wcontent, WA_BOLD); mvwprintw(Wcontent, 10, 5, "Ossan MSOILI"); wattroff(Wcontent, WA_BOLD);
}

///////////////////////////////////////////////////////////////////////////////
static void initNcurses(void)
{
    initscr();
    noecho();
    cbreak();
    timeout(400);
    curs_set(0);
    start_color();
    keypad(stdscr, 1);
    use_default_colors();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
}

///////////////////////////////////////////////////////////////////////////////
int TextMonitor::loop(void)
{
    int oldW = 0, oldH = 0;

    initNcurses();
    while (getInput()) {
        getmaxyx(stdscr, m_height, m_width);
        if (oldW != m_width || oldH != m_height) {
            clear();
            oldW = m_width;
            oldH = m_height;
        }

        WINDOW* Wcontent = subwin(stdscr, m_height - 2, m_width - 21, 1, 21);
        box(Wcontent, ACS_VLINE, ACS_HLINE);

        refresh();
        printNavBar(Wcontent);

        if (m_selected == 0) printInformation(Wcontent);
        if (m_selected == 1) printProcessor(Wcontent);
        if (m_selected == 2) printMemory(Wcontent);
        if (m_selected == 3) printServices(Wcontent);
        if (m_selected == 4) printNetwork(Wcontent);
        if (m_selected == 5) printDisks(Wcontent);
        if (m_selected == 6) printBattery(Wcontent);
        if (m_selected == 7) printCredits(Wcontent);

        wrefresh(Wcontent);
    }
    curs_set(1);
    nocbreak();
    endwin();
    return (0);
}

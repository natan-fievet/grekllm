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
    if (c == (int)'q') return (false);
    if (c == KEY_UP && m_selected > 0) { m_selected--; clear(); }
    if (c == KEY_DOWN && m_selected < 4) { m_selected++; clear(); }

    return (true);
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printNavBar(WINDOW* Wcontent) const
{
    static const char* menuItems[] = {
        "Information   ", "Processor     ",
        "Memory        ", "Network       ",
        "Credits       "
    };
    static const char* contentItems[] = {
        "Information", "Processor", "Memory", "Network", "Credits"
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
void TextMonitor::printProcessor(WINDOW* Wcontent) const
{
    (void)Wcontent;
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printMemory(WINDOW* Wcontent) const
{
    (void)Wcontent;
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printNetwork(WINDOW* Wcontent) const
{
    (void)Wcontent;
}

///////////////////////////////////////////////////////////////////////////////
void TextMonitor::printCredits(WINDOW* Wcontent) const
{
    (void)Wcontent;
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
    init_pair(10, COLOR_RED, -1);
    init_pair(11, COLOR_GREEN, -1);
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
        if (m_selected == 3) printNetwork(Wcontent);
        if (m_selected == 4) printCredits(Wcontent);

        wrefresh(Wcontent);
    }
    curs_set(1);
    nocbreak();
    endwin();
    return (0);
}

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
#include "modules/NetworkModule.hpp"
#include <string>
#include <fstream>

///////////////////////////////////////////////////////////////////////////////
NetworkModule::NetworkModule(void)
    : m_enabled(true)
{}

///////////////////////////////////////////////////////////////////////////////
NetworkModule::~NetworkModule()
{
    m_enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
void NetworkModule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

///////////////////////////////////////////////////////////////////////////////
bool NetworkModule::isEnabled(void) const
{
    return (m_enabled);
}

///////////////////////////////////////////////////////////////////////////////
bool NetworkModule::refresh(void)
{
    int i = 0, a = 0;
    std::string tmp, line;
    std::ifstream file("/proc/net/dev", std::ios::in);

    if (!m_enabled)
        return (false);
    while (std::getline(file, line)) {
        if ((a = line.find(":")) != -1) {
            a++;
            for (; line[a] == ' '; a++);
            for (; line[a] != ' '; a++)
                tmp += line[a];
            if (std::stoi(tmp) > 0) {
                m_up = std::stoi(tmp);
                tmp.clear();
                for (; i != 7; i++) {
                    for (; line[a] == ' '; a++);
                    for (; line[a] != ' '; a++);
                }
                i = 0;
                for (; line[a] == ' '; a++);
                for (; line[a] != ' '; a++)
                    tmp += line[a];
                m_down = std::stoi(tmp);
            }
        }
        tmp.clear();
    }
    file.close();
    if (m_graph.size() == 200)
        m_graph.pop_back();
    m_graph.push_front({m_up, m_down});
    return (true);
}

///////////////////////////////////////////////////////////////////////////////
void NetworkModule::setUp(int up)
{
    m_up = up;
}

///////////////////////////////////////////////////////////////////////////////
void NetworkModule::setDown(int down)
{
    m_down = down;
}

///////////////////////////////////////////////////////////////////////////////
int NetworkModule::getUp(void) const
{
    return (m_up);
}

///////////////////////////////////////////////////////////////////////////////
int NetworkModule::getDown(void) const
{
    return (m_down);
}

///////////////////////////////////////////////////////////////////////////////
std::list<NetworkModule::Data> NetworkModule::getGraph(void) const
{
    return (m_graph);
}

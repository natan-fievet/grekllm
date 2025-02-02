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
#include "modules/MemoryModule.hpp"
#include <fstream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////
MemoryModule::MemoryModule(void)
    : m_enabled(true)
{}

///////////////////////////////////////////////////////////////////////////////
MemoryModule::~MemoryModule()
{
    m_enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
void MemoryModule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

///////////////////////////////////////////////////////////////////////////////
bool MemoryModule::isEnabled(void) const
{
    return (m_enabled);
}

///////////////////////////////////////////////////////////////////////////////
bool MemoryModule::refresh(void)
{
    std::string tmp, name, line;
    int value;

    if (!m_enabled)
        return (false);

    std::ifstream input("/proc/meminfo");
    if (!input.is_open())
        return (false);
    while (std::getline(input, line)) {
        std::stringstream(line) >> name >> value;
        m_data[name.substr(0, name.length() - 1)] = value;
    }

    m_memTotal = m_data["MemTotal"];
    m_memFree = m_data["MemAvailable"];
    m_memUsed = (((float)m_memTotal - (float)m_memFree) * 100.f) / (float)m_memTotal;

    m_swapTotal = m_data["SwapTotal"];
    m_swapFree = m_data["SwapFree"];
    m_swapUsed = (((float)m_swapTotal - (float)m_swapFree) * 100.f) / (float)m_swapTotal;

    if (m_graph.size() >= 200)
            m_graph.pop_back();
    m_graph.push_front({m_memUsed, m_swapUsed});

    return (true);
}

///////////////////////////////////////////////////////////////////////////////
float MemoryModule::getMemoryUsed(void) const
{
    return (m_memUsed);
}

///////////////////////////////////////////////////////////////////////////////
int MemoryModule::getMemoryTotal(void) const
{
    return (m_memTotal);
}

///////////////////////////////////////////////////////////////////////////////
int MemoryModule::getMemoryFree(void) const
{
    return (m_memFree);
}

///////////////////////////////////////////////////////////////////////////////
float MemoryModule::getSwapUsed(void) const
{
    return (m_swapUsed);
}

///////////////////////////////////////////////////////////////////////////////
int MemoryModule::getSwapTotal(void) const
{
    return (m_swapTotal);
}

///////////////////////////////////////////////////////////////////////////////
int MemoryModule::getSwapFree(void) const
{
    return (m_swapFree);
}

///////////////////////////////////////////////////////////////////////////////
std::list<MemoryModule::Data> MemoryModule::getGraph(void) const
{
    return (m_graph);
}

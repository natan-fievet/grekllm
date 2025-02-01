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
#include <sstream>
#include <chrono>

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
    if (!m_enabled)
        return false;

    std::ifstream file("/proc/net/dev");
    if (!file.is_open())
        return false;

    std::string line;
    unsigned long long currentUp = 0, currentDown = 0;
    static unsigned long long lastUp = 0, lastDown = 0;
    static auto lastTime = std::chrono::steady_clock::now();

    std::getline(file, line);
    std::getline(file, line);

    while (std::getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos)
            continue;

        std::istringstream ss(line.substr(colonPos + 1));
        unsigned long long rxBytes, rxPackets, rxErrs, rxDrop, rxFifo, rxFrame, rxCompressed, rxMulticast;
        unsigned long long txBytes, txPackets, txErrs, txDrop, txFifo, txColls, txCarrier, txCompressed;

        ss >> rxBytes >> rxPackets >> rxErrs >> rxDrop >> rxFifo >> rxFrame >> rxCompressed >> rxMulticast
           >> txBytes >> txPackets >> txErrs >> txDrop >> txFifo >> txColls >> txCarrier >> txCompressed;

        currentDown += rxBytes;
        currentUp += txBytes;
    }

    auto currentTime = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(currentTime - lastTime).count();

    if (lastUp > 0 && lastDown > 0 && elapsed > 0) {
        m_up = static_cast<float>((currentUp - lastUp) / elapsed / 1024.0);
        m_down = static_cast<float>((currentDown - lastDown) / elapsed / 1024.0);

        if (m_graph.size() >= 200)
            m_graph.pop_back();
        m_graph.push_front({m_up, m_down});
    }

    lastUp = currentUp;
    lastDown = currentDown;
    lastTime = currentTime;

    file.close();
    return true;
}

///////////////////////////////////////////////////////////////////////////////
void NetworkModule::setUp(float up)
{
    m_up = up;
}

///////////////////////////////////////////////////////////////////////////////
void NetworkModule::setDown(float down)
{
    m_down = down;
}

///////////////////////////////////////////////////////////////////////////////
float NetworkModule::getUp(void) const
{
    return (m_up);
}

///////////////////////////////////////////////////////////////////////////////
float NetworkModule::getDown(void) const
{
    return (m_down);
}

///////////////////////////////////////////////////////////////////////////////
std::list<NetworkModule::Data> NetworkModule::getGraph(void) const
{
    return (m_graph);
}

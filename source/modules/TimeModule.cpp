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
#include "modules/TimeModule.hpp"
#include <algorithm>
#include <ctime>
#include <chrono>

///////////////////////////////////////////////////////////////////////////////
TimeModule::TimeModule(void)
    : m_enabled(true)
{}

///////////////////////////////////////////////////////////////////////////////
TimeModule::~TimeModule()
{
    m_enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
void TimeModule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

///////////////////////////////////////////////////////////////////////////////
bool TimeModule::isEnabled(void) const
{
    return (m_enabled);
}

///////////////////////////////////////////////////////////////////////////////
bool TimeModule::refresh(void)
{
    if (!m_enabled)
        return (false);
    auto now = std::chrono::system_clock::now();
    std::time_t tnow = std::chrono::system_clock::to_time_t(now);
    std::string snow = std::ctime(&tnow);
    snow.erase(std::remove(snow.begin(), snow.end(), '\n'), snow.end());
    setTime(snow);
    return (true);
}

///////////////////////////////////////////////////////////////////////////////
void TimeModule::setTime(const std::string& time)
{
    m_time = time;
}

///////////////////////////////////////////////////////////////////////////////
std::string TimeModule::getTime(void) const
{
    return (m_time);
}

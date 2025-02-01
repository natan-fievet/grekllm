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
#include "modules/ServiceModule.hpp"
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <array>
#include <iostream>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
ServiceModule::ServiceModule(void)
    : m_enabled(true)
{}

///////////////////////////////////////////////////////////////////////////////
ServiceModule::~ServiceModule()
{
    m_enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
void ServiceModule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

///////////////////////////////////////////////////////////////////////////////
bool ServiceModule::isEnabled(void) const
{
    return (m_enabled);
}

///////////////////////////////////////////////////////////////////////////////
bool ServiceModule::refresh(void)
{
    static const char* command = "systemctl list-units --type=service --all --no-pager --no-legend";
    if (!m_enabled)
        return (false);
    m_data.clear();

    std::array<char, 128> buffer;
    std::string result;
    using PipeDeleter = int (*)(FILE*);
    std::unique_ptr<FILE, PipeDeleter> pipe(popen(command, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    std::istringstream iss(result);
    std::string line;
        while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        Data service;

        std::string serviceNameWithDot;
        lineStream >> serviceNameWithDot;

        if (!serviceNameWithDot.empty() && serviceNameWithDot == "●") {
            lineStream >> service.name;
        } else service.name = serviceNameWithDot;

        std::string status;
        lineStream >> status;
        service.status = status;

        m_data.push_back(service);
    }
    return (true);
}

///////////////////////////////////////////////////////////////////////////////
std::vector<ServiceModule::Data> ServiceModule::getServices(void) const
{
    return (m_data);
}

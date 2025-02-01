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
#include "modules/ProcessModule.hpp"
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <array>
#include <iostream>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
ProcessModule::ProcessModule(void)
    : m_enabled(true)
{}

///////////////////////////////////////////////////////////////////////////////
ProcessModule::~ProcessModule()
{
    m_enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
void ProcessModule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

///////////////////////////////////////////////////////////////////////////////
bool ProcessModule::isEnabled(void) const
{
    return (m_enabled);
}

///////////////////////////////////////////////////////////////////////////////
bool ProcessModule::refresh(void)
{
    static const char* command = "/usr/bin/top -b -n 1 | /usr/bin/grep -E '^[ ]*[0-9]+'";

    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");

    m_data.clear();

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        result += buffer.data();

    std::istringstream iss(result);
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        Data process;

        lineStream >> process.pid >> process.user >> process.pr >> process.ni >> process.virt
                   >> process.res >> process.shr >> process.state >> process.cpu >> process.mem
                   >> process.time;

        std::getline(lineStream, process.command);
        process.command.erase(0, process.command.find_first_not_of(" "));


        if (!process.command.empty())
            m_data.push_back(process);
    }

    return (true);
}

///////////////////////////////////////////////////////////////////////////////
std::vector<ProcessModule::Data> ProcessModule::getProcesses(void) const
{
    return (m_data);
}

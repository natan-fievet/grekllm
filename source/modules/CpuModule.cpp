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
#include "modules/CpuModule.hpp"
#include <fstream>
#include <algorithm>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////
CpuModule::CpuModule(void)
    : m_enabled(true)
{}

///////////////////////////////////////////////////////////////////////////////
CpuModule::~CpuModule()
{
    m_enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
void CpuModule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

///////////////////////////////////////////////////////////////////////////////
bool CpuModule::isEnabled(void) const
{
    return (m_enabled);
}

///////////////////////////////////////////////////////////////////////////////
static std::string trimAndCamelCase(const std::string& str)
{
    size_t first = str.find_first_not_of(" _\t");
    if (first == std::string::npos) return ("");

    size_t last = str.find_last_not_of(" _\t");
    std::string trimmed = str.substr(first, last - first + 1);

    bool toUpper = false;
    std::string result;

    for (char c : trimmed)
    {
        if (c == ' ' || c == '_' || c == '\t') {
            toUpper = true;
        } else {
            if (toUpper) {
                result += toupper(c);
                toUpper = false;
            } else {
                result += tolower(c);
            }
        }
    }

    return (result);
}

///////////////////////////////////////////////////////////////////////////////
static std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return ("");
    size_t last = str.find_last_not_of(' ');
    return (str.substr(first, (last - first + 1)));
}

///////////////////////////////////////////////////////////////////////////////
static double getCpuUsage(void)
{
    std::ifstream statFile("/proc/stat");
    std::string line;
    std::getline(statFile, line);
    statFile.close();

    unsigned long long user, nice, system, idle, iowait, irq, softirq;
    sscanf(line.c_str(), "cpu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle, &iowait, &irq, &softirq);

    unsigned long long totalIdle = idle + iowait;
    unsigned long long totalNonIdle = user + nice + system + irq + softirq;
    unsigned long long total = totalIdle + totalNonIdle;

    static unsigned long long prevTotal = 0;
    static unsigned long long prevIdle = 0;

    unsigned long long diffTotal = total - prevTotal;
    unsigned long long diffIdle = totalIdle - prevIdle;

    prevTotal = total;
    prevIdle = totalIdle;

    return ((1.0 - (double)diffIdle / diffTotal) * 100.0);
}

///////////////////////////////////////////////////////////////////////////////
bool CpuModule::refresh(void)
{
    std::string line;
    std::ifstream cpuinfo;

    if (!m_enabled)
        return (false);
    cpuinfo.open("/proc/cpuinfo");
    if (!cpuinfo.is_open())
        return (false);
    m_procs.clear();
    m_procs.push_back(Proc());
    for (int i = 0; getline(cpuinfo, line);) {
        if (line.length() == 0) {
            m_procs[i].erase("flags");
            i++;
            m_procs.push_back(Proc());
            continue;
        }

        size_t colonPos = line.find(':');

        if (colonPos != std::string::npos) {
            std::string key = trimAndCamelCase(line.substr(0, colonPos));
            std::string value = trim(line.substr(colonPos + 1));
            m_procs[i][key] = value;
        }
    }
    m_usage = getCpuUsage();
    if (m_graph.size() == 200)
        m_graph.pop_front();
    m_graph.push_back(m_usage);
    m_procs.pop_back();
    cpuinfo.close();
    return (true);
}

///////////////////////////////////////////////////////////////////////////////
std::vector<CpuModule::Proc> CpuModule::getProcs(void) const
{
    return (m_procs);
}

///////////////////////////////////////////////////////////////////////////////
double CpuModule::getUsage(void) const
{
    return (m_usage);
}

///////////////////////////////////////////////////////////////////////////////
std::list<double> CpuModule::getGraph(void) const
{
    return (m_graph);
}

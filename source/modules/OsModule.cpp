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
#include "modules/OsModule.hpp"
#include <sys/utsname.h>

///////////////////////////////////////////////////////////////////////////////
OsModule::OsModule(void)
    : m_enabled(true)
{}

///////////////////////////////////////////////////////////////////////////////
OsModule::~OsModule()
{
    m_enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
void OsModule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

///////////////////////////////////////////////////////////////////////////////
bool OsModule::isEnabled(void) const
{
    return (m_enabled);
}

///////////////////////////////////////////////////////////////////////////////
bool OsModule::refresh(void)
{
    struct utsname buffer;
    std::string sysname = "";

    if (!m_enabled || uname(&buffer) != 0)
        return (false);
    sysname += buffer.sysname;
    sysname += " ";
    sysname += buffer.machine;
    setOs(sysname);
    setKernel(buffer.release);
    return (true);
}

///////////////////////////////////////////////////////////////////////////////
std::string OsModule::getOs(void) const
{
    return (m_os);
}

///////////////////////////////////////////////////////////////////////////////
std::string OsModule::getKernel(void) const
{
    return (m_kernel);
}

///////////////////////////////////////////////////////////////////////////////
void OsModule::setOs(const std::string& os)
{
    m_os = os;
}

///////////////////////////////////////////////////////////////////////////////
void OsModule::setKernel(const std::string& kernel)
{
    m_kernel = kernel;
}

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
#include "modules/UserModule.hpp"
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
UserModule::UserModule(void)
    : m_enabled(true)
{}

///////////////////////////////////////////////////////////////////////////////
UserModule::~UserModule()
{
    m_enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
void UserModule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

///////////////////////////////////////////////////////////////////////////////
bool UserModule::isEnabled(void) const
{
    return (m_enabled);
}

///////////////////////////////////////////////////////////////////////////////
std::string UserModule::getUserName(void) const
{
    return (m_user);
}

///////////////////////////////////////////////////////////////////////////////
std::string UserModule::getHostName(void) const
{
    return (m_host);
}

///////////////////////////////////////////////////////////////////////////////
void UserModule::setUserName(const std::string& name)
{
    m_user = name;
}

///////////////////////////////////////////////////////////////////////////////
void UserModule::setHostName(const std::string& name)
{
    m_host = name;
}

///////////////////////////////////////////////////////////////////////////////
bool UserModule::refresh(void)
{
    static const char *key[2] = {"USER", "HOSTNAME"};
    char *val[2];

    if (!m_enabled)
        return (false);
    for (int i = 0; i < 2; i++) {
        val[i] = getenv(key[i]);
        if (val[i] == nullptr)
            val[i] = (char*)"ERROR";
    }
    setUserName(val[0]);
    setHostName(val[0]);
    return (true);
}

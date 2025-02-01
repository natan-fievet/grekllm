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
#include "modules/BatteryModule.hpp"

///////////////////////////////////////////////////////////////////////////////
BatteryModule::BatteryModule(void) : m_enabled(true)
{

}

///////////////////////////////////////////////////////////////////////////////
BatteryModule::~BatteryModule(void)
{
    setEnabled(false);
}

bool BatteryModule::isEnabled(void) const
{
    return m_enabled;
}

///////////////////////////////////////////////////////////////////////////////
void BatteryModule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

///////////////////////////////////////////////////////////////////////////////
std::string BatteryModule::getBattery(void) const
{
    if (m_battery.empty())
        return "";
    return m_battery[0].m_capacity + "%";
}

///////////////////////////////////////////////////////////////////////////////
void BatteryModule::setBattery(const std::string& battery)
{
    if (m_battery.empty())
        m_battery.push_back(Data{});
    m_battery[0].m_capacity = battery;
}


///////////////////////////////////////////////////////////////////////////////
bool BatteryModule::refresh(void)
{
    if (!m_enabled)
        return false;
    std::string path;
    if (std::filesystem::exists("/sys/class/power_supply/BAT0/"))
        path = "/sys/class/power_supply/BAT0/";
    else if (std::filesystem::exists("/sys/class/power_supply/BAT1/"))
        path = "/sys/class/power_supply/BAT1/";
    else
        return false;

    auto setValueFromFile = [&](const std::string &filename, std::string& field) {
        std::ifstream file(path + filename);
        if (file) {
            std::getline(file, field);
            file.close();
        }
    };
    if (m_battery.empty())
        m_battery.push_back(Data{});
    setValueFromFile("capacity", m_battery[0].m_capacity);
    setValueFromFile("capacity_level", m_battery[0].m_capacity_level);
    setValueFromFile("charge_full_design", m_battery[0].m_charge_full_design);
    setValueFromFile("charge_full", m_battery[0].m_charge_full);
    setValueFromFile("charge_now", m_battery[0].m_charge_now);
    setValueFromFile("current_now", m_battery[0].m_current_now);
    setValueFromFile("manufacturer", m_battery[0].m_manufacturer);
    setValueFromFile("serial_number", m_battery[0].m_serial_number);
    setValueFromFile("type", m_battery[0].m_type);
    setValueFromFile("alarm", m_battery[0].m_alarm);
    setValueFromFile("voltage_min_design", m_battery[0].m_voltage_min_design);
    setValueFromFile("voltage_now", m_battery[0].m_voltage_now);
    setValueFromFile("model_name", m_battery[0].m_model_name);
    setValueFromFile("present", m_battery[0].m_present);
    setValueFromFile("status", m_battery[0].m_status);
    setValueFromFile("technology", m_battery[0].m_technology);
    return true;
}

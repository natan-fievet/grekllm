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
#include "modules/DiskModule.hpp"

///////////////////////////////////////////////////////////////////////////////
DiskModule::DiskModule(void)
    : m_enabled(true)
{}

///////////////////////////////////////////////////////////////////////////////
DiskModule::~DiskModule()
{
    m_enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
void DiskModule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

///////////////////////////////////////////////////////////////////////////////
bool DiskModule::isEnabled(void) const
{
    return (m_enabled);
}

///////////////////////////////////////////////////////////////////////////////
const std::vector<DiskModule::Data>& DiskModule::getDisks(void) const
{
    return (m_disks);
}

///////////////////////////////////////////////////////////////////////////////
bool DiskModule::refresh(void)
{
    if (!m_enabled)
        return (false);

    FILE* mtab = setmntent("/etc/mtab", "r");
    if (!mtab)
        return (false);

    struct mntent* entry;
    std::vector<Data> currentDisks;

    while ((entry = getmntent(mtab))) {
        if (strncmp(entry->mnt_fsname, "/dev/", 5) != 0)
            continue;

        struct statvfs stats;
        if (statvfs(entry->mnt_dir, &stats) != 0)
            continue;

        Data disk;
        disk.mountpoint = entry->mnt_dir;

        unsigned long blockSize = stats.f_frsize;
        disk.total = (blockSize * stats.f_blocks) / 1024;
        disk.available = (blockSize * stats.f_bavail) / 1024;
        disk.used = disk.total - (blockSize * stats.f_bfree) / 1024;
        disk.usage = (float)disk.used / disk.total * 100;

        currentDisks.push_back(disk);
    }

    endmntent(mtab);

    m_disks = currentDisks;

    return (true);
}

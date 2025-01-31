# TODO

!: MANDATORY
?: BONUSES

- ! add: Adding graphical interface
- ! add: Adding text interface
- ? add: Adding DiskModule (disk usage)
- ? add: Adding TemperatureModule (cpu, disk, ram temperatures)
- ? add: Adding ProcessModule (get the process information)

```c++
#include <iostream>
#include <cstdlib>
#include <fstream>

int main()
{
    // http://en.cppreference.com/w/cpp/utility/program/system
    std::system("/usr/bin/top -b -n 1 | /usr/bin/head -n5 > /tmp/result.txt");
    
    std::cout << "top result summary\n-----------------\n" 
              << std::ifstream( "/tmp/result.txt" ).rdbuf() ; 
}
```

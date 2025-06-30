# NrcCpp

**NrcCpp** is a C++ library for interfacing with NRC devices over TCP.  
It provides an easy-to-use class `NrcDevice` for connecting to a device, sending commands, reading relay and input states, and managing device operations.

## ⚙️ Requirements

- Windows with MinGW or Visual Studio
- TCP access to NRC device
- No external dependencies required beyond `<winsock2.h>`

## 🧩 Build

Use g++ (MinGW) to build the project:

```bash
g++ -o nrccpp main.cpp NrcDevice.cpp -lws2_32
```

Alternatively, use a Visual Studio solution/project.

## 📦 Structure

- `NrcDevice.h / .cpp`: Core library
- `main.cpp`: Example usage
- `README.md`: This file

## 🚀 Usage

Here's a complete example for using the library:

```cpp
#include <iostream>
#include <windows.h>
#include "NrcDevice.h"

int main() {
    try {
        std::string ip = "192.168.1.200";
        int port = 23;
        std::string username = "admin";
        std::string password = "admin";

        NrcDevice nrc(ip, port, username, password);
        nrc.connect();

        if (nrc.login()) {
            nrc.relayContact(1, 500);
            nrc.relayContact(2, 1000);
            nrc.relayOff(1);
            nrc.relayOn(2);

            std::cout << "Relays Status (hex): " << std::hex << nrc.getRelaysValues() << std::dec << std::endl;
            std::cout << "Relay 1 Status: " << nrc.getRelayValue(1) << std::endl;
            std::cout << "Relay 2 Status: " << nrc.getRelayValue(2) << std::endl;

            try {
                std::cout << "SW Inputs Status (hex): " << std::hex << nrc.getSwInputsValues() << std::dec << std::endl;
                std::cout << "SW 1 Status: " << nrc.getSwInputValue(1) << std::endl;
                std::cout << "SW 2 Status: " << nrc.getSwInputValue(2) << std::endl;
                std::cout << "SW 3 Status: " << nrc.getSwInputValue(3) << std::endl;
            } catch (const std::exception& e) {
                std::cout << "SW Error: " << e.what() << std::endl;
            }

            try {
                std::cout << "HV Inputs Status (hex): " << std::hex << nrc.getHvInputsValues() << std::dec << std::endl;
                std::cout << "HV 1 Status: " << nrc.getHvInputValue(1) << std::endl;
                std::cout << "HV 2 Status: " << nrc.getHvInputValue(2) << std::endl;
                std::cout << "HV 3 Status: " << nrc.getHvInputValue(3) << std::endl;
            } catch (const std::exception& e) {
                std::cout << "HV Error: " << e.what() << std::endl;
            }
        } else {
            std::cout << "Error in login" << std::endl;
        }

        nrc.disconnect();
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }

    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}
```

## ✅ Features

- Connect via TCP to NRC device
- Authenticate using username/password
- Turn relays on/off or trigger contact with delay
- Read relay status (as hex or bit)
- Read switch (SW) and high-voltage (HV) inputs
- Graceful error handling

## 📚 License

MIT License — free for personal and commercial use.

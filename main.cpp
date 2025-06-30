#include <iostream>
#include <windows.h>
#include "NrcDevice.h"

int main() {
    try {
        std::string ip = "192.168.1.203";
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
                std::cout << "SW Inputs Status: " << std::hex << nrc.getSwInputsValues() << std::dec << std::endl;
                std::cout << "SW 1 Status: " << nrc.getSwInputValue(1) << std::endl;
                std::cout << "SW 2 Status: " << nrc.getSwInputValue(2) << std::endl;
                std::cout << "SW 3 Status: " << nrc.getSwInputValue(3) << std::endl;
            } catch (const std::exception& e) {
                std::cout << "SW Inputs Status: " << e.what() << std::endl;
            }

            try {
                std::cout << "HV Inputs Status: " << std::hex << nrc.getHvInputsValues() << std::dec << std::endl;
                std::cout << "HV 1 Status: " << nrc.getHvInputValue(1) << std::endl;
                std::cout << "HV 2 Status: " << nrc.getHvInputValue(2) << std::endl;
                std::cout << "HV 3 Status: " << nrc.getHvInputValue(3) << std::endl;
            } catch (const std::exception& e) {
                std::cout << "HV Inputs Status: " << e.what() << std::endl;
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

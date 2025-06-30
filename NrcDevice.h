#ifndef NRCDEVICE_H
#define NRCDEVICE_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class NrcDevice {
private:
    std::string ip;
    int port;
    std::string username;
    std::string password;
    SOCKET sockfd;

    int getBit(int num, int index);

public:
    NrcDevice(const std::string& ip, int port, const std::string& username, const std::string& password);
    ~NrcDevice();

    void connect();
    void disconnect();
    std::string send(const std::string& command);
    bool login();
    void relayContact(int relayCode, int delay_ms = -1);
    void relayOn(int relayCode);
    void relayOff(int relayCode);
    int getRelaysValues();
    int getRelayValue(int relayCode);
    int getSwInputsValues();
    int getSwInputValue(int inputCode);
    int getHvInputsValues();
    int getHvInputValue(int inputCode);
};

#endif

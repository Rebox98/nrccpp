#include "NrcDevice.h"
#include <stdexcept>
#include <cstring>
#include <thread>
#include <chrono>

NrcDevice::NrcDevice(const std::string& ip, int port, const std::string& username, const std::string& password)
    : ip(ip), port(port), username(username), password(password), sockfd(INVALID_SOCKET) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        throw std::runtime_error("WSAStartup failed");
}

NrcDevice::~NrcDevice() {
    disconnect();
    WSACleanup();
}

void NrcDevice::connect() {
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET)
        throw std::runtime_error("Socket creation failed");

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (::connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
        throw std::runtime_error("Connection failed");
}


void NrcDevice::disconnect() {
    if (sockfd != INVALID_SOCKET) {
        closesocket(sockfd);
        sockfd = INVALID_SOCKET;
    }
}

std::string NrcDevice::send(const std::string& command) {
    if (sockfd == INVALID_SOCKET)
        throw std::runtime_error("Not connected");

    int sent = ::send(sockfd, command.c_str(), command.size(), 0);
    if (sent == SOCKET_ERROR)
        throw std::runtime_error("Send failed");

    char buffer[1024];
    int received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (received <= 0)
        throw std::runtime_error("Receive failed");

    buffer[received] = '\0';
    return std::string(buffer);
}

bool NrcDevice::login() {
    std::string res = send(username + ":" + password);
    return res == "Successful Login";
}

void NrcDevice::relayContact(int relayCode, int delay_ms) {
    std::string cmd = "%RCT" + std::to_string(relayCode);
    if (delay_ms >= 0)
        cmd += ":" + std::to_string(delay_ms);
    send(cmd);
}

void NrcDevice::relayOn(int relayCode) {
    send("%RON" + std::to_string(relayCode));
}

void NrcDevice::relayOff(int relayCode) {
    send("%ROF" + std::to_string(relayCode));
}

int NrcDevice::getBit(int num, int index) {
    return (num >> index) & 1;
}

int NrcDevice::getRelaysValues() {
    std::string res = send("%RST");
    if (res.back() == 'h')
        return std::stoi(res.substr(0, res.size() - 1), nullptr, 16);
    throw std::runtime_error("Invalid Response! Response must end with 'h'");
}

int NrcDevice::getRelayValue(int relayCode) {
    return getBit(getRelaysValues(), relayCode - 1);
}

int NrcDevice::getSwInputsValues() {
    std::string res = send("%ISW");
    if (res == "Error")
        throw std::runtime_error("Device not support feature!");
    if (res.back() == 'h')
        return std::stoi(res.substr(0, res.size() - 1), nullptr, 16);
    throw std::runtime_error("Invalid Response! Response must end with 'h'");
}

int NrcDevice::getSwInputValue(int inputCode) {
    return getBit(getSwInputsValues(), inputCode - 1);
}

int NrcDevice::getHvInputsValues() {
    std::string res = send("%IHV");
    if (res == "Error")
        throw std::runtime_error("Device not support feature!");
    if (res.back() == 'h')
        return std::stoi(res.substr(0, res.size() - 1), nullptr, 16);
    throw std::runtime_error("Invalid Response! Response must end with 'h'");
}

int NrcDevice::getHvInputValue(int inputCode) {
    return getBit(getHvInputsValues(), inputCode - 1);
}

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

void resolveDNS(const std::string& domain) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WinSock initialization error" << std::endl;
        return;
    }

    addrinfo hints{}, * res, * p;
    hints.ai_family = AF_UNSPEC; // Support both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(domain.c_str(), nullptr, &hints, &res);
    if (status != 0) {
        std::cerr << "Error retrieving DNS record: " << gai_strerrorA(status) << std::endl;
        WSACleanup();
        return;
    }

    std::cout << "IP addresses for " << domain << ":" << std::endl;
    for (p = res; p != nullptr; p = p->ai_next) {
        char ipStr[INET6_ADDRSTRLEN];
        void* addr;

        if (p->ai_family == AF_INET) { // IPv4
            sockaddr_in* ipv4 = (sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
        }
        else if (p->ai_family == AF_INET6) { // IPv6
            sockaddr_in6* ipv6 = (sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }
        else {
            continue;
        }

        inet_ntop(p->ai_family, addr, ipStr, sizeof(ipStr));
        std::cout << "  " << ipStr << std::endl;
    }

    freeaddrinfo(res);
    WSACleanup();
}

int main() {
    std::string domain;
    std::cout << "Enter domain name: ";
    std::cin >> domain;
    resolveDNS(domain);
    return 0;
}

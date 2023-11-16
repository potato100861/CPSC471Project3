#include <iostream>  // Include for standard I/O
#include <netdb.h>   // Include for network database operations
#include <netinet/in.h>  // Include for internet domain address structures
#include <sys/types.h>  // Include for system data type definitions
#include <ifaddrs.h>  // Include for interface addresses structure
#include <cstring>  // Include for string handling

// Function to get local IPv4 address
std::string getLocalIPv4() {
    struct ifaddrs *ifaddr, *ifa;  // Pointers for interface address structures
    int family, s;  // Variables for address family and return status
    char host[NI_MAXHOST];  // Buffer to hold the IP address

    // Getting linked list of network interfaces
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");  // Print error if getifaddrs fails
        exit(EXIT_FAILURE);  // Exit on failure
    }

    // Loop through linked list of interfaces
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) {
            continue;  // Skip if interface address is null
        }

        family = ifa->ifa_addr->sa_family;  // Get address family of current interface

        // Check if the family is IPv4 and not the loopback interface
        if (family == AF_INET && strcmp(ifa->ifa_name, "lo") != 0) {
            // Convert internet address to network address structure
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                            host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));  // Print error if getnameinfo fails
                exit(EXIT_FAILURE);  // Exit on failure
            }

            // Check if the address is not 127.0.0.1 (loopback address)
            if(strcmp(host, "127.0.0.1") != 0) {
                freeifaddrs(ifaddr);  // Free the interface addresses linked list
                return "Local IPv4 Address: " + std::string(host);  // Return the first non-loopback IPv4 address
            }
        }
    }

    freeifaddrs(ifaddr);  // Free the interface addresses linked list
    return "No IPv4 address found";  // Return this string if no address is found
}

// Main function
int main() {
    std::cout << getLocalIPv4() << std::endl;  // Print the IP address
    return 0;  // Return 0 to indicate successful execution
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "protocol.h" // Include the protocol header

#define DEFAULT_IP "127.0.0.1" // Default server IP address
#define DEFAULT_PORT 8080      // Default port for the server


// Function to handle errors
void errorhandler(const char *errorMessage) {
    fprintf(stderr, "%s\n", errorMessage);
}

// Function to clean Winsock
void clearwinsock() {
    WSACleanup();
}

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    int result;

    // Winsock initialization
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        errorhandler("Error: WSAStartup failed");
        return 1;
    }

    // Create the client socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        errorhandler("Error: Unable to create socket");
        clearwinsock();
        return 1;
    }

    // Configure the server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(DEFAULT_IP);
    serverAddr.sin_port = htons(DEFAULT_PORT);

    // Connect to the server
    printf("Connecting to server %s:%d...\n", DEFAULT_IP, DEFAULT_PORT);
    result = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        errorhandler("Error: Connection failed");
        closesocket(clientSocket);
        clearwinsock();
        return 1;
    }

    printf("Connected to the server at %s:%d\n", DEFAULT_IP, DEFAULT_PORT);

    // Main loop to interact with the server
    while (1) {
        printf("\nEnter password request (type and length, e.g., 'n 10', or 'q' to quit): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Remove newline character if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Check for exit condition
        if (strncmp(buffer, "q", 1) == 0) {
            printf("Closing connection...\n");
            break;
        }

        // Validate the request format
        char type;
        int length;
        if (sscanf(buffer, "%c %d", &type, &length) != 2 ||
            length < MIN_PASSWORD_LENGTH || length > MAX_PASSWORD_LENGTH) {
            printf("Invalid request format. Example of a valid request: 'n 10'\n");
            continue;
        }

        // Send the request to the server
        if (send(clientSocket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
            errorhandler("Error: Failed to send request to server");
            break;
        }

        // Receive the generated password from the server
        result = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (result == SOCKET_ERROR) {
            errorhandler("Error: Failed to receive data from server");
            break;
        }

        buffer[result] = '\0'; // Add null terminator
        printf("Generated password: %s\n", buffer);
    }

    // Close the socket and clean up Winsock
    closesocket(clientSocket);
    clearwinsock();
    printf("Connection closed.\n");

    return 0;
}

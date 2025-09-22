#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include "protocol.h" // Include the protocol header

#define DEFAULT_PORT 8080  // Default port for the server
#define QLEN 4          // Maximum client queue length (setting it to 4 to account for the implicit +1)

// Error handler function to print error messages
void errorhandler(const char *errorMessage) {
    perror(errorMessage);  // Prints the system error message related to the error
    fprintf(stderr, "Error occurred: %s\n", errorMessage);  // Detailed error message printed to stderr
}

// Function to generate a numeric password (only digits)
void generate_numeric(char *password, int length) {
    char digits[] = "0123456789";
    for (int i = 0; i < length; i++) {
    	password[i] = digits[rand() % (sizeof(digits) - 1)];  // Random digits
    }
    password[length] = '\0';  // Null-terminate the string
}

// Function to generate an alphabetic password (only lowercase letters)
void generate_alpha(char *password, int length) {
    char letters[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < length; i++) {
    	password[i] = letters[rand() % (sizeof(letters) - 1)];  // Random letters
    }
    password[length] = '\0';  // Null-terminate the string
}

// Function to generate a mixed password (letters and digits)
void generate_mixed(char *password, int length) {
    char letters_digits[] = "abcdefghijklmnopqrstuvwxyz0123456789"; // Valid characters (letters + digits)
    for (int i = 0; i < length; i++) {
        password[i] = letters_digits[rand() % (sizeof(letters_digits) - 1)];  // Random letter or digit
    }
    password[length] = '\0';  // Null-terminate the string
}

// Function to generate a secure password (letters, digits, and symbols)
void generate_secure(char *password, int length) {
    char secure_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?";
    for (int i = 0; i < length; i++) {
        password[i] = secure_chars[rand() % strlen(secure_chars)];  // Random secure character
    }
    password[length] = '\0';
}

// Function to handle a single client request
void handle_client_request(SOCKET clientSocket, struct sockaddr_in clientAddr) {
    char buffer[BUFFER_SIZE];
    int result;

    printf("New connection from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    while (1) {
        result = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (result <= 0) break; // Terminate if client disconnects

        buffer[result] = '\0'; // Ensure the buffer is null-terminated
        printf("Received request: %s\n", buffer);

        char type;
        int length;

        // Parse the client's request (e.g., "n 10")
        if (sscanf(buffer, "%c %d", &type, &length) != 2 ||
            length < MIN_PASSWORD_LENGTH || length > MAX_PASSWORD_LENGTH) {
            const char *error = "Invalid request format\n";
            send(clientSocket, error, strlen(error), 0);
            continue;
        }

        // Buffer to hold the generated password
        char password[MAX_PASSWORD_LENGTH + 1];
        // Generate the requested password
        switch (type) {
            case 'n':
                generate_numeric(password, length);  // Generate numeric password
                break;
            case 'a':
                generate_alpha(password, length);    // Generate alphabetic password
                break;
            case 'm':
                generate_mixed(password, length);    // Generate mixed password (letters + numbers)
                break;
            case 's':
                generate_secure(password, length);   // Generate secure password (letters, numbers, symbols)
                break;
            default:
                strcpy(password, "Invalid type");    // Handle invalid request
        }

        // Send the generated password back to the client
        send(clientSocket, password, strlen(password), 0);
    }

    printf("Connection closed for %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    closesocket(clientSocket); // Close the client socket
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    int activeClients = 0; // Track number of active clients

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Error: WSAStartup failed\n");
        return -1;
    }

    // Create the server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        fprintf(stderr, "Error: Unable to create socket\n");
        WSACleanup();
        return -1;
    }

    // Configure the server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(DEFAULT_PORT);

    // Bind the server socket to the address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        fprintf(stderr, "Error: Bind failed. Check if the port is already in use.\n");
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Start listening for client connections with a backlog of `QLEN` clients
    if (listen(serverSocket, QLEN) < 0) {
        fprintf(stderr, "Error: Listen failed\n");
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    printf("Server is listening on port %d...\n", DEFAULT_PORT);

    // Main loop to accept and handle clients
    while (1) {
        if (activeClients >= QLEN) {
            printf("Maximum number of clients (%d) reached. Server will shut down.\n", QLEN);
            break; // Exit the loop if max clients reached
        }

        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            fprintf(stderr, "Error: Accept failed\n");
            continue;
        }

        // Increment the active client count
        activeClients++;

        // Handle the client request
        handle_client_request(clientSocket, clientAddr);
    }

    // Cleanup
    closesocket(serverSocket);
    WSACleanup();
    printf("Server has shut down.\n");
    return 0;
}

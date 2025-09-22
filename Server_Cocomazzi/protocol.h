/*
 *  protocol.h
 *
 *  Created on: 21/11/2024
 *  Author: Andrea Francesco Cocomazzi
 *
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

// General limits and parameters
#define PROTO_PORT 8080             // Port for the connection
#define BUFFER_SIZE 64              // Buffer size for the message
#define MIN_PASSWORD_LENGTH 6       // Minimum password length
#define MAX_PASSWORD_LENGTH 32      // Maximum password length
#define QLEN 4						// Maximum number of pending connections in the queue (setting it to 4 to account for the implicit +1)

// Types of passwords that the server can generate
#define PASSWORD_TYPE_NUMERIC   'n'  // Numeric type
#define PASSWORD_TYPE_ALPHA     'a'  // Alphabetic type
#define PASSWORD_TYPE_MIXED     'm'  // Mixed type (alphabetic and numeric)
#define PASSWORD_TYPE_SECURE    's'  // Secure type (numbers, letters, symbols)

// Functions for password generation
void generate_numeric(char *password, int length);  // Generate a numeric password
void generate_alpha(char *password, int length);    // Generate an alphabetic password
void generate_mixed(char *password, int length);    // Generate a mixed password
void generate_secure(char *password, int length);   // Generate a secure password

#endif /* PROTOCOL_H_ */


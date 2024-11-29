#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include "HttpResponse.h"

int main(){
    // Step 1: Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1){
        perror("Failed to create socket");
        return 1;
    }

    // Step 2: Bind the socket to an IP address and port
    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if(bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        perror("Failed to bind");
        close(server_fd);
        return 1;
    }

    // Step 3: Listen for incoming connections
    if(listen(server_fd, 5) == -1){
        perror("Failed to listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Server is running on port 8080" << std::endl;

    // Step 4: Main loop for handling a connection
    while(true){
        // Accept a new connection
        sockaddr_in client_addr;
        std::memset(&client_addr, 0, sizeof(client_addr));
        socklen_t client_addr_size = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_addr_size);

        if (client_fd == -1) {
            perror("Accept failed");
            close(server_fd);
            return 1;
        }

        std::cout << "Client connected!\n";

        // Read data from the client
        char buffer[1024];
        std::memset(buffer, 0, sizeof(buffer));
        int bytes_read = recv(client_fd, buffer, sizeof(buffer)-1, 0);

        if(bytes_read > 0){
            std::cout << "Data Received:\n" << buffer << std::endl;

            // Parse request
            std::string method, path;
            // Create a istream object to read from buffer
            std::istringstream request_stream(buffer);
            // Read the method and path from the first line
            request_stream >> method >> path;

            HttpResponse response;
            if (method == "GET" && path == "/") {
                response = generate_response(200, "text/plain", "Welcome to the home page!");
            } else if (method == "GET" && path == "/about") {
                response = generate_response(200, "text/plain", "This is the about page.");
            } 
            else if (method != "GET"){
                response = generate_response(405, "text/plain", "Method not allowed.");
            }
            else {
                response = generate_response(404, "text/plain", "Page not found.");
            }

            // Step 7: Send the response
            std::string response_str = response.to_string();
            send(client_fd, response_str.c_str(), response_str.size(), 0);
        }
        else if(bytes_read == 0){
            std::cout << "Client disconnected\n";
        }
        else {
            perror("Failed to read data");
        }

        // Step 5: Close the client and server sockets
        close(client_fd);
    }
    
    close(server_fd);

    return 0;
}
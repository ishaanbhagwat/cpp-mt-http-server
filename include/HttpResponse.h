// Description: This file defines the HttpResponse struct, which represents an HTTP response.
#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>

// Define a struct to represent an HTTP response
struct HttpResponse{
    int status_code;
    std::map<std::string, std::string> headers;
    std::string body;

    // Define a method to convert the response to a string
    std::string to_string() const;

    private:
    std::string get_status_message() const;
};

HttpResponse generate_response(int status_code, const std::string& content_type, const std::string& body);

#endif // HTTPRESPONSE_H
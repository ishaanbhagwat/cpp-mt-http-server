#include "../include/HttpResponse.h"
#include <sstream>


// Converts HttpResponse object to response string
std::string HttpResponse::to_string() const{
    std::ostringstream response_stream;
    response_stream << "HTTP/1.1 " << status_code << " " << get_status_message() << "\r\n";

    // Add headers to the response
    for (const auto& [key, value]: headers){
        response_stream << key << ": " << value << "\r\n";
    }
    response_stream << "\r\n";
    response_stream << body;
    return response_stream.str();
}

std::string HttpResponse::get_status_message() const{
    switch (status_code) {
        case 200: return "OK";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 500: return "Internal Server Error";
        default: return "Unknown";
    }
}

// Factory function to generate an HttpResponse object
HttpResponse generate_response(int status_code, const std::string& content_type, const std::string& body){
    HttpResponse response;
    response.status_code = status_code;
    response.headers["Content-Type"] = content_type;
    response.headers["Content-Length"] = std::to_string(body.size());
    response.body = body;
    return response;
}
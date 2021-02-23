#include <iostream>
#include <string>
#include <map>
#include "httptny.h"

static const char* http_test_data =
"HTTP/1.1 200 OK\r\n"
"Content-Length: 50\r\n"
"Vary: Accept-Encoding\r\n"
"Server: TornadoServer/4.0.2.2\r\n"
"Etag: \"None\"\r\n"
"Cache-Control: private,max-age=300\r\n"
"Date: Mon, 22 Feb 2021 08:12:00 GMT\r\n"
"X-Frame-Options: DENY\r\n"
"Content-Type: application/json; ext=nn; charset=utf-8\r\n" 
"\r\n"
"{\"self\": \"/mit/me/1\", \"ctyp\": \"/cim/mm/moc/me,me\"}";

int main() {
    // http response parsing
    httptny::response_view http_response { http_test_data };
    auto contentValue = http_response.getHeaderValue("Content-Type");
    if (!contentValue.empty()) {
        std::cout << contentValue << std::endl;
    }
    auto serverValue = http_response.getHeaderValue("Server");
    if (!serverValue.empty()) {
        std::cout << serverValue << std::endl;
    }
    std::cout << http_response.getReturnCode() << std::endl;
    std::cout << http_response.getBody() << std::endl;
    auto headers = http_response.getHeaders<std::map>();
    for (auto header : headers) {
        std::cout << "> " << header.first << ": " << header.second << std::endl;
    }
    return 0;
}

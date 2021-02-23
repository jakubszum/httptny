# httptny
C++ Tiny HTTP parsing library

Usage example:

```c++
#include <iostream>
#include "httptny.h"

static const char* http_test_data = 
R"(HTTP/1.1 200 OK
Content-Length: 50
Vary: Accept-Encoding
Server: TornadoServer/4.0.2.2
Etag: "None"
Cache-Control: private,max-age=300
Date: Mon, 22 Feb 2021 08:12:00 GMT
X-Frame-Options: DENY
Content-Type: application/json; ext=nn; charset=utf-8

{"self": "/mit/me/1", "ctyp": "/cim/mm/moc/me,me"})";

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
```

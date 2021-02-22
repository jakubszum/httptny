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
    httptny::response_view http_response(http_test_data, { "Content-Type" });
    if (http_response.hasHeader("Content-Type")) {
        std::cout << http_response["Content-Type"] << std::endl;
    }
    if (http_response.hasHeader("Server")) {
        std::cout << http_response["Server"] << std::endl;
    }
    std::cout << http_response.getReturnCode() << std::endl;
    std::cout << http_response.getData() << std::endl;
    
    // http request generator
    httptny::request http_request("PATCH", "/ntfn/sub",
                                  R"([{"op": "replace", "path": "/ena", "value": true}])");
    std::cout << http_request.get() << std::endl;
    return 0;
}
```

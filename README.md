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
<<<<<<< HEAD
    if (http_response.getHeaders().contains("Content-Type")) {
        std::cout << http_response.getHeaders().at("Content-Type") << std::endl;
    }
    if (http_response.getHeaders().contains("Server")) {
        std::cout << http_response.getHeaders().at("Server") << std::endl;
=======
    if (http_response.getHeaders().exist("Content-Type")) {
        std::cout << http_response.getHeaders()["Content-Type"] << std::endl;
    }
    if (http_response.getHeaders().exist("Server")) {
        std::cout << http_response.getHeaders()["Server"] << std::endl;
>>>>>>> 564bc6137185af68b8ef746e00bc1ca8efcdcddd
    }
    std::cout << http_response.getReturnCode() << std::endl;
    std::cout << http_response.getBody() << std::endl;
    
    // http request generator
    httptny::request http_request("PATCH", "/ntfn/sub",
                                  R"([{"op": "replace", "path": "/ena", "value": true}])");
    std::cout << http_request.get() << std::endl;
    return 0;
}
```

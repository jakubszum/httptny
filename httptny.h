// Copyright (C) 2021 Jakub Szumacher. All rights reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#ifndef HTTPTNY_H_
#define HTTPTNY_H_

#include <cstring>
#include <cassert>
#include <vector>
#include <algorithm>
#ifdef __cpp_lib_string_view
#include <string_view>
#endif

namespace httptny {

#ifdef __cpp_lib_string_view
using string_view_t = std::string_view;
#else
using string_view_t = std::string;
#endif

#ifdef HTTPTNY_HTTP_VERSION_DECLARATION
constexpr const char httpVersionDeclaration[] = HTTPTNY_HTTP_VERSION_DECLARATION;
#else
constexpr const char httpVersionDeclaration[] = "HTTP/1.1";
#endif

#ifdef HTTPTNY_LINE_ENDING
constexpr const char lineEnding[] = HTTPTNY_LINE_ENDING;
#else
constexpr const char lineEnding[] = "\r\n";
#endif

constexpr const char lineEndingChar = lineEnding[sizeof(lineEnding) - 2];

struct header_entry {
    string_view_t key;
    string_view_t value;
};

class response_view {
 private:
    std::vector<header_entry> m_headers;
    int32_t m_returnCode;
    string_view_t m_data;

 public:
    response_view(const string_view_t& httpMessage,
                  std::initializer_list<string_view_t> reqHeaders = {}) {
        string_view_t remain = httpMessage;
        bool firstLine = true;
        while (true) {
            std::size_t endlinePos = remain.find(lineEndingChar);
            string_view_t line(remain.data(),
                               (endlinePos != std::string::npos ? endlinePos : remain.size()));
            if (firstLine) {
                std::size_t httpEndPos = line.find(' ');
                if (httpEndPos != std::string::npos) {
                    m_returnCode = std::atoi(line.data() + httpEndPos + 1);
                }
                firstLine = false;
            } else {
                std::size_t keyEndPos = line.find(": ");
                if (keyEndPos != std::string::npos) {
                    string_view_t key(line.data(), keyEndPos);
                    bool reqHeader = false;
                    if (reqHeaders.size() != 0) {
                        auto it = std::find(reqHeaders.begin(), reqHeaders.end(), key);
                        reqHeader = it != reqHeaders.end();
                    }
                    if (reqHeader) {
                        string_view_t value(line.data() + keyEndPos + 2,
                                            line.size() - keyEndPos - 2);
                        m_headers.emplace_back(header_entry {key, value});
                    }
                } else if(line.size() == 0) {
                    m_data = string_view_t(remain.data() + endlinePos + 1);
                    return;
                }
            }
            if (endlinePos == std::string::npos) {
                break;
            }
            remain = string_view_t(remain.data() + endlinePos + 1);
        }
    }

    bool hasHeader(const string_view_t& key) const {
        auto it = std::find_if(m_headers.begin(), m_headers.end(),
                              [key](const header_entry& header) { return header.key == key; });
        return it != m_headers.end();
    }

    const string_view_t& operator[](const string_view_t& key) const {
        auto it = std::find_if(m_headers.begin(), m_headers.end(),
                              [key](const header_entry& header) { return header.key == key; });
        assert(it != m_headers.end());
        return it->value;
    }

    const string_view_t& getData() const {
        return m_data;
    }

    const int32_t getReturnCode() const {
        return m_returnCode;
    }
};

class request {
 private:
    std::vector<header_entry> m_headers;
    string_view_t m_type;
    string_view_t m_url;
    string_view_t m_data;

 public:
    request(const string_view_t& type, const string_view_t& url, const string_view_t& data,
            std::initializer_list<header_entry> headers = {})
        : m_type(type), m_url(url), m_data(data) {
        for (auto header : headers) {
            m_headers.emplace_back(std::move(header));
        }
    }

    std::string get() const {
        std::string output;
        output += m_type;
        output += " ";
        output += m_url;
        output += " ";
        output += httpVersionDeclaration;
        output += lineEnding;
        for (auto header : m_headers) {
            output += header.key;
            output += ": ";
            output += header.value;
            output += lineEnding;
        }
        std::size_t contentLength = m_data.size() + std::strlen(lineEnding);
        output += "Content-Length: ";
        output += std::to_string(contentLength);
        output += lineEnding;
        output += lineEnding;
        output += m_data;
        return output;
    }
};

};  // namesapce httptny

#endif  // HTTPTNY_H_
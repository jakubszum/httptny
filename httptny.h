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

#include <string>
#include <utility>
#ifdef __cpp_lib_string_view
#include <string_view>
#endif

namespace httptny {

#ifdef __cpp_lib_string_view
using string_view_t = std::string_view;
#else
using string_view_t = std::string;
#endif

class response_view {
 private:
    const string_view_t m_httpMessage;

 public:
    explicit response_view(string_view_t&& httpMessage) : m_httpMessage(httpMessage) { }

    int32_t getReturnCode() const {
        std::size_t httpEndPos = m_httpMessage.find(' ');
        if (httpEndPos != std::string::npos) {
            return std::atoi(m_httpMessage.data() + httpEndPos + 1);
        }
        return -1;
    }

    template<class TString = string_view_t>
    TString getBody() const {
        string_view_t remain = m_httpMessage;
        while (true) {
            std::size_t endlinePos = remain.find('\n');
            string_view_t line(remain.data(), (endlinePos == std::string::npos ?
                                                remain.size() :
                                                (remain[endlinePos - 1] == '\r' ?
                                                        endlinePos - 1 : endlinePos)));
            if (line.size() == 0) {
                return TString(remain.data() + endlinePos + 1);
            }
            if (endlinePos == std::string::npos) {
                break;
            }
            remain = string_view_t(remain.data() + endlinePos + 1);
        }
        return TString();
    }

    template<class TString = string_view_t>
    TString getHeaderValue(const string_view_t& headerKey) const {
        string_view_t remain = m_httpMessage;
        while (true) {
            std::size_t endlinePos = remain.find('\n');
            string_view_t line(remain.data(), (endlinePos == std::string::npos ?
                                                remain.size() :
                                                (remain[endlinePos - 1] == '\r' ?
                                                        endlinePos - 1 : endlinePos)));
            std::size_t keyEndPos = line.find(": ");
            if (keyEndPos != std::string::npos) {
                string_view_t key(line.data(), keyEndPos);
                if (key == headerKey) {
                    TString value(line.data() + keyEndPos + 2,
                                  line.size() - keyEndPos - 2);
                    return value;
                }
            }
            if (line.size() == 0 || endlinePos == std::string::npos) {
                break;
            }
            remain = string_view_t(remain.data() + endlinePos + 1);
        }
        return TString();
    }

    template <template<class...> class TContainer, class TString = string_view_t>
    auto getHeaders() const -> TContainer<TString, TString> {
        TContainer<TString, TString> headers;
        string_view_t remain = m_httpMessage;
        while (true) {
            std::size_t endlinePos = remain.find('\n');
            string_view_t line(remain.data(), (endlinePos == std::string::npos ?
                                                remain.size() :
                                                (remain[endlinePos - 1] == '\r' ?
                                                        endlinePos - 1 : endlinePos)));
            std::size_t keyEndPos = line.find(": ");
            if (keyEndPos != std::string::npos) {
                TString key(line.data(), keyEndPos);
                TString value(line.data() + keyEndPos + 2,
                              line.size() - keyEndPos - 2);
                headers.emplace(std::move(key), std::move(value));
            }
            if (line.size() == 0 || endlinePos == std::string::npos) {
                break;
            }
            remain = string_view_t(remain.data() + endlinePos + 1);
        }
        return headers;
    }
};

};  // namespace httptny

#endif  // HTTPTNY_H_

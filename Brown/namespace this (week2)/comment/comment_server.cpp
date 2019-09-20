#include "test_runner.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include <optional>
#include <unordered_set>

using namespace std;

struct HttpRequest {
    string method, path, body;
    map<string, string> get_params;
};

enum class HttpCode {
    Ok = 200,
    NotFound = 404,
    Found = 302,
};

class HttpResponse {
private:
    HttpCode code_;
    multimap<string, string> headers;
    string content;

public:
    explicit HttpResponse(HttpCode code);

    HttpResponse& AddHeader(string name, string value);
    HttpResponse& SetContent(string a_content);
    HttpResponse& SetCode(HttpCode a_code);

    HttpCode GetCode() const {
        return code_;
    }

    multimap<string, string> GetHeadets() const {
        return headers;
    }

    string GetContent() const {
        return content;
    }

    friend ostream& operator << (ostream& output, const HttpResponse& resp) {
        output << "HTTP/1.1";
        auto code = resp.GetCode();
        output << ' ' << static_cast<size_t>(code);
        string result = [&code]() {
            string res;
            if (code == HttpCode::Ok) {
                res = "OK";
            } else if (code == HttpCode::Found) {
                res = "Found";
            } else {
                res = "Not found";
            }
            return move(res);
        }();
        output << ' ' << result;
        output << '\n';
        for (const auto& [key, value] : resp.GetHeadets()) {
            output << key << ':' << ' ' << value << '\n';
        }
        string content = resp.GetContent();
        if (!content.empty()) {
            output << "Content-Length: " << content.size() << '\n';
        }
        output << '\n';
        if (!content.empty()) {
            output << content;
        }
        stringstream ss;
        return output;
    }
};

pair<string, string> SplitBy(const string& what, const string& by) {
    size_t pos = what.find(by);
    if (by.size() < what.size() && pos < what.size() - by.size()) {
        return {what.substr(0, pos), what.substr(pos + by.size())};
    } else {
        return {what, {}};
    }
}

template<typename T>
T FromString(const string& s) {
    T x;
    istringstream is(s);
    is >> x;
    return x;
}

pair<size_t, string> ParseIdAndContent(const string& body) {
    auto [id_string, content] = SplitBy(body, " ");
    return {FromString<size_t>(id_string), content};
}

struct LastCommentInfo {
    size_t user_id, consecutive_count;
};

class CommentServer {
private:
    vector<vector<string>> comments_;
    std::optional<LastCommentInfo> last_comment;
    unordered_set<size_t> banned_users;

public:
    HttpResponse ServeRequest(const HttpRequest& req) {
        HttpResponse httpResponse(HttpCode::Ok);
        if (req.method == "POST") {
            if (req.path == "/add_user") {
                comments_.emplace_back();
                auto response = to_string(comments_.size() - 1);
                httpResponse.SetContent(response);
            } else if (req.path == "/add_comment") {
                auto [user_id, comment] = ParseIdAndContent(req.body);

                if (!last_comment || last_comment->user_id != user_id) {
                    last_comment = LastCommentInfo {user_id, 1};
                } else if (++last_comment->consecutive_count > 3) {
                    banned_users.insert(user_id);
                }

                if (banned_users.count(user_id) == 0) {
                    comments_[user_id].push_back(string(comment));
                } else {
                    httpResponse.SetCode(HttpCode::Found);
                    httpResponse.AddHeader("Location", "/captcha");
                }
            } else if (req.path == "/checkcaptcha") {
                if (auto [id, response] = ParseIdAndContent(req.body); response == "42") {
                    banned_users.erase(id);
                    if (last_comment && last_comment->user_id == id) {
                        last_comment.reset();
                    } else {
                        httpResponse.SetCode(HttpCode::Found);
                        httpResponse.AddHeader("Location", "/captcha");
                    }
                } else {
                    httpResponse.SetCode(HttpCode::Found);
                    httpResponse.AddHeader("Location", "/captcha");
                }
            } else {
                httpResponse.SetCode(HttpCode::NotFound);
            }
        } else if (req.method == "GET") {
            if (req.path == "/user_comments") {
                auto user_id = FromString<size_t>(req.get_params.at("user_id"));
                string response;
                for (const string& c : comments_[user_id]) {
                    response += c + '\n';
                }
                httpResponse.SetContent(response);
            } else if (req.path == "/captcha") {
                string response = "What's the answer for The Ultimate Question of Life, the Universe, and Everything?";
                httpResponse.SetContent(response);
            } else {
                httpResponse.SetCode(HttpCode::NotFound);
            }
        } else {
            httpResponse.SetCode(HttpCode::NotFound);
        }
        return httpResponse;
    }
};

struct HttpHeader {
    string name, value;
};

ostream& operator<<(ostream& output, const HttpHeader& h) {
    return output << h.name << ": " << h.value;
}

bool operator==(const HttpHeader& lhs, const HttpHeader& rhs) {
    return lhs.name == rhs.name && lhs.value == rhs.value;
}

struct ParsedResponse {
    int code;
    vector<HttpHeader> headers;
    string content;
};

istream& operator >>(istream& input, ParsedResponse& r) {
    string line;
    getline(input, line);

    {
        istringstream code_input(line);
        string dummy;
        code_input >> dummy >> r.code;
    }

    size_t content_length = 0;

    r.headers.clear();
    while (getline(input, line) && !line.empty()) {
        if (auto [name, value] = SplitBy(line, ": "); name == "Content-Length") {
            istringstream length_input(value);
            length_input >> content_length;
        } else {
            r.headers.push_back( {std::move(name), std::move(value)});
        }
    }

    r.content.resize(content_length);
    input.read(r.content.data(), r.content.size());
    return input;
}

HttpResponse::HttpResponse(HttpCode code) : code_(code) {}

HttpResponse& HttpResponse::AddHeader(string name, string value) {
    headers.insert({move(name), move(value)});
    return *this;
}

HttpResponse& HttpResponse::SetCode(HttpCode a_code) {
    code_ = a_code;
    return *this;
}

HttpResponse& HttpResponse::SetContent(string a_content) {
    content = move(a_content);
    return *this;
}
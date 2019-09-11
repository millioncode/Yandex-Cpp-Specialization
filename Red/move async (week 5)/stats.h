#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
#include <algorithm>
#include <iterator>
#include <iostream>
using namespace std;

class Stats {
    public:
        Stats(): method_  {
                {"GET", 0},
                {"PUT", 0},
                {"POST", 0},
                {"DELETE", 0},
                {"UNKNOWN", 0},
            },
                 uri_ {
        {"/", 0},
        {"/order", 0},
        {"/product", 0},
        {"/basket", 0},
        {"/help", 0},
        {"unknown", 0}
}
        {}

        void AddMethod(string_view method) {
            Add(method, method_, "UNKNOWN");
            /*while (1) {
                size_t space = method.find(' ');
                if (space == method.npos) {
                    break;
                }
                else {
                    auto word = method.substr(0, space);
                    auto it = method_.lower_bound(word);
                    if ( (*it).first == word  ){
                        method_.at(word)++;
                    }
                    else {
                        if (word!="") {
                            method_.at("UNKNOWN")++;
                        }
                    }
                    method.remove_prefix(space+1);
                }
            }*/
        }
        void AddUri(string_view uri) {
            Add(uri, uri_, "unknown");
            /*while (1) {
                size_t space = uri.find(' ');
                if (space == uri.npos) {
                    break;
                }
                else {
                    auto word = uri.substr(0, space);
                    auto it = uri_.lower_bound(word);
                    if ( (*it).first == word  ){
                        uri_.at(word)++;
                    }
                    else {
                        if (word!="") {
                            uri_.at("unknown")++;

                        }
                    }
                    uri.remove_prefix(space+1);
                }
            }*/
        }
        const map<string_view, int>& GetMethodStats() const {
            return method_;
        }
        const map<string_view, int>& GetUriStats() const {
            return uri_;
        }

    private:
        void Add(string_view& method, map<string_view, int>& container, const string& POLE) ;
        map<string_view, int> method_;
        map<string_view, int> uri_;

};
void Stats::Add(string_view& method, map<string_view, int>& container, const string& POLE) {
    while (1) {
        size_t space = method.find(' ');
        if (space == method.npos) {
            break;
        }
        else {
            auto word = method.substr(0, space);
            auto it = container.lower_bound(word);
            if ( (*it).first == word  ){
                container.at(word)++;
            }
            else {
                if (word!="") {
                    container.at(POLE)++;
                }
            }
            method.remove_prefix(space+1);
        }
    }
}

bool Parse(string_view& line, string& methods) {
    size_t space = line.find(' ');
    if (space==line.npos) {
        methods += line.substr(0, space);
        methods += " ";
        return true;
    }
    else {
        methods += line.substr(0, space);
        methods += " ";
        line.remove_prefix(space+1);
        //cout << line << endl;
    }
    return false;
}
HttpRequest ParseRequest(string_view line) {

    static string methods;
    static string uri;
    static string protocol;
    methods.clear();
    uri.clear();
    protocol.clear();
    while (1) {
        /*size_t space = line.find(' ');
        if (space==line.npos) {
            methods += line.substr(0, space);
            methods += " ";
            break;
        }
        else {
            methods += line.substr(0, space);
            methods += " ";
            line.remove_prefix(space+1);
        }*/
        if (Parse(line, methods)) break;
        if (Parse(line, uri)) break;
        if (Parse(line, protocol)) break;
        /*space = line.find(' ');
        if (space==line.npos) {
            uri += line.substr(0, space);
            uri += " ";
            break;
        }
        else {
            uri += line.substr(0, space);
            uri += " ";
            line.remove_prefix(space+1);
        }*/
        /*size_t space = line.find(' ');
        if (space==line.npos) {
            break;
        }
        else {
            result.protocol = line.substr(0, space);
            line.remove_prefix(space+1);
        }*/
    }
    //HttpRequest result {methods, uri, protocol};
    return {methods, uri, protocol};
}

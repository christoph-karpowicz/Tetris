#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <fstream>
#include <string>
#include <evhttp.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <map>
#include <vector>
#include <tuple>
#include <cassert>
#include "Server.h"
#include "DB.h"

using namespace Database;
using namespace ServerN;

Router::Router() {
    createGETResponses();
};

void Router::createGETResponses() {
    // std::map<string, std::function<void(string)> > GETResponses;
    GETResponses["test"] = []() {
        // Router::GETResponse::setResponse("{test: \"tt\"}");
        Router::GETResponse res;
        res.setResponse("{test: \"tttt\"}");
        return res;
    };
};

string Router::getGETResponse(string getRequest) {
    GETResponse getr = GETResponses[getRequest]();
    return getr.getResponse();
};

// GETResponse nested class
string Router::GETResponse::getResponse() {
    // return Router::GETResponse::response;
    return response;
};

void Router::GETResponse::setResponse(string res) {
    // Router::GETResponse::response = res;
    response = res;
};
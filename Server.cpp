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

std::tuple<std::string, URI_TO_PATH_STATUS> ServerN::uri_to_path(const char *uri) {
    assert(uri != NULL);

    struct evhttp_uri *decodedUri = NULL;
    const char *path = NULL;
    const char *decodedPath = NULL;

    URI_TO_PATH_STATUS status = URI_TO_PATH_STATUS::SUCCESS;

    decodedUri = evhttp_uri_parse(uri);

    if (!decodedUri) {
        status = URI_TO_PATH_STATUS::FAILURE_URI_PARSE;
        goto end;
    }

    path = evhttp_uri_get_path(decodedUri);

    if (!path) {
        status = URI_TO_PATH_STATUS::FAILURE_URI_TO_PATH;
        goto end;
    }

    decodedPath = evhttp_uridecode(path, 0, NULL);

    if (!decodedPath) {
        status = URI_TO_PATH_STATUS::FAILURE_PATH_DECODE;
        goto end;
    }

    end:
    if (decodedUri) {
        evhttp_uri_free(decodedUri);
    }

    auto result = std::make_tuple(
            (status == URI_TO_PATH_STATUS::SUCCESS ? std::string(decodedPath) : std::string("")),
            status
        );

    free((void *)decodedPath);

    return result;
}

Server::Server(std::string addr, std::uint16_t prt): address(addr), port(prt) {

    // createGETResponses();
    // test = "aaaa";

    onReq = [] (evhttp_request *req, void *) {

        Router router;

        const char *uri = evhttp_request_get_uri(req);
        std::tuple<std::string, URI_TO_PATH_STATUS> uriTuple = uri_to_path(uri);

        auto path = std::get<0>(uriTuple);
        auto status = std::get<1>(uriTuple);

        assert(status == URI_TO_PATH_STATUS::SUCCESS);

        struct evbuffer *buffer;
        struct evkeyvalq headers;
        const char *getParam;
        // Parse the query for later lookups
        evhttp_parse_query(uri, &headers);

        DB* db = new DB();
        db->init();
        vector<LeaderboardRecord> leaderboardDBRecords;
        leaderboardDBRecords = {};
        string response = "{ ";

        // getParam = evhttp_find_header(&headers, "test");
        std::cout << path << std::endl;
        std::cout << "Got decoded path " << path << std::endl;
        // printf("%s\n", q);

        auto *OutBuf = evhttp_request_get_output_buffer(req);
        if (!OutBuf)
            return;

        if (path == "/") {

            if (evhttp_find_header(&headers, "test") != NULL) {

                // getParam = evhttp_find_header(&headers, "test");
                // std::cout << "GET " << getParam << std::endl;
                string res = router.getGETResponse("test");
                // std::cout << test << std::endl;
                // GETResponses["test"]();
                evbuffer_add_printf(OutBuf, res.c_str());
                // evbuffer_add_printf(OutBuf, "Server::GETResponse.getResponse()");

            }
            else if (evhttp_find_header(&headers, "getLeaderboard") != NULL) {

                response = response + " \"leaderboard\": [";
                db->select(leaderboardDBRecords);
                // test(leaderboardDBRecords);
                std::sort(leaderboardDBRecords.begin(), leaderboardDBRecords.end(), [](LeaderboardRecord a, LeaderboardRecord b) {
                    return std::stoi(a.score) > std::stoi(b.score);
                });
                for (LeaderboardRecord lr : leaderboardDBRecords) {
                    response = response + " { ";
                    response = response + " \"name\": \"" + lr.name + "\", ";
                    response = response + " \"score\": \"" + lr.score + "\", ";
                    response = response + " \"added\": \"" + lr.added + "\"";
                    // cout << "name: " << lr.name << " | score: " << lr.score << " | added: " << lr.added << endl;
                    response = response + " }, ";
                }
                leaderboardDBRecords.clear();
                // delete leaderboardDBRecords;
                response = response + "\"e\" ] }";
                evbuffer_add_printf(OutBuf, response.c_str());

            }
            else if (evhttp_find_header(&headers, "insertIntoLeaderboard") != NULL) {

                string name = evhttp_find_header(&headers, "name");
                string score = evhttp_find_header(&headers, "score");
                string added = evhttp_find_header(&headers, "added");
                // cout << name << endl;
                db->insert(name, score, added);

                response = "ok";
                evbuffer_add_printf(OutBuf, response.c_str());

            } 
            else {

                /** 
                 * Get the contents of the HTML file.
                */
                string singleLine, fileContent;

                // open a file in read mode.
                ifstream indexFile; 
                indexFile.open("index.html"); 

                while (getline(indexFile, singleLine)) {
                    fileContent.append(singleLine);
                    fileContent.append("\n");
                }

                // close the opened file.
                indexFile.close();

                // Convert HTML string into char array. 
                char fileContentToChar[fileContent.size() + 1];
                fileContent.copy(fileContentToChar, fileContent.size() + 1);
                fileContentToChar[fileContent.size()] = '\0';
                
                evbuffer_add_printf(OutBuf, fileContentToChar);

                evhttp_add_header(evhttp_request_get_output_headers(req),
                    "Content-Type", "text/html");
                // evbuffer_add_printf(OutBuf, "%s", q);

                // evhttp_add_header(evhttp_request_get_output_headers(req),
                //   "Content-Type", "application/wasm");
                
            }

            evhttp_send_reply(req, HTTP_OK, "", OutBuf);

        } 
        else if (path == "/test.js") {

            struct evbuffer *evb = NULL;
            struct stat st;
            evb = evbuffer_new();

            int fd = -1;
            if ((fd = open("test.js", O_RDONLY)) < 0) {
            // perror("open");
            // goto err;
            }

            if (fstat(fd, &st)<0) {
            /* Make sure the length still matches, now that we
            * opened the file :/ */
            // perror("fstat");
            // goto err;
            }

            evbuffer_add_file(evb, fd, 0, st.st_size);  
            evhttp_add_header(evhttp_request_get_output_headers(req),
            "Content-Type", "text/javascript");
            evhttp_send_reply(req, HTTP_OK, "", evb);

        }
        else if (path == "/app.js") {
            struct evbuffer *evb = NULL;
            struct stat st;
            evb = evbuffer_new();

            int fd = -1;
            if ((fd = open("app.js", O_RDONLY)) < 0) {
            // perror("open");
            // goto err;
            }

            if (fstat(fd, &st)<0) {
            /* Make sure the length still matches, now that we
            * opened the file :/ */
            // perror("fstat");
            // goto err;
            }

            evbuffer_add_file(evb, fd, 0, st.st_size);
            evhttp_add_header(evhttp_request_get_output_headers(req),
            "Content-Type", "text/javascript");
            evhttp_send_reply(req, HTTP_OK, "", evb);
        }
        else if (path == "/app.wasm") {
            struct evbuffer *evb = NULL;
            struct stat st;
            evb = evbuffer_new();

            int fd = -1;
            if ((fd = open("app.wasm", O_RDONLY)) < 0) {
            // perror("open");
            // goto err;
            }

            if (fstat(fd, &st)<0) {
            /* Make sure the length still matches, now that we
            * opened the file :/ */
            // perror("fstat");
            // goto err;
            }

            evbuffer_add_file(evb, fd, 0, st.st_size);  
            evhttp_add_header(evhttp_request_get_output_headers(req),
            "Content-Type", "application/wasm");
            evhttp_send_reply(req, HTTP_OK, "", evb);
        }

        delete db;

    };
};

std::string Server::getAddress() const {
    return address;
}

std::uint16_t Server::getPort() const {
    return port;
}

// void Server::createGETResponses() {
//     // std::map<string, std::function<void(string)> > GETResponses;
//     GETResponses["test"] = []() {
//         Server::GETResponse::setResponse("{test: \"tt\"}");
//     };
// };

// // Server::GETResponse Server::getGETResponse(string getRequest) const {
// //     return GETResponses[getRequest]();
// // };

fptr Server::getOnReq() {
    return onReq;
};

// // GETResponse nested class
// string Server::GETResponse::getResponse() {
//     return response;
// };

// void Server::GETResponse::setResponse(string res) {
//     response = res;
// };
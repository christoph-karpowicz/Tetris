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
#include <regex>
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

    const auto result = std::make_tuple(
            (status == URI_TO_PATH_STATUS::SUCCESS ? std::string(decodedPath) : std::string("")),
            status
        );

    free((void *)decodedPath);

    return result;
}

Server::Server(const std::string addr, const std::uint16_t prt): address(addr), port(prt) {

    onReq = [] (evhttp_request *req, void *) {

        const char *uri = evhttp_request_get_uri(req);
        std::tuple<std::string, URI_TO_PATH_STATUS> uriTuple = uri_to_path(uri);

        const auto path = std::get<0>(uriTuple);
        const auto status = std::get<1>(uriTuple);

        assert(status == URI_TO_PATH_STATUS::SUCCESS);

        struct evbuffer *buffer;
        struct evkeyvalq headers;
        const char *getParam;
        // Parse the query for later lookups
        evhttp_parse_query(uri, &headers);

        std::string sPath = path;
        const std::string jsFilePath = "/app.js";
        const std::string wasmFilePath = "/app.wasm";

        std::cout << "Got decoded path " << path << std::endl;

        auto *OutBuf = evhttp_request_get_output_buffer(req);
        if (!OutBuf)
            return;

        if (sPath == "/") {

            Database::DB* db = new Database::DB();
            db->init();

            const Router router(db);

            char* res = nullptr;
            std::map<std::string, std::string> inputData;

            if (evhttp_find_header(&headers, "getLeaderboard") != NULL) {

                res = router("getLeaderboard", inputData);
                evbuffer_add_printf(OutBuf, res);

            }
            else if (evhttp_find_header(&headers, "insertIntoLeaderboard") != NULL) {

                const string name = std::string(evhttp_find_header(&headers, "name"));
                const std::string score = std::string(evhttp_find_header(&headers, "score"));
                const std::string added = std::string(evhttp_find_header(&headers, "added"));
                inputData.insert(pair<std::string, std::string>("name", name));
                inputData.insert(pair<std::string, std::string>("score", score));
                inputData.insert(pair<std::string, std::string>("added", added));
                res = router("insertIntoLeaderboard", inputData);

                evbuffer_add_printf(OutBuf, res);

            } 
            else {

                /** 
                 * Get the contents of the HTML file.
                */
                string singleLine, fileContent;

                // open a file in read mode.
                ifstream indexFile; 
                indexFile.open("client/index.html"); 

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
                
            }

            evhttp_send_reply(req, HTTP_OK, "", OutBuf);
            if (res) delete[] res;
            inputData.clear();

            delete db;

        } else {
            struct evbuffer *evb = NULL;
            struct stat st;
            evb = evbuffer_new();

            // std::regex slash("^\\/");
            // sPath = std::regex_replace(sPath, slash, "");
            sPath = "client" + sPath;

            cout << sPath << endl;

            int fd = -1;
            if ((fd = open(sPath.c_str(), O_RDONLY)) < 0) {
                // perror("open");
                // goto err;
            }

            if (fstat(fd, &st) < 0) {
                /* Make sure the length still matches, now that we
                * opened the file :/ */
                // perror("fstat");
                // goto err;
            }

            evbuffer_add_file(evb, fd, 0, st.st_size);
            if (path == jsFilePath)
                evhttp_add_header(evhttp_request_get_output_headers(req),
                    "Content-Type", "text/javascript");
            if (path == wasmFilePath)
                evhttp_add_header(evhttp_request_get_output_headers(req),
                    "Content-Type", "application/wasm");
            
            evhttp_send_reply(req, HTTP_OK, "", evb);

        }

    };
};

std::string Server::getAddress() const {
    return address;
}

std::uint16_t Server::getPort() const {
    return port;
}

fptr Server::getOnReq() const {
    return onReq;
};
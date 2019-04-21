#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <memory>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <evhttp.h>
#include <cassert>
#include <string>
#include <string.h>
#include <vector>

#include <sqlite3.h>
#include "DB.h"
#include "Callback.h"

#ifdef _WIN32
#ifndef stat
#define stat _stat
#endif
#ifndef fstat
#define fstat _fstat
#endif
#ifndef open
#define open _open
#endif
#ifndef close
#define close _close
#endif
#ifndef O_RDONLY
#define O_RDONLY _O_RDONLY
#endif
#endif

using namespace std;
using namespace Database;
using namespace Callback;

enum class URI_TO_PATH_STATUS : std::int8_t {
  SUCCESS = 0,
  FAILURE_URI_PARSE,
  FAILURE_URI_TO_PATH,
  FAILURE_PATH_DECODE,
};

std::tuple<std::string, URI_TO_PATH_STATUS> uri_to_path(const char *uri) {
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

void test(vector<LeaderboardRecord> &drv) {
  LeaderboardRecord lr;
  lr.name = "aaaa";
  lr.score = "15";
  lr.added = "asdasd";
  drv.push_back(lr);
}

int main()
{
// g++ server.cpp DB.cpp Callback.cpp -levent -l sqlite3

  if (!event_init())
  {
    std::cerr << "Failed to init libevent." << std::endl;
    return -1;
  }
  
  char const SrvAddress[] = "127.0.0.1";
  std::uint16_t SrvPort = 8000;
  std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);
  
  if (!Server)
  {
    std::cerr << "Failed to init http server." << std::endl;
    return -1;
  }
  
  void (*OnReq)(evhttp_request *req, void *) = [] (evhttp_request *req, void *)
  {

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

    if (evhttp_find_header(&headers, "test") != NULL) {

      getParam = evhttp_find_header(&headers, "test");
      std::cout << "GET " << getParam << std::endl;
      evbuffer_add_printf(OutBuf, "{test: \"tt\"}");
      evhttp_send_reply(req, HTTP_OK, "", OutBuf);

    }
    else if (evhttp_find_header(&headers, "getLeaderboard") != NULL) {

      response = response + " \"leaderboard\": [";
      db->select(leaderboardDBRecords);
      // test(leaderboardDBRecords);
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
      evhttp_send_reply(req, HTTP_OK, "", OutBuf);
    }

    else if (evhttp_find_header(&headers, "insertIntoLeaderboard") != NULL) {

      string name = evhttp_find_header(&headers, "name");
      string score = evhttp_find_header(&headers, "score");
      string added = evhttp_find_header(&headers, "added");
      // cout << name << endl;
      db->insert(name, score, added);

      response = "ok";
      evbuffer_add_printf(OutBuf, response.c_str());
      evhttp_send_reply(req, HTTP_OK, "", OutBuf);

    } else {
      if (path == "/test.js") {
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
      else if (path == "/") {

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
        evhttp_send_reply(req, HTTP_OK, "", OutBuf);
        // evbuffer_add_printf(OutBuf, "%s", q);

        // evhttp_add_header(evhttp_request_get_output_headers(req),
        //   "Content-Type", "application/wasm");

      }

    }

    delete db;
        
  };

  evhttp_set_gencb(Server.get(), OnReq, nullptr);
  if (event_dispatch() == -1)
  {
    std::cerr << "Failed to run messahe loop." << std::endl;
    return -1;
  }

  return 0;

}
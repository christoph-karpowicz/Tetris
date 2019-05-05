#include <evhttp.h>
#include <functional>
#include <string>
#include <map>
#include "DB.h"

namespace ServerN {

    enum class URI_TO_PATH_STATUS : std::int8_t {
        SUCCESS = 0,
        FAILURE_URI_PARSE,
        FAILURE_URI_TO_PATH,
        FAILURE_PATH_DECODE,
    };

    std::tuple<std::string, URI_TO_PATH_STATUS> uri_to_path(const char *uri);

    typedef void (*fptr)(evhttp_request *req, void *);

    class Server {
        private:
            const std::string address;
            const std::uint16_t port;
            void (*onReq)(evhttp_request *req, void *);
        public:
            Server(const std::string addr, const std::uint16_t prt);
            fptr getOnReq() const;
            std::string getAddress() const;
            std::uint16_t getPort() const;
    };

    class Router {
        private:
            class GETResponse {
                private:
                    std::string response;
                public:
                    std::string getResponse() const;
                    void setResponse(const std::string res);
            };
            Database::DB *&database;
            mutable std::map<std::string, std::function<Router::GETResponse(Database::DB* db, const std::map<std::string, std::string> &inputData)> > GETResponses;
            void createGETResponses() const;
        public:
            Router(Database::DB *&database);
            char* operator()(const std::string &getRequest, const std::map<std::string, std::string> &inputData) const;

    };

}
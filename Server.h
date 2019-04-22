#include <evhttp.h>
#include <functional>
#include <string>
#include <map>

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
            std::string address;
            std::uint16_t port;
            void (*onReq)(evhttp_request *req, void *);
        public:
            Server(std::string addr, std::uint16_t prt);
            fptr getOnReq();
            std::string getAddress() const;
            std::uint16_t getPort() const;
            // Router::GETResponse getGETResponse(std::string getRequest) const;
    };

    class Router {
        private:
            class GETResponse {
                private:
                    std::string response;
                public:
                    std::string getResponse();
                    void setResponse(std::string res);
            };
            // std::map<std::string, std::function<void()> > GETResponses;
            std::map<std::string, std::function<Router::GETResponse()> > GETResponses;
            void createGETResponses();
        public:
            Router();
            std::string getGETResponse(std::string getRequest);

    };

}
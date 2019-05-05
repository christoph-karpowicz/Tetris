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
#include <algorithm>

#include <sqlite3.h>
#include "Server.h"

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
using namespace ServerN;

int main()
{

  if (!event_init())
  {
    std::cerr << "Failed to init libevent." << std::endl;
    return -1;
  }

  const Server server("127.0.0.1", 8000);
  
  std::unique_ptr<evhttp, decltype(&evhttp_free)> libServer(evhttp_start(server.getAddress().c_str(), server.getPort()), &evhttp_free);
  
  if (!libServer)
  {
    std::cerr << "Failed to init http server." << std::endl;
    return -1;
  }

  evhttp_set_gencb(libServer.get(), server.getOnReq(), nullptr);
  if (event_dispatch() == -1)
  {
    std::cerr << "Failed to run message loop." << std::endl;
    return -1;
  }

  return 0;

}
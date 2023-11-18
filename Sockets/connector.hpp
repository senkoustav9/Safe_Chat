#include "../Utils/util.hpp"
#include "../Headers/server-details.hpp"
using namespace std;

class connector
{
  int sockfd;
  struct sockaddr_in saddr;

public:
  // s_c is 1 for Server and 0 for Client
  connector(bool s_c, uint16_t port, string addr = "") 
  {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (!~sockfd)
    {
      err("Can't create socket.");
    }

    memset(&saddr, 0, sizeof(sockaddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);

    if (s_c)
    {
      saddr.sin_addr.s_addr = htonl(INADDR_ANY);
      int reuse = 1;
      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse)) < 0) 
      {
        perror("setsockopt(SO_REUSEADDR) failed");
      }

#ifdef SO_REUSEPORT
      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (const char *)&reuse, sizeof(reuse)) < 0)
      {
        perror("setsockopt(SO_REUSEPORT) failed");
      }
#endif

      if (!~bind(sockfd, (sockaddr *)&saddr, sizeof saddr))
      {
        err("Can't bind the socket.");
      }
    }

    else if (!~inet_pton(AF_INET, addr.c_str(), &saddr.sin_addr))
    {
      err("Wrong address format.");
    }
  }

  int connectToServer()
  {
    if (!~connect(sockfd, (sockaddr *)&saddr, sizeof saddr))
      err("Cannot connect.");
    return sockfd;
  }

  void listenForClients()
  {
    if (!~listen(sockfd, 10))
      err("Cannot listen.");
    cout << "Waiting for Connections on port: " << PORT << endl;
  }

  int acceptNow()
  {
    int new_fd;
    socklen_t a = sizeof(saddr);
    if (!~(new_fd = accept(sockfd, (sockaddr *)&saddr, (socklen_t *)&a)))
      err("Cannot accept.");
    return new_fd;
  }

  void closeServer()
  {
    close(sockfd);
  }
};
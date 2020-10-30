#include "TcpServer.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    TcpServer* server = new TcpServer();
    server->StartAccept();
    server->StartIO();//启动io_service
    return 0;
}

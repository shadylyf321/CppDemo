/*
 * @Author: your name
 * @Date: 2020-10-29 20:33:25
 * @LastEditTime: 2020-10-30 22:27:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /AsyncTcpServerDemo/TcpServer.hpp
 */
#include <boost/asio.hpp>
#include <set>
#include "TcpConnection.hpp"

using namespace boost::asio;
using namespace std;

class TcpServer
{
public:
    TcpServer() : mAcceptor(mIsService) {}

    ~TcpServer() {}

    void StartAccept()
    {
        mAcceptor.open(ip::tcp::v4());
        mAcceptor.set_option(socket_base::reuse_address(true));

        mAcceptor.bind(ip::tcp::endpoint(ip::tcp::v4(), 3200));
        mAcceptor.listen(socket_base::max_connections);
        AsyncAcceptConn();
        return;
    }

    void StartIO()
    {
        mIsService.run();
    }

private:
    io_service mIsService;
    ip::tcp::acceptor mAcceptor;
    set<TcpConnection*> connections;

    void AsyncAcceptConn()
    {
        cout << "AsyncAcceptConn: " << connections.size() << endl;
        TcpConnection* conn = new TcpConnection(new boost::asio::ip::tcp::socket(mIsService));
        mAcceptor.async_accept(*(conn->Socket),
            boost::bind(&TcpServer::AcceptHandler, this, conn,
                boost::asio::placeholders::error));
    }

    void AcceptHandler(TcpConnection* conn, const boost::system::error_code& error)
    {
        if (error)
        {
            cout << boost::system::system_error(error).what() << endl;
            AsyncAcceptConn();
            return;
        }
        cout << "accept adress:" << conn->Socket->remote_endpoint().address() << endl;
        cout << "accept port:" << conn->Socket->remote_endpoint().port() << endl;
        conn->SetOption();
        conn->DoRecv();
        conn->RegisterCloseHandler(boost::bind(&TcpServer::OnConnectionClose, this, _1));
        connections.insert(conn);
        AsyncAcceptConn();
    }

    //连接断开后回调
    void OnConnectionClose(TcpConnection* conn)
    {
        connections.erase(conn);
        delete conn;
    }
};
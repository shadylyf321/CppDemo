/*
 * @Author: shady lyf
 * @Date: 2020-10-29 20:33:25
 * @LastEditTime: 2020-10-30 22:39:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /AsyncTcpServerDemo/TcpConnection.hpp
 */
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <time.h>
#include <sys/timeb.h>
#include <iostream>
using namespace boost::asio;
using namespace std;

class TcpConnection
{
    typedef boost::function1<void, TcpConnection*> OnCloseHandler;
public:
    ip::tcp::socket *Socket;
    char ReadBuf[1024];
    TcpConnection(ip::tcp::socket *s) : Socket(s) { cout << "construct TcpConnection: " << this << endl;}
    ~TcpConnection() 
    {
        cout << "destroy TcpConnection: " << this << endl;
        delete Socket;
    }

    //异步接受
    void DoRecv()
    {
        cout << "socket:" << Socket << endl;
        async_read((*Socket), buffer(ReadBuf), transfer_at_least(10),
                   boost::bind(&TcpConnection::AsyncRead, this,
                               boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void SetOption()
    {
        boost::asio::ip::tcp::no_delay option(true);
        boost::asio::socket_base::keep_alive option1(true);
        Socket->set_option(option);
        Socket->set_option(option1);
    }

    void RegisterCloseHandler(const OnCloseHandler& closeHandler)
    {
        mOnCloseHandler = closeHandler;
    }

private:
    OnCloseHandler mOnCloseHandler; //连接断开回调

    void AsyncRead(const boost::system::error_code &error, std::size_t readLen)
    {
         time_t now = time(0);
        // 把 now 转换为字符串形式
        char* dt = ctime(&now);
        struct timeb t;
        ftime(&t);
        std::cout << "time: " << t.millitm << "-" << dt   
            << "----len: " << readLen 
            << "  read: " << ReadBuf << std::endl;

        if (error.value() != 0)
        {
            Socket->close();
            std::cout << "an error occur = " << boost::system::system_error(error).what() << std::endl;
            if (mOnCloseHandler)
                mOnCloseHandler(this);
            return;
        }
        DoRecv();
    }
};

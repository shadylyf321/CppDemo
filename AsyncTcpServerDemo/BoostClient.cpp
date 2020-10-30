/*
 * @Author: your name
 * @Date: 2020-10-12 21:24:43
 * @LastEditTime: 2020-10-30 22:36:39
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /AsyncTcpServerDemo/BoostClient.cpp
 */
//code of client
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;
 
int main(int argc, char* argv[])
{
    try
    {
        const int BUFF_SIZE = 24;
        char sendBuf[BUFF_SIZE];
        //通过tcp::socket类定义一个tcp client对象socket
        boost::asio::io_service io;
        tcp::socket socket(io);
        
        //通过connect函数连接服务器，打开socket连接。
        tcp::endpoint end_point(boost::asio::ip::address::from_string("127.0.0.1"), 3200);
        socket.connect(end_point);
        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;
            memset(sendBuf, '\0', sizeof(sendBuf));
            cout << "input send" << endl;
            cin >> sendBuf;
            socket.write_some(boost::asio::buffer(sendBuf, strlen(sendBuf)));
            // socket.write_some(boost::asio::buffer("hello this is Me!!!!!!!"));
            cin.get();
        }
    }
    catch (std::exception& e)
    {
        cout<<e.what()<<endl;
    }
    cin.get();
    return 0;
}
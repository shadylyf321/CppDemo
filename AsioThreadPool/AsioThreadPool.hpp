/*
 * @Author: shady lyf
 * @Date: 2020-10-21 22:24:12
 * @LastEditTime: 2020-10-30 22:11:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /CppTest/AsioThreadPool.hpp
 */
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <thread>
#include <iostream>
#include <chrono>

using IOService = boost::asio::io_service;
using Work = boost::asio::io_service::work;
using WorkPtr = std::unique_ptr<Work>;

using namespace std;

class AsioThreadPool
{
private:
    std::vector<std::shared_ptr<IOService>> mIoServoces;
    std::vector<shared_ptr<std::thread>> mThreads;
    uint mNextIoService;
    uint mCount;
    void WorkFunction(std::shared_ptr<IOService> io, uint index)
    {
        boost::asio::io_service::work work(*io);
        io->run();
    }
public:
    AsioThreadPool(uint num) : mCount(num), mNextIoService(0)
    {
        for (uint i = 0; i < num; ++i)
        {
            shared_ptr<boost::asio::io_service> io =
                make_shared<boost::asio::io_service>();
            shared_ptr<thread> t =
                make_shared<thread>(boost::bind(
                    &AsioThreadPool::WorkFunction,
                    this, io, i));
            mIoServoces.emplace_back(io);
            mThreads.emplace_back(t);
        }
    }
    ~AsioThreadPool()
    {
        for (uint i = 0; i < mCount; ++i)
        {
            mIoServoces[i]->stop();
            mThreads[i]->join();
        }
    }

    template<class F>
    void AddWork(F&& task)
    {
        mIoServoces[mNextIoService]->post(task);
        mNextIoService = (mNextIoService + 1) % mCount;
    }
};



/*
 * @Author: shady lyf
 * @Date: 2020-10-22 22:31:36
 * @LastEditTime: 2020-10-30 22:13:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /CppTest/TestThread.cpp
 */
#include <iostream>
#include "AsioThreadPool.hpp"
#include "boost/function.hpp"
#include <chrono>
using namespace std;

class Base
{
public:
    virtual void Parse(string str) = 0;
};

class A : public Base
{
public:
    A() {}
    ~A() {}
    void Parse(string str) { cout << "Parse A" << endl; }
};

void Work1(int param)
{
    this_thread::sleep_for(chrono::seconds(1));
    cout << "work1: " << param << endl;
}

void Work2(const string &param)
{
    this_thread::sleep_for(chrono::seconds(2));
    cout << "work2: " << param << endl;
}

void Work3(A &param)
{
    this_thread::sleep_for(chrono::seconds(3));
    cout << "work3: " << &param << endl;
}

struct WorkTemplate
{
    template <class T, typename F>
    void operator()(T &param, F &&call)
    {
        this_thread::sleep_for(chrono::seconds(4));
        cout << "WorkTemplate: " << &param << endl;
        param.Parse("");
        call(param);
    }
};

void OnComplete(A &a)
{
    cout << "On Complete" << endl;
}

int main()
{
    AsioThreadPool pool(thread::hardware_concurrency());
    pool.AddWork(boost::bind(Work1, 1));
    pool.AddWork([] { cout << "lambda" << endl; });

    pool.AddWork(boost::bind(Work2, "sss"));
    A a;
    pool.AddWork(boost::bind(Work3, a));
    boost::function<void(A &, boost::function<void(A &)>)> f = WorkTemplate();

    pool.AddWork(boost::bind(f, a, &OnComplete));
    this_thread::sleep_for(chrono::seconds(5));
    cin.get();
    return 1;
}
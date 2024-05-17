#pragma once
#include<queue>
#include<iostream>
#include<vector>
#include<mutex>
#include<string>
#include<thread>
#include<condition_variable>
#include<functional>
using namespace std;

#define newThread(a)\
    ThreadPool::getInstance()->enqueue([=]{a;})

class ThreadPool
{
    private:
    static ThreadPool* instance_;
    public:
    static ThreadPool* getInstance()
    {
        return instance_;
    }
    static void deleteInstance()
    {
        if(instance_)
        {
            delete instance_;
            instance_=NULL;
        }
    }
    ThreadPool(int num):stop(false)
    {
        for(int i = 0; i < num;i++)
        {
            threads.emplace_back([=]
            {
                while(1)
                {
                    unique_lock<mutex> lock(mtx);
                    //  lamda
                    //  if true will go, false will wait
                    // 此处，在lambda表达式中this等同于=（以值传递形式捕获父作用域的所有变量）
                    condition.wait(lock,[this]
                    {
                        return !tasks.empty() || stop;
                    });
                    if(stop && tasks.empty())
                    {
                        return;
                    }
                    // move
                    function<void()> task(move(tasks.front()));
                    tasks.pop();
                    lock.unlock();
                    task();
                }
            });

        }
    }
    ~ThreadPool()
    {
        {
        unique_lock<mutex> lock(mtx);
        stop=true;
        }
        condition.notify_all();
        for(auto& t:threads)
        {
            t.join();
        }
    }

    template<class F, class... Args> void enqueue(F&& f,Args&&... args)
    {
        auto task=
            bind(forward<F>(f),forward<Args>(args)...);
        {
        unique_lock<mutex> lock(mtx);
        tasks.emplace(move(task));
        }
        condition.notify_one();
    }
private:
    vector<thread> threads;
    queue<function<void()>> tasks;

    mutex mtx;
    condition_variable condition;

    bool stop;
};

ThreadPool* ThreadPool::instance_ = new ThreadPool(128);
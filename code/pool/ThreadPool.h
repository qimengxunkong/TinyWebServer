#pragma once
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <future>
#include <chrono>
class ThreadPool
{
public:
    ThreadPool(int ThreadNum)
    {
        _pool = std::make_shared<Pool>();
        _pool->_isClosethp = false;
        for (int i = 0;i<ThreadNum;i++)
        {
            std::thread th([pool = _pool] (){
                std::unique_lock<std::mutex> loker(pool->_mtx);
                while (true)
                {
                    if (!pool->_isClosethp && !pool->_taskque.empty())
                    {
                        auto task = pool->_taskque.front();
                        pool->_taskque.pop();
                        loker.unlock();
                        (*task)();
                        loker.lock();
                    }
                    else if (pool->_isClosethp) break;
                    else pool->_cond.wait(loker);
                }

            });
            th.detach();
        }
    }


    template<typename T_Fn, typename... T_Args>
    auto AddTask(T_Fn&& fn, T_Args&&... args)  -> std::future<decltype(fn(args...))>
    {
        std::unique_lock<std::mutex> loker(_pool->_mtx);

        auto task = std::make_shared<std::packaged_task<decltype(fn(args...))()>>( std::bind(std::forward<T_Fn>(fn), std::forward<T_Args>(args)...));
        std::future<decltype(fn(args...))> future = task->get_future();
        //std::function<void(void)> func = std::move(*task);
        //std::shared_ptr<std::function<void(void)>> funptr(std::move(func));
        std::shared_ptr<std::function<void()>> func = std::make_shared<std::function<void()>>([QueTask = task]()
        {
            (*QueTask)();
        });
        _pool->_taskque.emplace(func);
        loker.unlock();

        _pool->_cond.notify_one();
        //std::cout << "AddTask Return Value:" << future.get() << std::endl;
        return future;
    }


    ~ThreadPool()
    {
        std::unique_lock<std::mutex> loker(_pool->_mtx);
        _pool->_isClosethp = true;
        _pool->_cond.notify_all();
    }

private:
    struct Pool
    {
        std::mutex _mtx;
        std::condition_variable _cond;
        std::queue<std::shared_ptr<std::function<void()>>> _taskque;
        bool _isClosethp;
    };
    std::shared_ptr<Pool> _pool;

};
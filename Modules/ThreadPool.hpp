#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <condition_variable>

    namespace Thread
    {

        class ThreadPool
        {
        public:
            ThreadPool(size_t numThreads = 10) : stop(false)
            {
                for (size_t i = 0; i < numThreads; ++i)
                 {
                    threads.emplace_back([this]
                                         {
                    while (true) {
                        std::function<void()> task;

                        {
                            std::unique_lock<std::mutex> lock(mutex);
                            condition.wait(lock, [this] { return stop || !tasks.empty(); });

                            if (stop && tasks.empty()) {
                                return;
                            }

                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        task();
                    } });
                }
            }

            ~ThreadPool()
            {
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    stop = true;
                }

                condition.notify_all();

                for (std::thread &thread : threads)
                {
                    thread.join();
                }
            }

            template <class F, class... Args>
            void enqueue(F f,  Args &&...args)
            {
                {
                    std::unique_lock<std::mutex> lock(mutex);

                    tasks.emplace([=]
                                  { std::invoke(f, std::forward<Args>(args)...); });
                }

                condition.notify_one();
            }

        private:
            std::vector<std::thread> threads;
            std::queue<std::function<void()>> tasks;
            std::mutex mutex;
            std::condition_variable condition;
            bool stop;
        };

        template<typename T>
        class ThreadSafeQueue {
            public:
                ThreadSafeQueue() = default;
                ThreadSafeQueue(ThreadSafeQueue<T> &&) = delete;
                ~ThreadSafeQueue()
                {
                    clear();
                }

                const T& front(void)
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    return dequeue.front();
                }
                
                const T& back(void)
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    return dequeue.back();
                }

                void push_back(const T& elem)
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    dequeue.push_back(elem);
                    std::unique_lock<std::mutex> clock(condVarMutex);
                    condVar.notify_one();
                }

                void push_front(const T& elem)
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    dequeue.push_front(elem);
                    std::unique_lock<std::mutex> clock(condVarMutex);
                    condVar.notify_one();
                }

                int size(void)
                { 
                    std::unique_lock<std::mutex> lock(mutex);
                    return dequeue.size();
                }

                bool empty(void)
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    return dequeue.empty();
                }

                void clear(void)
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    dequeue.clear();
                }
                
                bool contain(const T& elem)
                {
                    return !(std::find(dequeue.begin(), dequeue.end(), elem) == dequeue.end());
                }

                T pop_front(void)
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    T elem = dequeue.front();
                    dequeue.pop_front();
                    return elem;
                }

                T pop_back(void)
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    T elem = dequeue.back();
                    dequeue.pop_back();
                    return elem;
                }

                void wait()
                {
                    while(empty()) {
                        std::unique_lock<std::mutex> ul(condVarMutex);
                        condVar.wait(ul);
                    }
                }

            public:
                std::mutex mutex;
                std::deque<T> dequeue;
                std::condition_variable condVar;
                std::mutex condVarMutex;
                std::condition_variable condition;
        };

    }
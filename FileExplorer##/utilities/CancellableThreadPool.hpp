#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

class ThreadPool
{
    std::vector<std::jthread>                 workers;
    std::deque<std::move_only_function<void()>> queue;
    std::mutex                                  mutex;
    std::condition_variable                        cv;
    bool                                         stop = false;

public:
    ThreadPool(uint32_t threads)
    {
        for (uint32_t i = 0; i < threads; ++i)
        {
            workers.emplace_back([this] {
                for (;;)
                {
                    std::move_only_function<void()> task;

                    {
                        std::unique_lock lock{mutex};
                        cv.wait(lock, [&] { return stop || !queue.empty(); });
                        if (stop && queue.empty())
                            return;

                        task = std::move(queue.front());
                        queue.pop_front();
                    }

                    task();
                }
                });
        }
    }

    ~ThreadPool()
    {
        {
            std::scoped_lock lock{mutex};
            stop = true;
        }
        cv.notify_all();
    }

    template<class Fn, class... Args>
    auto enqueue(Fn&& f, Args&&... args)
    {
        using return_type = typename std::invoke_result_t<Fn, Args...>;
        auto task = std::packaged_task<return_type()>(
            std::bind(std::forward<Fn>(f), std::forward<Args>(args)...));

        auto res = task.get_future();

        {
            std::scoped_lock lock{mutex};
            queue.push_back([task = std::move(task)]() mutable { task(); });
        }
        cv.notify_one();

        return res;
    }
};
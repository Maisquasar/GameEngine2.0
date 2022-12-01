#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
namespace Utils
{
	class ThreadManager
	{
	public:
		ThreadManager();
		~ThreadManager();
		std::vector<std::thread> Threads;
		template <typename F, typename... A> void QueueJob(F&& task, A&&... args)
		{
			std::function<void()> task_function = std::bind(std::forward<F>(task), std::forward<A>(args)...);
			{
				_tasks.push(task_function);
			}
			++_totalTasks;
		}
		void ThreadLoop();
		void Lock();
		void Unlock();
		void Terminate() { _terminate = true; }
	private:
		std::queue<std::function<void()>> _tasks;
		std::mutex _mutex;
		std::atomic<int> _totalTasks = 0;
		size_t _threadsAvaible;
		bool _terminate = false;

	};
}
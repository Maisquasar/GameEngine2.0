#include "Include/Utils/ThreadManager.h"
#include "Include/Debug/Log.h"

Utils::ThreadManager::ThreadManager()
{
#if MULTITHREAD_LOADING
	_threadsAvaible = std::thread::hardware_concurrency();
	Threads.resize(_threadsAvaible);
	for (uint32_t i = 0; i < _threadsAvaible; i++) {
		Threads.at(i) = std::thread(&ThreadManager::ThreadLoop, this);
	}
#endif
}

Utils::ThreadManager::~ThreadManager()
{
#if MULTITHREAD_LOADING
	for (int i = 0; i < this->Threads.size(); i++)
		Threads[i].join();
#endif
}

void Utils::ThreadManager::ThreadLoop()
{
#if MULTITHREAD_LOADING
	while (!_terminate)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (!_tasks.empty())
		{
			Lock();
			if (!_tasks.empty()) {
				if (auto task = _tasks.front()) {
					_tasks.pop();
					Unlock();
					if (task != nullptr)
						task();
				}
				else {
					_tasks.pop();
					Unlock();
				}
				break;
			}
			Unlock();
		}
	}
	if (_terminate)
	{
		return;
	}
	ThreadLoop();
#endif
}

void Utils::ThreadManager::Lock()
{
	_mutex.lock();
}
void Utils::ThreadManager::Unlock()
{
	_mutex.unlock();
}
#include "Include/Utils/ThreadManager.h"

Utils::ThreadManager::ThreadManager()
{
	_threadsAvaible = std::thread::hardware_concurrency();
	Threads.resize(_threadsAvaible);
	for (uint32_t i = 0; i < _threadsAvaible; i++) {
		Threads.at(i) = std::thread(&ThreadManager::ThreadLoop, this);
	}
}

Utils::ThreadManager::~ThreadManager()
{
	for (int i = 0; i < this->Threads.size(); i++)
		Threads[i].join();
}

void Utils::ThreadManager::ThreadLoop()
{
	while (!_terminate)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (!_tasks.empty())
		{
			Lock();
			if (!_tasks.empty()) {
				auto task = _tasks.front();
				_tasks.pop();
				Unlock();
				if (task != nullptr)
					task();
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
}

void Utils::ThreadManager::Lock()
{
	_mutex.lock();
}
void Utils::ThreadManager::Unlock()
{
	_mutex.unlock();
}
#pragma once

#include <thread>
#include <array>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
	ThreadPool()
	{
		for (int id = 0; id < poolSize; id++) // Launch workers
			workers[id] = std::thread([this]() { workerTask(); });
	}

	~ThreadPool()
	{
		std::unique_lock<std::mutex> lock(queueMtx);

		stop = true; // Notify workers they have to stop
		condition.notify_all();

		for (int id = 0; id < poolSize; id++) // Kill workers thread
			workers[id].join();
	}

	template <class T>
	void addToQueue(T&& func, const std::string& _name)
	{
		std::unique_lock<std::mutex> lock(queueMtx);
		// Add the task to the queue
		tasksQueue.emplace(std::forward<T>(func));
		Log::Print("Task %s added to Queue.", _name.c_str());
		// Notify workers that one new task is available
		condition.notify_one();
	}

private:
	static const unsigned int poolSize = 8;

	std::array<std::thread, poolSize> workers;
	std::queue<std::function<void()>> tasksQueue;

	std::mutex queueMtx;
	std::condition_variable condition;

	bool stop = false;

	void workerTask()
	{
		while (true)
		{
			std::function<void()> task;

			std::unique_lock<std::mutex> lock(queueMtx);
			// Wait until there is a task in the queue or the thread should stop
			condition.wait(lock, [this] { return stop || !tasksQueue.empty(); });

			if (stop && tasksQueue.empty())
				return; // Exit the thread if it's time to stop

			// Get the next task from the queue
			task = std::move(tasksQueue.front());
			tasksQueue.pop();

			task();
		}
	}
};
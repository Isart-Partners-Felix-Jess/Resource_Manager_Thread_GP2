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
		for (int id = 0; id < s_m_poolSize; id++) // Launch workers
			m_workers[id] = std::thread([this]() { WorkerTask(); });
	}

	~ThreadPool()
	{
		std::unique_lock<std::mutex> lock(m_queueMtx);

		m_stop = true; // Notify workers they have to stop
		m_waitCondition.notify_all();

		for (int id = 0; id < s_m_poolSize; id++) // Kill workers thread
			m_workers[id].join();
	}

	template <class T>
	void AddToQueue(T&& _func, const std::string& _name)
	{
		std::unique_lock<std::mutex> lock(m_queueMtx);
		// Add the task to the queue
		m_tasksQueue.emplace(std::forward<T>(_func));
		Log::Print("Task %s added to Queue.", _name.c_str());
		// Notify workers that one new task is available
		m_waitCondition.notify_one();
	}

private:
	static const unsigned int s_m_poolSize = 20;

	std::array<std::thread, s_m_poolSize> m_workers;
	std::queue<std::function<void()>> m_tasksQueue;

	std::mutex m_queueMtx;
	std::condition_variable m_waitCondition;

	bool m_stop = false;

	void WorkerTask()
	{
		while (true)
		{
			std::function<void()> task;

			std::unique_lock<std::mutex> lock(m_queueMtx);
			// Wait until there is a task in the queue or the thread should stop
			m_waitCondition.wait(lock, [this] { return m_stop || !m_tasksQueue.empty(); });

			if (m_stop && m_tasksQueue.empty())
				return; // Exit the thread if it's time to stop

			// Get the next task from the queue
			task = std::move(m_tasksQueue.front());
			m_tasksQueue.pop();

			task();
		}
	}
};
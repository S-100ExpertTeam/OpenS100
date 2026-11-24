#include "stdafx.h"
#include "DatasetOpening.h"

#include <condition_variable>
#include <deque>
#include <thread>
#include <utility>

namespace
{
	// The one thread datasets are opened on, started with the first task and stopped when the
	// process ends.
	class Worker
	{
	public:
		~Worker()
		{
			{
				const std::lock_guard<std::mutex> lock(mutex);
				stopping = true;
				tasks.clear();
			}
			wake.notify_all();
			if (thread.joinable())
			{
				thread.join();
			}
		}

		void Run(std::function<void()> task)
		{
			{
				const std::lock_guard<std::mutex> lock(mutex);
				if (stopping)
				{
					return;
				}
				tasks.push_back(std::move(task));
				if (!thread.joinable())
				{
					thread = std::thread([this]() { Work(); });
				}
			}
			wake.notify_all();
		}

		void WaitUntilIdle()
		{
			std::unique_lock<std::mutex> lock(mutex);
			idle.wait(lock, [this]() { return stopping || (tasks.empty() && !busy); });
		}

	private:
		void Work()
		{
			for (;;)
			{
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(mutex);
					wake.wait(lock, [this]() { return stopping || !tasks.empty(); });
					if (stopping)
					{
						return;
					}
					task = std::move(tasks.front());
					tasks.pop_front();
					busy = true;
				}
				task();
				task = nullptr;   // what it holds is let go before anyone is told it is done
				{
					const std::lock_guard<std::mutex> lock(mutex);
					busy = false;
				}
				idle.notify_all();
			}
		}

		std::mutex mutex;
		std::condition_variable wake;
		std::condition_variable idle;
		std::deque<std::function<void()>> tasks;
		std::thread thread;
		bool busy = false;
		bool stopping = false;
	};

	Worker& TheWorker()
	{
		static Worker worker;
		return worker;
	}

	std::mutex& ReadyLock()
	{
		static std::mutex lock;
		return lock;
	}

	// What the application asked to be called; empty while background opening is off.
	std::shared_ptr<const GISLibrary::BackgroundOpening::Ready>& ReadyCall()
	{
		static std::shared_ptr<const GISLibrary::BackgroundOpening::Ready> call;
		return call;
	}
}

namespace GISLibrary
{
	bool OpensInBackground(const std::wstring& path)
	{
		const auto dot = path.find_last_of(L'.');
		if (dot == std::wstring::npos || path.size() - dot != 4)
		{
			return false;
		}
		for (auto i = dot + 1; i < path.size(); i++)
		{
			if (path[i] < L'0' || path[i] > L'9')
			{
				return false;
			}
		}
		return true;
	}

	std::recursive_mutex& OpeningLock()
	{
		static std::recursive_mutex lock;
		return lock;
	}

	void BackgroundOpening::Enable(Ready ready)
	{
		const std::lock_guard<std::mutex> lock(ReadyLock());
		ReadyCall() = std::make_shared<const Ready>(std::move(ready));
	}

	void BackgroundOpening::Disable()
	{
		{
			const std::lock_guard<std::mutex> lock(ReadyLock());
			ReadyCall().reset();
		}
		// What was asked for before is still opened; nobody is told.
		WaitUntilIdle();
	}

	bool BackgroundOpening::Enabled()
	{
		const std::lock_guard<std::mutex> lock(ReadyLock());
		return ReadyCall() != nullptr;
	}

	void BackgroundOpening::Run(std::function<void()> task)
	{
		TheWorker().Run(std::move(task));
	}

	void BackgroundOpening::NotifyReady()
	{
		std::shared_ptr<const Ready> call;
		{
			const std::lock_guard<std::mutex> lock(ReadyLock());
			call = ReadyCall();
		}
		if (call && *call)
		{
			(*call)();
		}
	}

	void BackgroundOpening::WaitUntilIdle()
	{
		TheWorker().WaitUntilIdle();
	}
}

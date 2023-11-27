#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <iomanip>


enum class LogLevel {
    Error,
    Debug,
    General
};

class Logger {
public:
    using CallbackType = std::function<void(const std::string&, LogLevel)>;

    static Logger& GetInstance() {
        static Logger instance;
        return instance;
    }

    void SetFormattingLog(bool res)
    {
        Formatted = res;
    }

    bool GetFormattingLog()
    {
        return Formatted;
    }

    void Subscribe(LogLevel level, CallbackType callback, const std::string& id) {
        std::unique_lock<std::mutex> lock(callbackMutex);
        callbacks[level][id] = callback;
    }

    void Unsubscribe(LogLevel level, const std::string& id) {
        std::unique_lock<std::mutex> lock(callbackMutex);
        if (callbacks[level].find(id) != callbacks[level].end()) {
            callbacks[level].erase(id);
        }
    }

    void Log(const std::string& message, LogLevel level = LogLevel::General) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (logQueue.size() < maxQueueSize) {
                logQueue.push(std::make_pair(message + "\n", level));
                newLogAdded = true;
            }
            else {
                std::cerr << "Log queue is full. Dropping log messages." << std::endl;
            }
        }
        cv.notify_one();
    }

    void ErrorLog(const std::string& message, const std::string& file, int line) {
        std::string formattedMessage = Formatted ? FormatLogMessage("Error", message, file, line) : message;
        Log(formattedMessage, LogLevel::Error);
    }

    void DebugLog(const std::string& message, const std::string& file, int line) {
        std::string formattedMessage = Formatted ? FormatLogMessage("Debug", message, file, line) : message;
        Log(formattedMessage, LogLevel::Debug);
    }

private:
    Logger() : exitFlag(false), newLogAdded(false) {
        workerThread = std::thread(&Logger::ProcessLogs, this);
    }

    ~Logger() {
        exitFlag.store(true);
        cv.notify_one();
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }

    static const size_t maxQueueSize = 1000;
    std::queue<std::pair<std::string, LogLevel>> logQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::thread workerThread;
    std::atomic<bool> exitFlag;
    std::atomic<bool> newLogAdded;
    std::map<LogLevel, std::map<std::string, CallbackType>> callbacks;
    std::mutex callbackMutex;
    bool Formatted = true;

    void ProcessLogs() {
        std::vector<std::pair<std::string, LogLevel>> buffer;

        try {
            while (true) {
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    cv.wait(lock, [this] { return newLogAdded || exitFlag.load(); });
                    newLogAdded = false;

                    buffer.clear();
                    buffer.reserve(logQueue.size());
                    while (!logQueue.empty()) {
                        buffer.push_back(std::move(logQueue.front()));
                        logQueue.pop();
                    }
                }

                for (const auto& [message, level] : buffer) {
                    try {
                        NotifySubscribers(message, level);
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Log processing error: " << e.what() << std::endl;
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Exception in log processing: " << e.what() << std::endl;
        }
    }

    void NotifySubscribers(const std::string& message, LogLevel level) {
        std::map<std::string, CallbackType> callbacksCopy;
        {
            std::unique_lock<std::mutex> lock(callbackMutex);
            callbacksCopy = callbacks[level]; 
        } 

        for (const auto& callbackPair : callbacksCopy) {
            if (callbackPair.second) {
                callbackPair.second(message, level);
            }
        }
    }

    std::string FormatLogMessage(const std::string& level, const std::string& message, const std::string& file, int line) {
        std::ostringstream oss;
        oss << "[" << GetCurrentTime() << "] [" << level << "] - [" << file << ":" << line << "] " << message;
        return oss.str();
    }

    std::string GetCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::tm tm_buf;
        char buf[80];
        localtime_s(&tm_buf, &in_time_t);
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_buf);

        return std::string(buf);
    }
};

#define LOG(message) Logger::GetInstance().Log(message)
#define LOG_ERROR(message) Logger::GetInstance().ErrorLog(message, __FILE__, __LINE__)
#define LOG_DEBUG(message) Logger::GetInstance().DebugLog(message, __FILE__, __LINE__)
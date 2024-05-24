#include <iostream>
#include <fstream>
#include <queue>
#include <semaphore>
#include <thread>

enum LogLevel
{
    ERROR,
    WARNING,
    INFO,
    DEBUG
};

class LogMessage
{
    friend class Logger;
    LogLevel Level;
    std::string Message;
    time_t Timestamp;
    pid_t PID;
    std::thread::id TID;

public:
    LogMessage(LogLevel, std::string);
};

class Logger
{
    std::ofstream outFile;
    std::string FileName;
    
    std::counting_semaphore<1024> LogSemaphore{0};
    std::queue<LogMessage> LogQueue;
    
public:
    Logger();
    Logger(std::string);

    void LoggerMainThread();
    Logger& operator<<(LogMessage);
    Logger& operator<<(std::string);
};
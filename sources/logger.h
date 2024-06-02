#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <fstream>
#include <queue>
#include <semaphore>
#include <thread>

//Enumerate level of log messages
enum LogLevel
{
    CRITICAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG
};

//Structure of logging message
class LogMessage
{
    friend class Logger;
    LogLevel Level;
    std::string Message;
    time_t Timestamp;
    pid_t PID;
    std::thread::id TID;

public:
    LogMessage(const LogLevel, const std::string);
};

//Logger with its' file, semaphore and queue
class Logger
{
    std::ofstream outFile;
    std::string FileName;
    
    std::counting_semaphore<1024> LogSemaphore{0};
    std::queue<LogMessage> LogQueue;
    
public:
    Logger();
    Logger(const std::string);

    void LoggerMainThread();
    Logger& operator<<(const LogMessage);
    Logger& operator<<(const std::string);
};

void InitLogger(Logger *Logs);

#endif
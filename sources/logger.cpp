#include "logger.h"

//Function to start logger in another thread
void InitLogger(Logger* Logs)
{
    Logs -> LoggerMainThread();
}

//Creates logging message with timestamp and PID with TID
LogMessage::LogMessage(const LogLevel InputLevel, const std::string InputMsg)
{
    Timestamp = time(nullptr);
    Level = InputLevel;
    Message = InputMsg;
    PID = getpid();
    TID = std::this_thread::get_id();
}

//Creates log file by default name
Logger::Logger() : Logger::Logger("RedisLogs")
{
    
}

//Inits log file
Logger::Logger(const std::string InputFileName)
{
    FileName = InputFileName;
    //Clear previous file
    outFile.open(FileName, std::ios::out);
    outFile.close();
    *this << "Logger inited";
}

//Puts log message in queue and signal through semaphore to another thread
Logger& Logger::operator<<(const LogMessage InputLog)
{
    LogQueue.push(InputLog);
    LogSemaphore.release();
    return *this;
}

//Create message by string with default level "INFO"
Logger& Logger::operator<<(const std::string InputMsg)
{
    *this << LogMessage(INFO, InputMsg);
    return *this;
}

//Main thread of logger. Waiting for semaphore and writes logs to file in format
void Logger::LoggerMainThread()
{
    outFile.open(FileName, std::ios::app);
    while(1)
    {
        if (!LogSemaphore.try_acquire())
        {
            outFile.close();
            LogSemaphore.acquire();
            outFile.open(FileName, std::ios::app);
        }
        if (!LogQueue.empty() && outFile.is_open())
        {
            LogMessage Current = LogQueue.front();

            struct tm* timeinfo = localtime(&Current.Timestamp);
            char buffer[30];
            strftime(buffer, 30, "[%d.%m.%Y %H:%M:%S ", timeinfo);

            outFile << buffer;
            outFile << Current.PID << "." << Current.TID << "] ";
            if (Current.Level == CRITICAL)
                outFile << "[CRITICAL]";
            else if (Current.Level == ERROR)
                outFile << "[ERROR]";
            else if (Current.Level == WARNING)
                outFile << "[WARN ]";
            else if (Current.Level == INFO)
                outFile << "[INFO ]";
            else if (Current.Level == DEBUG)
                outFile << "[DEBUG]";
            outFile << " " << Current.Message << std::endl;

            LogQueue.pop();
        }
    }
}
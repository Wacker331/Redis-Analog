#include "logger.h"
void InitLogger(Logger* Logs)
{
    Logs -> LoggerMainThread();
}

LogMessage::LogMessage(LogLevel InputLevel, std::string InputMsg)
{
    Timestamp = time(nullptr);
    Level = InputLevel;
    Message = InputMsg;
    PID = getpid();
    TID = std::this_thread::get_id();
}

Logger::Logger() : Logger::Logger("RedisLogs")
{

}

Logger::Logger(std::string InputFileName)
{
    FileName = InputFileName;
    //Clear previous file
    outFile.open(FileName, std::ios::out);
    outFile.close();
    *this << "Logger inited";

    // std::thread Thread(InitLogger, this);
}

Logger& Logger::operator<<(LogMessage InputLog)
{
    LogQueue.push(InputLog);
    LogSemaphore.release();
    return *this;
}

Logger& Logger::operator<<(std::string InputMsg)
{
    *this << LogMessage(INFO, InputMsg);
    return *this;
}

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
            if (Current.Level == ERROR)
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
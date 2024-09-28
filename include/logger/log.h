#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>

namespace logger
{
    class AsyncFileWriter
    {
    public:
        AsyncFileWriter(const std::string &message, const std::string &filename);

        ~AsyncFileWriter();

    private:
        std::string filename_;
        std::string message_;
        std::thread writeThread_;
        std::mutex fileMutex;

        void writeMessageToFile();
    };

    class Logger
    {
    public:
        void log(const std::string &message);
    };
} // namespace logger

#endif
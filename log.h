#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>

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
    // {
    //     std::lock_guard<std::mutex> lock(fileMutex);

    //     std::ofstream outFile;
    //     outFile.open(filename_, std::ios::app); // Open file in append mode
    //     if (outFile.is_open())
    //     {
    //         outFile << message_ << std::endl; // Write message to file
    //         outFile.close();                  // Close the file
    //     }
    //     else
    //     {
    //         std::cerr << "Error opening file!" << std::endl;
    //     }
    // }
};

class Logger
{
public:
    void log(const std::string &message);
};

#endif
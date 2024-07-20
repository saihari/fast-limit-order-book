#include "log.h"
#include <thread>

AsyncFileWriter::AsyncFileWriter(const std::string &message, const std::string &filename)
    : filename_(filename), message_(message), writeThread_(&AsyncFileWriter::writeMessageToFile, this)
{
}

AsyncFileWriter::~AsyncFileWriter()
{
    if (writeThread_.joinable())
    {
        writeThread_.join(); // Ensure the thread has completed
    }
}

void AsyncFileWriter::writeMessageToFile()
{
    std::lock_guard<std::mutex> lock(fileMutex);

    std::ofstream outFile;
    outFile.open(filename_, std::ios::app); // Open file in append mode
    if (outFile.is_open())
    {
        outFile << message_ << std::endl; // Write message to file
        outFile.close();                  // Close the file
    }
    else
    {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void Logger::log(const std::string &message)
{
    // Create an AsyncFileWriter instance to write the message
    AsyncFileWriter writer(message, "output.txt");
    // The AsyncFileWriter will handle writing in a separate thread
}
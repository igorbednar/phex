#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

constexpr int NUM_OF_BYTES_PER_LINE = 16;
constexpr int CHUNK_SIZE = 4096;

void printBufferInHex(const std::vector<unsigned char> &buffer, int &byteCount)
{
    for (const auto &byte : buffer)
    {
        std::cout << std::setw(2) << static_cast<int>(byte) << ' ';
        byteCount++;
        if (byteCount == NUM_OF_BYTES_PER_LINE)
        {
            std::cout << std::endl;
            byteCount = 0;
        }
    }
}

void printFileInHex(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    std::cout << std::hex << std::setfill('0');
    std::vector<unsigned char> buffer(CHUNK_SIZE);
    int byteCount = 0;
    bool dataRead = true;
    file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
    while (file.gcount())
    {
        buffer.resize(file.gcount());
        printBufferInHex(buffer, byteCount);
        buffer.resize(CHUNK_SIZE);
        file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
    }

    std::cout << std::endl;
}

void printUsage(std::string_view programName)
{
    std::cerr << "Usage: " << programName << " <file name>" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printUsage(argv[0]);
        return 1;
    }

    try
    {
        printFileInHex(argv[1]);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}
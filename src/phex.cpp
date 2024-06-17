#include <iostream>
#include <fstream>
#include <iomanip>

constexpr int NUM_OF_BYTES_PER_LINE = 16;

bool printFileAsHex(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    std::cout << std::hex << std::setfill('0');
    unsigned char byte;
    int byteCount = 0;
    while (file.read(reinterpret_cast<char *>(&byte), sizeof(byte)))
    {
        std::cout << std::setw(2) << static_cast<int>(byte) << ' ';
        byteCount++;
        if (byteCount == NUM_OF_BYTES_PER_LINE)
        {
            std::cout << std::endl;
            byteCount = 0;
        }
    }

    std::cout << std::endl;
    return true;
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
        printFileAsHex(argv[1]);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}
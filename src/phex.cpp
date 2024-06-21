#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

constexpr int NUM_OF_BYTES_PER_LINE = 16;
constexpr int CHUNK_SIZE = 4096;
constexpr int ADDRESS_SIZE = 8; // 4*8 = 32 bits

bool isPrintable(unsigned char byte)
{
    return byte >= 0x20 && byte <= 0x7E;
}

void printRemainingTextBuffer(const std::vector<unsigned char> &buffer, std::vector<unsigned char> &text)
{
    if (buffer.size() != CHUNK_SIZE)
    {
        for (int i = buffer.size() % NUM_OF_BYTES_PER_LINE; i < NUM_OF_BYTES_PER_LINE; i++)
        {
            std::cout << "   ";
        }
        std::cout << " " << text.data() << std::endl;
    }
}

void printBufferInHex(const std::vector<unsigned char> &buffer, unsigned int &address)
{
    std::vector<unsigned char> printable(NUM_OF_BYTES_PER_LINE);
    for (const auto &byte : buffer)
    {
        auto row = address % NUM_OF_BYTES_PER_LINE;
        if (row == 0)
        {
            std::cout << std::setw(ADDRESS_SIZE) << address << ": ";
        }

        std::cout << std::setw(2) << static_cast<int>(byte) << " ";
        printable[row] = '.';

        if (isPrintable(byte))
        {
            printable[row] = byte;
        }
        address++;

        if (address % NUM_OF_BYTES_PER_LINE == 0)
        {
            std::cout << " " << printable.data() << std::endl;
            printable.assign(NUM_OF_BYTES_PER_LINE, ' ');
        }
    }

    printRemainingTextBuffer(buffer, printable);
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
    unsigned int address = 0;
    file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
    while (file.gcount())
    {
        buffer.resize(file.gcount());
        printBufferInHex(buffer, address);
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
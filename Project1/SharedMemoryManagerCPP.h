#pragma once
#include <string>
// #pragma comment(lib, "user32.lib")

class SharedMemoryLocation {
private:
    std::string fileName;
    unsigned fileSize;
    void* handler;
    void* data;
    // 0 not determined 1 is owner 2 is not owner
    char owner;
    wchar_t* toCharArray(const std::string& txt);
    // true if has a name and valid buffer size
    bool verify();
public:
    // initalizes class members to default or nullptr
    SharedMemoryLocation();

    // initalizes class members with specified values
    // bufferSize in bytes
    SharedMemoryLocation(const std::string& fileName, const unsigned int& fileSize);

    ~SharedMemoryLocation();

    // creates a shared file
    bool createFile();

    // opens a current shared file
    bool openFile();

    // gets the data stored in the shared file if it exists
    void* getData();

    // the data to be stored
    // the size of that data eg 'sizeof(T) * lenghtOfData'
    // offset in bytes
    void writeToFile(void* data, const unsigned int& dataSize, const unsigned int& offset);

    // closes the file should it exist
    void closeFile();

    // true is this object created the shared memory
    bool isCreator();

    // returns the file size
    unsigned getFileSize();
};
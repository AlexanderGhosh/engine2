#include "SharedMemoryManagerCPP.h"

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <cassert>
// #pragma comment(lib, "user32.lib")

wchar_t* SharedMemoryLocation::toCharArray(const std::string& txt) {
    const size_t cSize = txt.size() + 1;
    wchar_t* wc = new wchar_t[cSize];
    // mbstowcs(wc, txt.c_str(), cSize); 
    size_t outSize;
    mbstowcs_s(&outSize, wc, cSize, txt.c_str(), cSize - 1);
    return wc;
}

bool SharedMemoryLocation::verify() {
    return fileName != "" && fileSize > 0;
}

SharedMemoryLocation::SharedMemoryLocation() : fileName(""), fileSize(0), handler(nullptr), data(nullptr), owner(0) {

}

SharedMemoryLocation::SharedMemoryLocation(const std::string& fileName, const unsigned int& fileSize) : SharedMemoryLocation() {
    this->fileName = fileName;
    this->fileSize = fileSize;
}

SharedMemoryLocation::~SharedMemoryLocation() {
    closeFile();
}

bool SharedMemoryLocation::createFile() {
    assert(owner == 0);
    assert(verify());
    wchar_t* memName = toCharArray(fileName);

    handler = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        fileSize,              // maximum object size (low-order DWORD)
        memName);                // name of mapping object

    if (handler == nullptr)
    {
        std::cout << "Could not create file mapping object " << std::to_string(GetLastError()) << ".\n";
        return 0;
    }
    delete memName;
    owner = 1;
    return 1;
}

bool SharedMemoryLocation::openFile() {
    assert(owner == 0);
    assert(verify());
    wchar_t* memName = toCharArray(fileName);

    handler = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,                 // do not inherit the name
        memName);               // name of mapping object               // name of mapping object

    if (handler == nullptr)
    {
        std::cout << "Could not open file mapping object " << std::to_string(GetLastError()) << ".\n";
        return 0;
    }
    delete memName;
    owner = 2;
    return 1;
}

void* SharedMemoryLocation::getData() {
    assert(verify() && owner != 0);
    if (data)
        return data;
    data = MapViewOfFile(
        handler,             // handle to map object
        FILE_MAP_ALL_ACCESS, // read/write permission
        0,
        0,
        fileSize);

    if (data == nullptr)
    {
        std::cout << "Could not map view of file " << std::to_string(GetLastError()) << ".\n";
        CloseHandle(handler);

        return nullptr;
    }
    return data;
}

void SharedMemoryLocation::writeToFile(void* data, const unsigned int& dataSize, const unsigned int& offset) {
    assert(verify() && owner != 0);

    void* memLoc = (void*)((char*)this->data + offset);

    memcpy(memLoc, data, dataSize);
    auto t = _getch();
}

void SharedMemoryLocation::closeFile() {
    if (!data || !handler)
        return;

    UnmapViewOfFile(data);
    CloseHandle(handler);
    
    handler = data = nullptr;
}

bool SharedMemoryLocation::isCreator() {
    return owner == 1;
}
#include <windows.h>
#include <iostream>
#include <string>

void HandleError(const std::string& errorMessage) {
    std::cerr << errorMessage << ". Error code: " << GetLastError() << std::endl;
    exit(EXIT_FAILURE);
}

int main() {
    const std::string filePath = "E:\\Exam\\cmake-build-debug\\Exam.txt"; // Замість "example.txt" вкажіть шлях до вашого файлу

    // Відкрити файл для читання
    HANDLE hFile = CreateFile(
        filePath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        HandleError("Failed to open file");
    }

    // Створити об'єкт відображення файлу
    HANDLE hFileMapping = CreateFileMapping(
        hFile,
        nullptr,
        PAGE_READONLY,
        0,
        0,
        nullptr
    );

    if (hFileMapping == nullptr) {
        CloseHandle(hFile);
        HandleError("Failed to create file mapping");
    }

    // Відобразити файл у пам'ять
    LPVOID lpBaseAddress = MapViewOfFile(
        hFileMapping,
        FILE_MAP_READ,
        0,
        0,
        0
    );

    if (lpBaseAddress == nullptr) {
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        HandleError("Failed to map view of file");
    }

    // Отримати розмір файлу
    DWORD fileSize = GetFileSize(hFile, nullptr);
    if (fileSize == INVALID_FILE_SIZE) {
        UnmapViewOfFile(lpBaseAddress);
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        HandleError("Failed to get file size");
    }

    // Прочитати вміст файлу як масив символів
    const char* fileData = static_cast<const char*>(lpBaseAddress);

    // Підрахувати кількість символів у файлі
    size_t characterCount = 0;
    for (DWORD i = 0; i < fileSize; ++i) {
        if (fileData[i] != '\0') {
            ++characterCount;
        }
    }

    // Вивести результат
    std::cout << "The file contains " << characterCount << " characters." << std::endl;

    // Закрити всі ресурси
    UnmapViewOfFile(lpBaseAddress);
    CloseHandle(hFileMapping);
    CloseHandle(hFile);

    return 0;
}

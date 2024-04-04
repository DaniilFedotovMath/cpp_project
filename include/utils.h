#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>

#define ASSERT(cond)                         \
    {                                        \
        if (!(cond))                         \
            throw std::runtime_error(#cond); \
    }

template<class I, class T>
constexpr bool IsIn(I f, I l, const T& v)
{
    return std::find(f, l, v) != l;
}

template<class C, class T>
constexpr bool IsIn(const C& c, const T& v)
{
    return std::find(c.cbegin(), c.cend(), v) != c.cend();
}

inline size_t getFileSize(std::istream& file)
{
    const auto pos = file.tellg();
    file.seekg(0, std::ios::end);
    const auto fileSize = file.tellg();
    file.seekg(pos);
    return fileSize;
}

inline size_t getFileSize(std::string_view fileName)
{
    std::error_code ec{};
    const auto size = std::filesystem::file_size(fileName, ec);
    ASSERT(!ec);
    return size;
}

template<class Result>
Result readFileImpl(std::string_view fileName)
{
    const auto fileSize = getFileSize(fileName);
    if (fileSize == 0) {
        return Result{};
    }
    std::filebuf file;
    ASSERT(file.open(std::filesystem::path(fileName), std::ios_base::in | std::ios_base::binary));
    Result result(fileSize, '\0');
    const auto read =
        file.sgetn(reinterpret_cast<char*>(&result[0]), static_cast<std::streamsize>(fileSize));
    ASSERT(static_cast<size_t>(read) == fileSize);
    return result;
}

inline std::string readFileToString(std::string_view fileName)
{
    return readFileImpl<std::string>(fileName);
}


#pragma once
#include <fstream>
#include <unordered_map>

#include "Archive.h"
#include "PlatformDefine.h"


namespace ShapeEngine
{
    class SE_API WriteArchive_Binary : public WriterArchive
    {
    public:
        WriteArchive_Binary(const std::string& filePath);
        ~WriteArchive_Binary() override;

        bool IsValid() const override;

        WriterArchive& operator<<(bool value) override;
        WriterArchive& operator<<(int32_t value) override;
        WriterArchive& operator<<(uint32_t value) override;
        WriterArchive& operator<<(uint64_t value) override;
        WriterArchive& operator<<(float value) override;
        WriterArchive& operator<<(const std::string& value) override;
        WriterArchive& operator<<(const std::shared_ptr<Object>& value) override;

    private:
        template<typename T>
        void Write(const T& value);
        std::fstream FileStream;
        std::unordered_map<std::shared_ptr<Object>, uint64_t> SavedObjectMap;
    };

    template <typename T>
    void WriteArchive_Binary::Write(const T& value)
    {
        if (IsValid()) FileStream.write(reinterpret_cast<const char*>(&value), sizeof(T));
    }

    class SE_API ReadArchive_Binary : public ReaderArchive
    {
    public:
        ReadArchive_Binary(const std::string& filePath);
        ~ReadArchive_Binary() override;

        bool IsValid() const override;

        ReaderArchive& operator>>(bool& value) override;
        ReaderArchive& operator>>(int32_t& value) override;
        ReaderArchive& operator>>(uint32_t& value) override;
        ReaderArchive& operator>>(uint64_t& value) override;
        ReaderArchive& operator>>(float& value) override;
        ReaderArchive& operator>>(std::string& value) override;
        ReaderArchive& operator>>(std::shared_ptr<Object>& ptr) override;

    private:
        template<typename T>
        void Read(T& value);
        std::fstream FileStream;
        std::unordered_map<uint64_t, std::shared_ptr<Object>> LoadedObjectMap;
    };

    template <typename T>
    void ReadArchive_Binary::Read(T& value)
    {
        if (IsValid()) FileStream.read(reinterpret_cast<char*>(&value), sizeof(T));
    }
}
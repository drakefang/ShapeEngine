
#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include "PlatformDefine.h"

namespace ShapeEngine
{
    class Object;

    class ArchiveBase
    {
    public:
        virtual ~ArchiveBase() = default;
        virtual bool IsValid() const = 0;

    protected:
        ArchiveBase() = default;
        ArchiveBase(const ArchiveBase&) = delete;
        ArchiveBase& operator=(const ArchiveBase&) = delete;
    };

    class WriterArchive : public ArchiveBase
    {
    public:
        virtual WriterArchive& operator<<(bool value) = 0;
        virtual WriterArchive& operator<<(int32_t value) = 0;
        virtual WriterArchive& operator<<(uint32_t value) = 0;
        virtual WriterArchive& operator<<(uint64_t value) = 0;
        virtual WriterArchive& operator<<(float value) = 0;
        virtual WriterArchive& operator<<(const std::string& value) = 0;
        virtual WriterArchive& operator<<(const std::shared_ptr<Object>& value) = 0;
    };

    class ReaderArchive : public ArchiveBase
    {
    public:
        virtual ReaderArchive& operator>>(bool& value) = 0;
        virtual ReaderArchive& operator>>(int32_t& value) = 0;
        virtual ReaderArchive& operator>>(uint32_t& value) = 0;
        virtual ReaderArchive& operator>>(uint64_t& value) = 0;
        virtual ReaderArchive& operator>>(float& value) = 0;
        virtual ReaderArchive& operator>>(std::string& value) = 0;
        virtual ReaderArchive& operator>>(std::shared_ptr<Object>& value) = 0;
    };
}

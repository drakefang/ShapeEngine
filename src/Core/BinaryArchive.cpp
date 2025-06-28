
#include "BinaryArchive.h"

#include <entt/core/fwd.hpp>

#include "Object.h"

namespace ShapeGame
{
    WriteArchive_Binary::WriteArchive_Binary(const std::string& filePath)
    {
        FileStream.open(filePath, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    }

    WriteArchive_Binary::~WriteArchive_Binary()
    {
        if (FileStream.is_open()) FileStream.close();
    }

    bool WriteArchive_Binary::IsValid() const
    {
        return FileStream.is_open() && !FileStream.fail();
    }

    WriterArchive& WriteArchive_Binary::operator<<(bool value)
    {
        Write(value);
        return *this;
    }

    WriterArchive& WriteArchive_Binary::operator<<(int32_t value)
    {
        Write(value);
        return *this;
    }

    WriterArchive& WriteArchive_Binary::operator<<(uint32_t value)
    {
        Write(value);
        return *this;
    }

    WriterArchive& WriteArchive_Binary::operator<<(uint64_t value)
    {
        Write(value);
        return *this;
    }

    WriterArchive& WriteArchive_Binary::operator<<(float value)
    {
        Write(value);
        return *this;
    }

    WriterArchive& WriteArchive_Binary::operator<<(const std::string& value)
    {
        const size_t len = value.length();
        Write(len);
        if (len > 0) FileStream.write(value.c_str(), len);
        return *this;
    }

    WriterArchive& WriteArchive_Binary::operator<<(const std::shared_ptr<Object>& value)
    {
        if (value == nullptr)
        {
            constexpr uint64_t nullId = 0;
            *this << nullId;
        }
        else
        {
            if (const auto it = SavedObjectMap.find(value); it != SavedObjectMap.end())
            {
                *this << it->second;
            }
            else
            {
                const uint64_t newId = value->ObjectId;
                SavedObjectMap[value] = newId;
                *this << newId;

                const entt::id_type typeId = value->GetTypeId();
                *this << typeId;

                value->Save(*this);
            }
        }
        return *this;
    }

    ReadArchive_Binary::ReadArchive_Binary(const std::string& filePath)
    {
        FileStream.open(filePath, std::ios_base::in | std::ios_base::binary);
    }

    ReadArchive_Binary::~ReadArchive_Binary()
    {
        if (FileStream.is_open()) FileStream.close();
    }

    bool ReadArchive_Binary::IsValid() const
    {
        return FileStream.is_open() && !FileStream.fail();
    }

    ReaderArchive& ReadArchive_Binary::operator>>(bool& value)
    {
        Read(value);
        return *this;
    }

    ReaderArchive& ReadArchive_Binary::operator>>(int32_t& value)
    {
        Read(value);
        return *this;
    }

    ReaderArchive& ReadArchive_Binary::operator>>(uint32_t& value)
    {
        Read(value);
        return *this;
    }

    ReaderArchive& ReadArchive_Binary::operator>>(uint64_t& value)
    {
        Read(value);
        return *this;
    }

    ReaderArchive& ReadArchive_Binary::operator>>(float& value)
    {
        Read(value);
        return *this;
    }

    ReaderArchive& ReadArchive_Binary::operator>>(std::string& value)
    {
        size_t len = 0;
        Read(len);
        if (len > 0)
        {
            value.resize(len);
            FileStream.read(&value[0], len);
        }
        else
        {
            value.clear();
        }
        return *this;
    }

    ReaderArchive& ReadArchive_Binary::operator>>(std::shared_ptr<Object>& ptr)
    {
        uint64_t objectId;
        *this >> objectId;
        if (objectId == 0)
        {
            ptr = nullptr;
        }
        else
        {
            auto it = LoadedObjectMap.find(objectId);
            if (it != LoadedObjectMap.end())
            {
                ptr = it->second;
            }
            else
            {
                uint32_t typeId_u32;
                *this >> typeId_u32;
                entt::id_type typeId = typeId_u32;

                if (auto metaType = entt::resolve(typeId))
                {
                    if(entt::meta_any newInstance = metaType.construct())
                    {
                        ptr = newInstance.cast<std::shared_ptr<Object>>();
                        LoadedObjectMap[objectId] = ptr;
                        ptr->Load(*this);
                    }
                }
                else
                {
                    ptr = nullptr;
                }
            }
        }
        return *this;
    }
}
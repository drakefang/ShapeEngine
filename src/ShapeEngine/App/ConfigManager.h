//
// Created by yuyzc on 2025/7/24.
//

#pragma once

#include "Core/PlatformDefine.h"
#include <string>
#include <vector>
#include <filesystem>
#include <optional>
#include <memory>
#include <any>
#include <functional>
#include <utility>

namespace ShapeEngine
{
    using OnConfigChangedDelegate = std::function<void(const std::string& key, const std::any& newValue)>;

    class SHAPE_ENGINE_API ConfigManager
    {
    public:
        static ConfigManager& Get();

        ConfigManager();
        ~ConfigManager();

        ConfigManager(ConfigManager&&) noexcept;
        ConfigManager& operator=(ConfigManager&&) noexcept;

        ConfigManager(const ConfigManager&) = delete;
        ConfigManager& operator=(const ConfigManager&) = delete;

        void LoadFromFiles(const std::vector<std::filesystem::path>& configFilePaths) const;

        template<typename T>
        void SetValue(const std::string& key, T value);

        [[nodiscard]] uint64_t RegisterChangeCallback(const std::string& key, OnConfigChangedDelegate delegate) const;
        void UnregisterChangeCallback(const std::string& key, uint64_t handle) const;

        template<typename T>
        std::optional<T> GetValue(const std::string& key) const;

        template<typename T>
        T GetValueOrDefault(const std::string& key, T defaultValue) const;

    private:
        std::unique_ptr<class Impl> pimpl;

        void SetValueAny(const std::string& key, const std::any& value) const;
        [[nodiscard]] std::any GetValueAny(const std::string& key) const;
    };

    template<typename T>
    void ConfigManager::SetValue(const std::string& key, T value)
    {
        SetValueAny(key, std::any(std::move(value)));
    }

    template<typename T>
    std::optional<T> ConfigManager::GetValue(const std::string& key) const
    {
        std::any result = GetValueAny(key);
        if (result.has_value())
        {
            try
            {
                return std::any_cast<T>(result);
            }
            catch (const std::bad_any_cast& e)
            {
                return std::nullopt;
            }
        }
        return std::nullopt;
    }

    template<typename T>
    T ConfigManager::GetValueOrDefault(const std::string& key, T defaultValue) const
    {
        auto value = GetValue<T>(key);
        return value.value_or(std::move(defaultValue));
    }
}
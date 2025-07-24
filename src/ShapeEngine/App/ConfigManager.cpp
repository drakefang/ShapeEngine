//
// Created by yuyzc on 2025/7/24.
//

// ReSharper disable CppDeclarationHidesLocal
#include "ConfigManager.h"
#include "Core/Logger.h"

#include <toml++/toml.hpp>
#include <algorithm>
#include <sstream>

namespace ShapeEngine
{
    class Impl
    {
    public:
        toml::table ConfigData;

        struct DelegateInfo
        {
            uint64_t Handle;
            OnConfigChangedDelegate Delegate;
        };

        std::map<std::string, std::vector<DelegateInfo> > ChangeDelegates;
        uint64_t NextDelegateHandle = 1;

        template <typename T>
        void SetValueByPath(const std::string& key, T&& value)
        {
            std::stringstream ss(key);
            std::string segment;
            std::vector<std::string> path_segments;
            while (std::getline(ss, segment, '.'))
            {
                path_segments.push_back(segment);
            }

            if (path_segments.empty())
            {
                return;
            }

            toml::node* current_node = &ConfigData;
            for (size_t i = 0; i < path_segments.size() - 1; ++i)
            {
                auto& segment_str = path_segments[i];
                if (!current_node->is_table() || !current_node->as_table()->contains(segment_str))
                {
                    current_node->as_table()->insert(segment_str, toml::table{});
                }
                current_node = current_node->as_table()->get(segment_str);
            }

            current_node->as_table()->insert_or_assign(path_segments.back(), std::forward<T>(value));
        }
    };

    ConfigManager& ConfigManager::Get()
    {
        static ConfigManager instance;
        return instance;
    }

    ConfigManager::ConfigManager() : pimpl(std::make_unique<Impl>())
    {
    }

    ConfigManager::~ConfigManager() = default;

    ConfigManager::ConfigManager(ConfigManager&&) noexcept = default;

    ConfigManager& ConfigManager::operator=(ConfigManager&&) noexcept = default;

    void ConfigManager::LoadFromFiles(const std::vector<std::filesystem::path>& configFilePaths) const
    {
        pimpl->ConfigData.clear();
        pimpl->ChangeDelegates.clear();

        for (const auto& path : configFilePaths)
        {
            if (!std::filesystem::exists(path))
            {
                Logger()->warn("Config file not found, skipping: {}", path.string());
                continue;
            }
            try
            {
                for (toml::table tbl = toml::parse_file(path.string()); auto&& [key, val] : tbl)
                {
                    pimpl->ConfigData.insert_or_assign(key, val);
                }
                Logger()->info("Successfully loaded and merged config file: {}", path.string());
            }
            catch (const toml::parse_error& err)
            {
                Logger()->error("Failed to parse config file '{}':\n{}", path.string(), err.what());
            }
        }
    }

    uint64_t ConfigManager::RegisterChangeCallback(const std::string& key, OnConfigChangedDelegate delegate) const
    {
        const uint64_t handle = pimpl->NextDelegateHandle++;
        pimpl->ChangeDelegates[key].push_back({handle, std::move(delegate)});
        return handle;
    }

    void ConfigManager::UnregisterChangeCallback(const std::string& key, uint64_t handle) const
    {
        if (const auto it = pimpl->ChangeDelegates.find(key); it != pimpl->ChangeDelegates.end())
        {
            auto& delegates = it->second;
            std::erase_if(
                delegates,
                [handle](const Impl::DelegateInfo& info)
                {
                    return info.Handle == handle;
                });
        }
    }

    void ConfigManager::SetValueAny(const std::string& key, const std::any& value) const
    {
        if (const auto* val_ptr = std::any_cast<bool>(&value))
        {
            pimpl->SetValueByPath(key, toml::value(*val_ptr));
        }
        else if (const auto* val_ptr = std::any_cast<int64_t>(&value))
        {
            pimpl->SetValueByPath(key, toml::value(*val_ptr));
        }
        else if (const auto* val_ptr = std::any_cast<int>(&value))
        {
            pimpl->SetValueByPath(key, toml::value(static_cast<int64_t>(*val_ptr)));
        }
        else if (const auto* val_ptr = std::any_cast<double>(&value))
        {
            pimpl->SetValueByPath(key, toml::value(*val_ptr));
        }
        else if (const auto* val_ptr = std::any_cast<float>(&value))
        {
            pimpl->SetValueByPath(key, toml::value(static_cast<double>(*val_ptr)));
        }
        else if (const auto* val_ptr = std::any_cast<std::string>(&value))
        {
            pimpl->SetValueByPath(key, toml::value(*val_ptr));
        }
        else if (const auto* val_ptr = std::any_cast<const char*>(&value))
        {
            pimpl->SetValueByPath(key, toml::value(*val_ptr));
        }
        else
        {
            Logger()->error("Unsupported type for SetValue with key '{}'", key);
            return;
        }

        if (const auto it = pimpl->ChangeDelegates.find(key); it != pimpl->ChangeDelegates.end())
        {
            for (const auto& [Handle, Delegate] : it->second)
            {
                if (Delegate)
                {
                    Delegate(key, value);
                }
            }
        }
    }

    std::any ConfigManager::GetValueAny(const std::string& key) const
    {
        const auto node = pimpl->ConfigData.at_path(key);
        if (!node)
            return {};

        switch (node.type())
        {
            case toml::node_type::string:
                return node.value<std::string>();
            case toml::node_type::integer:
                return node.value<int64_t>();
            case toml::node_type::floating_point:
                return node.value<double>();
            case toml::node_type::boolean:
                return node.value<bool>();
            default:
                return {};
        }
    }
}
//
// Created by yuyzc on 2025/8/10.
//

#include "SubsystemManager.h"

#include <ranges>

#include "Logger.h"

namespace ShapeEngine
{
    SubsystemManager::~SubsystemManager()
    {
        ShutdownAll();
    }

    void SubsystemManager::StartupAll()
    {
        if (!StartupOrder.empty())
        {
            Logger()->warn("SubsystemManager::StartupAll() called more than once.");
            return;
        }

        Logger()->info("Starting up all engine subsystems...");

        std::unordered_map<std::string, EVisitState> visitState;
        for (const auto& key : SubsystemMap | std::views::keys)
        {
            visitState[key] = EVisitState::Unvisited;
        }

        for (const auto& [fst, snd] : SubsystemMap)
        {
            if (visitState[fst] == EVisitState::Unvisited)
            {
                TopologicalSort(snd, StartupOrder, visitState, SubsystemMap);
            }
        }

        for (ISubSystem* system : StartupOrder)
        {
            Logger()->info("  -> Starting subsystem: {}", system->GetName());
            system->Startup();
        }

        Logger()->info("All engine subsystems started successfully.");
    }

    void SubsystemManager::ShutdownAll()
    {
        if (StartupOrder.empty())
        {
            return;
        }

        Logger()->info("Shutting down all engine subsystems...");

        std::reverse(StartupOrder.begin(), StartupOrder.end());
        for (ISubSystem* system : StartupOrder)
        {
            Logger()->info("  -> Shutting down subsystem: {}", system->GetName());
            system->Shutdown();
        }

        StartupOrder.clear();
        SubsystemMap.clear();
        Subsystems.clear();

        Logger()->info("All engine subsystems shut down.");
    }

    void SubsystemManager::TopologicalSort(
        const std::shared_ptr<ISubSystem>& system,
        std::vector<ISubSystem*>& sortedList,
        std::unordered_map<std::string, EVisitState>& visitState,
        std::unordered_map<std::string, std::shared_ptr<ISubSystem>>& systemMap)
    {
        const std::string systemName = system->GetName();
        visitState[systemName] = EVisitState::Visiting;

        for (const auto& depName : system->GetDependencies())
        {
            if (!systemMap.contains(depName))
            {
                throw std::runtime_error("Dependency subsystem not found: " + depName);
            }

            if (visitState[depName] == EVisitState::Unvisited)
            {
                TopologicalSort(systemMap[depName], sortedList, visitState, systemMap);
            }
            else if (visitState[depName] == EVisitState::Visiting)
            {
                throw std::runtime_error("Circular dependency detected involving: " + systemName + " and " + depName);
            }
        }

        visitState[systemName] = EVisitState::Visited;
        sortedList.push_back(system.get());
    }

}

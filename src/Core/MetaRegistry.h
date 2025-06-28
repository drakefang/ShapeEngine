
#pragma once

#include <functional>

namespace ShapeGame
{
    struct RegistrationNode
    {
        std::function<void()> RegistrationFunc;
        RegistrationNode* Next = nullptr;

        RegistrationNode(std::function<void()> func) : RegistrationFunc(func)
        {

        }
    };

    class MetaRegistry
    {
    public:
        static void AddNode(RegistrationNode* node)
        {
            node->Next = GetHead();
            GetHead() = node;
        }

        static void RegisterAll()
        {
            RegistrationNode* current = GetHead();
            while (current)
            {
                if (current->RegistrationFunc)
                {
                    current->RegistrationFunc();
                }
                current = current->Next;
            }
        }

    private:
        static RegistrationNode*& GetHead()
        {
            static RegistrationNode* head = nullptr;
            return head;
        }
    };

    struct AutoRegistrar
    {
        AutoRegistrar(std::function<void()> func) : Node(std::move(func))
        {
            MetaRegistry::AddNode(&Node);
        }

    private:
        RegistrationNode Node;
    };
}
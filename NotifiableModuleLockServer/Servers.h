#pragma once

#include "ServerA.g.h"
#include "ServerB.g.h"

namespace winrt::NotifiableModuleLock::implementation
{
    struct ServerA : ServerAT<ServerA>
    {
        ServerA() = default;

        winrt::hstring ToString()
        {
            return L"A from server";
        }
    };

    struct ServerB : ServerBT<ServerB>
    {
        ServerB() = default;

        winrt::hstring ToString()
        {
            return L"B from server";
        }
    };
}

namespace winrt::NotifiableModuleLock::factory_implementation
{
    struct ServerA : ServerAT<ServerA, implementation::ServerA>
    {
    };

    struct ServerB : ServerBT<ServerB, implementation::ServerB>
    {
    };
}

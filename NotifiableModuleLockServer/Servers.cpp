#include "pch.h"
#include "Servers.h"
#if __has_include("ServerA.g.cpp")
#include "ServerA.g.cpp"
#endif
#if __has_include("ServerB.g.cpp")
#include "ServerB.g.cpp"
#endif

using namespace winrt;

namespace winrt::NotifiableModuleLock::implementation
{
}

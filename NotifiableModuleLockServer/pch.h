#pragma once
#include "Unknwn.h"
#include "NotifiableModuleLock.h"
namespace winrt
{
    inline auto& get_module_lock()
    {
        static NotifiableModuleLock<void(*)()> lock;
        return lock;
    }
}
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>

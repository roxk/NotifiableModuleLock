#include "pch.h"
#include <Unknwn.h>
#include <winrt/Windows.Foundation.h>
#include "wil/resource.h"
#include "Servers.h"

namespace wf = winrt::Windows::Foundation;

template<typename T>
struct Factory : winrt::implements<Factory<T>, IClassFactory, winrt::no_module_lock>
{
    HRESULT __stdcall CreateInstance(
        IUnknown* outer,
        GUID const& iid,
        void** result) noexcept final
    {
        *result = nullptr;

        if (outer)
        {
            return CLASS_E_NOAGGREGATION;
        }

        return T().as(iid, result);
    }

    HRESULT __stdcall LockServer(BOOL) noexcept final
    {
        return S_OK;
    }
};

template<typename T, typename... Rest>
std::vector<DWORD> Register()
{
    std::vector<DWORD> registrations;
    registrations.reserve(sizeof...(Rest) + 1);
    DoRegister<T, Rest...>(registrations);
    return registrations;
}

template<typename T = void, typename... Rest>
void DoRegister(std::vector<DWORD>& registrations)
{
    auto& back = registrations.emplace_back();
    winrt::check_hresult(CoRegisterClassObject(
        winrt::guid_of<T>(),
        winrt::make<Factory<T>>().get(),
        CLSCTX_LOCAL_SERVER,
        REGCLS_MULTIPLEUSE,
        &back));
    DoRegister<Rest...>(registrations);
}

template<>
void DoRegister<void>(std::vector<DWORD>&)
{
}

wil::unique_event _comExitEvent;

void _notifier()
{
    _comExitEvent.SetEvent();
}

int main()
{
    _comExitEvent.create();

    NotifiableModuleLock<decltype(&_notifier)>::func = _notifier;
    winrt::init_apartment();

    auto registrations = Register<winrt::NotifiableModuleLock::ServerA, winrt::NotifiableModuleLock::ServerB>();

    _comExitEvent.wait();

    for (auto&& registration : registrations)
    {
        CoRevokeClassObject(registration);
    }

    return 0;
}
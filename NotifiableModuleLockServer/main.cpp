#include "pch.h"
#include <Unknwn.h>
#include <winrt/Windows.Foundation.h>
#include "wil/resource.h"

namespace wf = winrt::Windows::Foundation;

struct Stringable : winrt::implements<Stringable, wf::IStringable>
{
    winrt::hstring ToString()
    {
        return L"Hello from server";
    }
};

struct Factory : winrt::implements<Factory, IClassFactory, winrt::no_module_lock>
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

        return winrt::make<Stringable>().as(iid, result);
    }

    HRESULT __stdcall LockServer(BOOL) noexcept final
    {
        return S_OK;
    }
};

static constexpr GUID server_clsid // DAA16D7F-EF66-4FC9-B6F2-3E5B6D924576
{
    0xdaa16d7f, 0xef66, 0x4fc9, { 0xb6, 0xf2, 0x3e, 0x5b, 0x6d, 0x92, 0x45, 0x76 }
};

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

    DWORD registration{};

    winrt::check_hresult(CoRegisterClassObject(
        server_clsid,
        winrt::make<Factory>().get(),
        CLSCTX_LOCAL_SERVER,
        REGCLS_MULTIPLEUSE,
        &registration));

    _comExitEvent.wait();

    winrt::check_hresult(CoRevokeClassObject(registration));

    return 0;
}
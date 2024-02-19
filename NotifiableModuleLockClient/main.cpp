#include "pch.h"

namespace wf = winrt::Windows::Foundation;

static constexpr GUID a_clsid // DAA16D7F-EF66-4FC9-B6F2-3E5B6D924576
{
    0xdaa16d7f, 0xef66, 0x4fc9, { 0xb6, 0xf2, 0x3e, 0x5b, 0x6d, 0x92, 0x45, 0x76 }
};

static constexpr GUID b_clsid // DAA16D7F-EF66-4FC9-B6F2-3E5B6D924576
{
    0xdaa16d7f, 0xef66, 0x4fc9, { 0xb6, 0xf2, 0x3e, 0x5b, 0x6d, 0x92, 0x45, 0x77 }
};

int main()
{
    winrt::init_apartment();
    try
    {
        {
            auto a = winrt::create_instance<wf::IStringable>(a_clsid, CLSCTX_LOCAL_SERVER);
            auto b = winrt::create_instance<wf::IStringable>(b_clsid, CLSCTX_LOCAL_SERVER);
        }
    }
    catch (winrt::hresult_error const& e)
    {
        printf("Error: %ls", e.message().c_str());
    }
}

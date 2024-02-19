#pragma once

#include <atomic>
#include <stdint.h>

template<typename Func>
struct NotifiableModuleLock
{
    static inline Func func;
    NotifiableModuleLock() = default;

    NotifiableModuleLock(uint32_t count) : m_count(count)
    {
    }

    uint32_t operator=(uint32_t count) noexcept
    {
        return m_count = count;
    }

    uint32_t operator++() noexcept
    {
        return m_count.fetch_add(1, std::memory_order_relaxed) + 1;
    }

    uint32_t operator--() noexcept
    {
        auto const remaining = m_count.fetch_sub(1, std::memory_order_release) - 1;

        if (remaining == 0)
        {
            std::atomic_thread_fence(std::memory_order_acquire);
            func();
        }
        else if (remaining < 0)
        {
            abort();
        }

        return remaining;
    }

    operator uint32_t() const noexcept
    {
        return m_count;
    }

private:

    std::atomic<int32_t> m_count;
};

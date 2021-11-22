#pragma once
#include <coroutine>

struct custom_suspender final
{
	using coro_handle = std::coroutine_handle<>;

	bool m_state;
	explicit custom_suspender(bool state) : m_state{ state } {}
	bool await_ready() const noexcept { return m_state; }
	bool await_suspend(coro_handle) { return !m_state; }
	void await_resume() const noexcept {}
};



template <typename custom_suspender>
struct return_type
{
    struct promise_type
    {
        using coro_handle = std::coroutine_handle<promise_type>;
        inline static bool is_aborted = false;
        constexpr std::suspend_always initial_suspend()const noexcept { return {}; }
        constexpr std::suspend_always final_suspend()const noexcept { return {}; }
        auto get_return_object() { return coro_handle::from_promise(*this); }

        void return_void() const noexcept {}
        void unhandled_exception() { std::terminate(); }

        auto await_transform(std::suspend_always)
        {
            return custom_suspender{ is_aborted };
        }
    };

    using coro_handle = std::coroutine_handle<promise_type>;
     return_type(coro_handle handle) : m_handle{ handle }
    {}

    return_type() = delete;

    return_type(const return_type&) = delete;
    return_type& operator=(const return_type&) = delete;

    return_type(return_type&& rhs) noexcept : m_handle{ rhs.m_handle }
    {
        rhs.m_handle = nullptr;
    }
    return_type& operator=(return_type&& rhs) noexcept
    {
        if (&rhs == this)
            return *this;
        m_handle = rhs.m_handle;
        rhs.m_handle = nullptr;
        return*this;
    }
    ~return_type()
    {
        if (m_handle)
            m_handle.destroy();
    }

    bool resume()
    {
        return m_handle ? (m_handle.resume(), !m_handle.done()) : false;
    }

    void abort()
    {
        promise_type::is_aborted = true;
        resume();
    }

private:
    coro_handle m_handle;
};
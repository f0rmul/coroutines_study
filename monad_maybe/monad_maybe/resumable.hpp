#pragma once 
#include <coroutine>
#include <optional>

template <typename type>
class resume_type
{
    struct awaiter
    {
        std::optional<type> m_opt;
        awaiter(std::optional<type> opt) : m_opt{ opt } {}
        bool await_ready() { return m_opt.has_value(); }
        void await_suspend(std::coroutine_handle<>) {}
        auto await_resume() { return m_opt.value(); }

    };

public:
    struct promise_type
    {
        std::optional<type> m_opt;
        using coro_handle = std::coroutine_handle<promise_type>;

        auto get_return_object() { return coro_handle::from_promise(*this); }
        awaiter await_transform(std::optional<type> opt) { return  awaiter{ opt }; }
        void return_value(type value) { m_opt = value; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() { std::terminate(); }
    };
    using coro_handle = std::coroutine_handle<promise_type>;
    resume_type(coro_handle h) : m_handle{ h } {}
    resume_type(const resume_type&) = delete;
    resume_type& operator=(const resume_type&) = delete;
    resume_type(resume_type&& rhs) : m_handle{ rhs.m_handle }
    {
        rhs.m_handle = nullptr;
    }
    resume_type& operator=(resume_type&& rhs)
    {
        if (&rhs == this)
            return*this;
        m_handle = rhs.m_handle;
        rhs.m_handle = nullptr;
        return *this;
    }

    std::optional<type> current_value() const
    {
        if (m_handle)
            return m_handle.promise().m_opt;
        return std::nullopt;
    }
    ~resume_type()
    {
        if (m_handle)
            m_handle.destroy();
    }
private:
    coro_handle m_handle;
};

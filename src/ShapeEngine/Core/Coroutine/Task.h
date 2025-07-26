//
// Created by yuyzc on 2025/7/26.
//

#pragma once

#include <coroutine>
#include <optional>
#include <stdexcept>
#include <utility>
#include <exception>

namespace ShapeEngine
{
    template <typename T>
    class [[nodiscard]] Task;

    namespace detail
    {
        struct PromiseBase
        {
            std::coroutine_handle<> continuation = nullptr;
            std::exception_ptr exception = nullptr;

            [[nodiscard]] std::suspend_always initial_suspend() const noexcept
            {
                return {};
            }

            void unhandled_exception()
            {
                exception = std::current_exception();
            }

            struct FinalAwaiter
            {
                PromiseBase* promise;

                [[nodiscard]] bool await_ready() const noexcept
                {
                    return false;
                }

                [[nodiscard]] std::coroutine_handle<> await_suspend(std::coroutine_handle<> /*handle*/) const noexcept
                {
                    if (promise->continuation)
                    {
                        return promise->continuation;
                    }
                    else
                    {
                        return std::noop_coroutine();
                    }
                }

                void await_resume() const noexcept
                {
                }
            };

            FinalAwaiter final_suspend() noexcept
            {
                return FinalAwaiter{this};
            }
        };

        template <typename T>
        struct Promise : public PromiseBase
        {
            Task<T> get_return_object();

            template <typename U>
            void return_value(U&& value)
            {
                result.emplace(std::forward<U>(value));
            }

            std::optional<T> result;
        };

        template <>
        struct Promise<void> : public PromiseBase
        {
            Task<void> get_return_object();

            void return_void() const
            {
            }
        };
    }

    template <typename T = void>
    class [[nodiscard]] Task
    {
    public:
        using promise_type = detail::Promise<T>;

        explicit Task(std::coroutine_handle<promise_type> handle) : handle(handle)
        {
        }

        ~Task()
        {
            if (handle)
            {
                handle.destroy();
            }
        }

        Task(const Task&) = delete;

        Task& operator=(const Task&) = delete;

        Task(Task&& other) noexcept : handle(std::exchange(other.handle, nullptr))
        {
        }

        Task& operator=(Task&& other) noexcept
        {
            if (this != &other)
            {
                if (handle)
                {
                    handle.destroy();
                }
                handle = std::exchange(other.handle, nullptr);
            }
            return *this;
        }

        [[nodiscard]] bool await_ready() const noexcept
        {
            return !handle || handle.done();
        }

        std::coroutine_handle<promise_type> await_suspend(std::coroutine_handle<> awaiting_coroutine) noexcept
        {
            handle.promise().continuation = awaiting_coroutine;
            return handle;
        }

        auto await_resume()
        {
            if (handle.promise().exception)
            {
                std::rethrow_exception(handle.promise().exception);
            }

            if constexpr (std::is_void_v<T>)
            {
                return;
            }
            else
            {
                return std::move(*handle.promise().result);
            }
        }

        void Start()
        {
            if (handle && !handle.done())
            {
                handle.resume();
            }
        }

    private:
        std::coroutine_handle<promise_type> handle;
    };

    namespace detail
    {
        template <typename T>
        Task<T> Promise<T>::get_return_object()
        {
            return Task<T>{std::coroutine_handle<Promise<T> >::from_promise(*this)};
        }

        inline Task<void> Promise<void>::get_return_object()
        {
            return Task<void>{std::coroutine_handle<Promise<void> >::from_promise(*this)};
        }
    }
}
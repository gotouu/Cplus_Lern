#include<iostream>
#include<coroutine>

// �������m�ۂ��K�v�ƂȂ����Ƃ���::operator new(size_t, nothrow_t)���g����
struct generator {
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;
    struct promise_type {
        int current_value;
        auto get_return_object_on_allocation_failure() { return generator{ nullptr }; }
        auto get_return_object() { return generator{ handle::from_promise(*this) }; }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void unhandled_exception() { std::terminate(); }
        void return_void() {}
        auto yield_value(int value) {
            current_value = value;
            return std::suspend_always{};
        }
    };
    bool move_next() { return coro ? (coro.resume(), !coro.done()) : false; }
    int current_value() { return coro.promise().current_value; }
    generator(generator const&) = delete;
    generator(generator&& rhs) : coro(rhs.coro) { rhs.coro = nullptr; }
    ~generator() { if (coro) coro.destroy(); }
private:
    generator(handle h) : coro(h) {}
    handle coro;
};

//generator f(){ co_yield 1; co_yield 2; };

int main() {
    /*auto g = f();
    while (g.move_next()) std::cout << g.current_value() << std::endl;*/
}
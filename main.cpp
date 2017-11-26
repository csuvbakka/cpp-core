#include "any_of.hpp"

struct Foo
{
    explicit Foo(int x)
        : i(x)
    {
    }
    Foo(const Foo&) { std::cout << "copy\n"; }
    Foo(Foo&&) { std::cout << "move\n"; }

    int i;
};

inline bool operator==(const Foo& lhs, const Foo& rhs)
{
    return lhs.i == rhs.i;
}

int main()
{
    const Foo f1{13};
    const Foo f2{69};

    if (f1 == any_of(f2, Foo{13}))
        std::cout << "yep\n";

    if (f1 != any_of(f2, Foo{99}))
        std::cout << "yep\n";

    if (42 < any_of(13, 99))
        std::cout << "yep\n";
}

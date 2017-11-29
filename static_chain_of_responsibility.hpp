#include <iostream>
#include <type_traits>
#include <tuple>

template <typename... Handlers> 
class StaticChainOfResponsibility {
public:
  explicit StaticChainOfResponsibility(const Handlers&... handlers)
      : handlers_(std::forward_as_tuple(handlers)...) {}
  
  template <typename... Ts,
  typename = std::enable_if_t<sizeof...(Ts) == sizeof...(Handlers)>>
  explicit StaticChainOfResponsibility(Ts&&... handlers)
        : handlers_(std::forward_as_tuple(std::forward<Ts>(handlers)...))
    {
    }

  template <typename... Args> void execute(Args&&... args) {
    std::apply([&args...](auto&&... handlers){(handlers(std::forward<Args>(args)...), ...);}, handlers_);
  }

private:
  std::tuple<Handlers...> handlers_;
};

struct Foo {};
struct ByX {
    ByX() { std::cout << "ctor\n"; }
    ByX(const ByX&) { std::cout << "copy\n"; }
    ByX(ByX&&) { std::cout << "move\n"; }

    void operator()(const Foo&) { std::cout << "ByX" << std::endl; }
};
struct ByNotX {
    void operator()(const Foo&) { std::cout << "ByNotX" << std::endl; }
};


int main()
{
    StaticChainOfResponsibility chain(ByX{}, ByNotX{}, ByX{});
    chain.execute(Foo{});
}

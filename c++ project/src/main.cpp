#include <iostream>
#include <memory>

template <typename T>
auto get_value(T t) {
    if constexpr (std::is_pointer_v<T>)
        return *t;
    else
        return t;
}

int main()
{
     auto pi = std::make_unique<int>(9);
     int i = 9;

     std::cout << "try of functionality of git using visual studio" << '\n';
     
     std::cout << get_value(pi.get()) << "\n";
     std::cout << get_value(i) << "\n";
}
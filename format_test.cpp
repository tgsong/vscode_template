#include <iostream>
#include <memory>
#include <vector>

using namespace std;

namespace {

template<typename T>
struct Foo {
public:
    Foo(int a) : m_a{a} {}

    int m_a;

    int f() noexcept
    {
        vector<int> v{1, 2, 3};
        auto l = []() {
            int* a = nullptr;
            int& bb = *a;
            return 1;
        };

        try {
            bool a = true;
            if (a) {
                return 0;
            } else {
                return 1;
            }
        } catch (...) {
            return 0;
        }
    }
};

} // namespace

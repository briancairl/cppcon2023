#include <iostream>
#include <map>

int main() {

    std::multimap<int, int> mmap;
    mmap.emplace(0, 3);
    mmap.emplace(0, 1);
    mmap.emplace(0, 2);
    for (int i = 0; i < 33; ++i)
    {
        mmap.emplace(1, i);
    }
    mmap.emplace(0, 4);
    mmap.emplace(2, 3);

    auto r = mmap.equal_range(0);

    auto prev = r.first;
    auto curr = std::next(prev);
    for (; curr != r.second; ++prev, ++curr) {
        std::cerr << (reinterpret_cast<std::byte*>(std::addressof(*curr)) - reinterpret_cast<std::byte*>(std::addressof(*prev))) << std::endl;
    }
}

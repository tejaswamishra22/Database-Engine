#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

constexpr std::size_t PAGE_SIZE = 4096;

class Page {
public:
    Page() = default;

    std::byte* getData(){
        return buffer.data();
    }

private:
    std::array<std::byte, PAGE_SIZE> buffer{};
};

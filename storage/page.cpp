#include "page.h"

std::byte* Page::getByte() {
    return buffer.data();
}

const std::byte* Page::getByte() const {
    return buffer.data();
}

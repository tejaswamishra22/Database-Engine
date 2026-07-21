#include "page.h"

std::byte* Page::getByte() {
    return buffer.data();
}

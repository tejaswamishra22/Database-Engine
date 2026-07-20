#include "page.h"

std::byte* Page::getData() {
    return buffer.data();
}

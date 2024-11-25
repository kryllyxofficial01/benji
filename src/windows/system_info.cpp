#include "include/system_info.hpp"

std::string get_cpu_name() {
    std::string info;

    std::array<int, 4> integer_buffer = {};
    std::array<char, 64> char_buffer = {};

    constexpr size_t int_buf_size = sizeof(int) * integer_buffer.size();

    for (int i = 2; i <= 4; i++) {
        __cpuid(integer_buffer.data(), 0x80000000 + i);

        std::memcpy(char_buffer.data(), integer_buffer.data(), int_buf_size);

        info += std::string(char_buffer.data());
    }

    return info;
}
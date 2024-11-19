#include "include/system_info.hpp"

std::string get_cpu_info() {
    std::string info;

    std::array<int, 4> integer_buffer = {};
    std::array<char, 64> char_buffer = {};

    constexpr size_t int_buf_size = sizeof(int) * integer_buffer.size();

    constexpr std::array<unsigned int, 3> function_ids = {
        // Manufacturer
        0x80000002,

        // Model
        0x80000003,

        // Clock Speed
        0x80000004
    };

    for (int id: function_ids) {
        __cpuid(integer_buffer.data(), id);

        std::memcpy(char_buffer.data(), integer_buffer.data(), int_buf_size);

        info += std::string(char_buffer.data());
    }

    return info;
}
#ifndef BEZPIEKA_RC4_HPP
#define BEZPIEKA_RC4_HPP

#include <array>
#include <string>
#include <string_view>
#include <vector>

constexpr size_t s_size = 256;

namespace details
{
    template<size_t N>
    constexpr std::array<char8_t, N> ksa(const std::vector<char8_t>& key) {
        const size_t key_size = key.size();
        std::array<char8_t, N> s{};
        size_t j = 0;

        for (size_t i = 0; i < N; i++) {
            s[i] = i;
        }

        for (size_t i = 0; i < 256; i++) {
            j = (j + s[i] + key[i % key_size]) % N;
            std::swap(s[i], s[j]);
        }
        return s;
    }

    template<size_t N>
    constexpr std::vector<char8_t> rpga(const std::vector<char8_t>& input, std::array<char8_t, N>& s) {
        std::vector<char8_t> output(input.size());
        size_t i = 0;
        size_t j = 0;
        size_t index;

        for (int n = 0; n < input.size(); n++) {
            i = (i + 1) % N;
            j = (s[i] + j) % N;
            std::swap(s[i], s[j]);
            index = (s[i] + s[j]) % N;
            output[n] = input[n] ^ s[index];
        }
        return output;
    }

    constexpr std::vector<char8_t> to_vec_u8(std::string_view str) {
        std::vector<char8_t> vec{};
        vec.reserve(str.size());

        for (auto& c: str) {
            vec.push_back(c);
        }
        return vec;
    }

    constexpr std::string to_string(const std::vector<char8_t>& vec) {
        std::string str{};
        str.reserve(vec.size());

        for (auto& c: vec) {
            str += (char) c;
        }
        return str;
    }
}// namespace details

constexpr std::vector<char8_t> rc4(const std::vector<char8_t>& input, const std::vector<char8_t>& key) {
    auto s = details::ksa<s_size>(key);
    return details::rpga(input, s);
}

constexpr std::string rc4(std::string_view input, std::string_view key) {
    return details::to_string(rc4(details::to_vec_u8(input), details::to_vec_u8(key)));
}

constexpr bool is_same_key(const std::vector<char8_t>& input1, const std::vector<char8_t>& input2) {
    if (input1.size() != input2.size()) {
        return false;
    }

    for (int i = 0; i < input1.size(); i++) {
        unsigned char test = input1[i] ^ input2[i];
        if (test >= 0x80) {  // >= 256
            return false;
        }
    }
    return true;
}

#endif//BEZPIEKA_RC4_HPP

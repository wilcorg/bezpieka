#ifndef BEZPIEKA_RC4_CRACK_HPP
#define BEZPIEKA_RC4_CRACK_HPP

#include "bank_account.hpp"
#include "rc4.hpp"

#include <format>
#include <unordered_map>

//namespace details
//{
//    template<size_t N>
//    constexpr std::string to_string(const std::array<uint8_t, N> arr) {
//        std::string str{};
//        str.reserve(N);
//
//        for (auto& c: arr) {
//            str += std::to_string(c);
//        }
//        return str;
//    }
//
//    std::vector<std::string> prepare_dict() {
//        std::vector<std::string> vec{};
//        vec.reserve(bank_id.size() * 100);
//
//        for (auto& entry: bank_id) {
//            auto bank_nr_str = details::to_string<8>(entry.second);
//
//            for (size_t i = 0; i < 100; i++) {
//                vec.push_back(std::format("{:02}", i) + bank_nr_str);
//            }
//        }
//        return vec;
//    }
//}// namespace details

std::string crack(std::string_view known_plain, std::string_view known_enc, std::string&& target) {
    bool is_ok = true;
    std::string decrypted{};
    decrypted.reserve(target.size());

    for (size_t i = 0; i < target.size(); i++) {
        char8_t p = known_plain[i % known_plain.size()];
        char8_t c1 = known_enc[i % known_enc.size()];
        char8_t c2 = target[i];
        char8_t dc = ((p ^ c1) ^ c2);
        if (!isdigit(dc)) {
            is_ok = false;
            break;
        } else {
            decrypted += (char) dc;
        }
    }

    if (is_ok) {
        return decrypted;
    } else {
        return {};
    }
}

#endif//BEZPIEKA_RC4_CRACK_HPP

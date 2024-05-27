#ifndef BEZPIEKA_BANK_ACCOUNT_HPP
#define BEZPIEKA_BANK_ACCOUNT_HPP

#include "rc4.hpp"

#include <array>
#include <cstdint>
#include <format>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

enum class BankName {
    NBP,
    MILLENNIUM,
    ING,
    SANTANDER,
    PKOBP
};

const std::unordered_map<BankName, std::array<uint8_t, 8>> bank_id = {
        {BankName::NBP, {1, 0, 1, 0, 0, 0, 0, 0}},
        {BankName::MILLENNIUM, {1, 1, 6, 0, 0, 0, 0, 6}},
        {BankName::ING, {1, 0, 5, 0, 0, 0, 0, 2}},
        {BankName::SANTANDER, {2, 1, 2, 0, 0, 0, 0, 1}},
        {BankName::PKOBP, {1, 0, 2, 0, 0, 0, 0, 3}},
};

std::vector<std::string> gen_bank_accounts(size_t n) {
    namespace mp = boost::multiprecision;
    std::vector<std::string> bank_accounts{};
    bank_accounts.reserve(n);
    std::random_device rd;
    std::mt19937 rng(rd());

    for (auto& entry: bank_id) {
        auto& nr = entry.second;

        for (int num = 0; num < n; num++) {
            std::string bank_account_str{};

            std::array<uint8_t, 16> client_id{};
            for (int i = 0; i < 16; i++) {
                client_id[i] = (uint8_t) rng() % 10;
            }
            // PL i 2 zera
            mp::uint128_t bank_account = 2125 * 100;
            for (size_t i = 0; i < nr.size(); i++) {
                bank_account += nr[i] * mp::pow((mp::uint128_t) 10, 7 - i + 21);
            }
            for (int i = 0; i < 16; i++) {
                bank_account += client_id[i] * mp::pow((mp::uint128_t) 10, 15 - i + 5);
            }

            bank_account %= 97;
            bank_account = 98 - bank_account;
            // 1  -> 01
            // 9  -> 09
            // 12 -> 12
            bank_account_str += std::format("{:0>2}", bank_account.str());
            for (auto& i: nr) {
                bank_account_str += std::to_string(i);
            }
            for (auto& i: client_id) {
                bank_account_str += std::to_string(i);
            }
            bank_accounts.push_back(bank_account_str);
        }
    }
    return bank_accounts;
}

std::vector<std::string> encrypt_bank_accounts(const std::vector<std::string>& bank_accounts, const std::string& key) {
    std::vector<std::string> encrypted_accounts{};
    encrypted_accounts.reserve(bank_accounts.size());

    for (auto& account: bank_accounts) {
        encrypted_accounts.push_back(rc4(account, key));
    }
    return encrypted_accounts;
}

#endif//BEZPIEKA_BANK_ACCOUNT_HPP

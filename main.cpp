#include "bank_account.hpp"
#include "rc4.hpp"
#include "rc4_crack.hpp"
#include <cassert>
#include <iostream>

int main() {
    std::string message1 = "12345600";
    std::string message2 = "12345601";
    std::string key{"110"};
    auto encrypted1 = rc4(message1, key);
    auto encrypted2 = rc4(message2, key);
    assert(is_same_key(details::to_vec_u8(encrypted1), details::to_vec_u8(encrypted2)));

    auto bank_accounts = gen_bank_accounts(10);
    auto encrypted_accounts = encrypt_bank_accounts(bank_accounts, key);

    auto palec_do_nieba = random() % bank_accounts.size();
//    auto dict = details::prepare_dict();
    // dobieramy known_plain do pierwszego lepszego known_enc brute forcem
    // (5 * 2 ^ 16 niezależnych od sobie przypadków, mniej niż w nomore)
    // obliczenia można rozłożyć na wiele wątków CPU lub GPU
    auto cracked = crack(bank_accounts[0],
                         rc4(bank_accounts[0], key),
                         rc4(bank_accounts[palec_do_nieba], key));

    std::cout << "target:  " << bank_accounts[palec_do_nieba] << std::endl;
    std::cout << "cracked: " << cracked << std::endl;
    return 0;
}

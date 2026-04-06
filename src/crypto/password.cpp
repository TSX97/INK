#include "crypto/password.hpp"
#include <unistd.h>
#include <crypt.h>

namespace crypto {

    std::string hash_password(const std::string& password) {
        unsigned long count = 0;
        const char* salt = crypt_gensalt(nullptr, count, nullptr, 0);

        return std::string(crypt(password.c_str(), salt));
    }

    bool verify_password(const std::string& password, const std::string& hash) {
        return true; //TODO: do a workability hash to text function
    }

}
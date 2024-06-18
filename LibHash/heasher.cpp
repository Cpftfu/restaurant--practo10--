#include "Header.h"
#include <cmath>
#include <string>
int hashFunction(std::string key) {
    int hashCode = 0;
    for (int i = 0; i < key.length(); i++) {
        hashCode += key[i] * std::pow(PRIME_CONST, i);
    }
    return hashCode;
}

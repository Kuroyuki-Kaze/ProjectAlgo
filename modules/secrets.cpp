#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

#include "secrets.hpp"

uint32_t rightRotate(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

std::string SHA256(std::string input) {
    // SHA256 without libsodium
    
    // convert input std::string to a vector of bits
    std::vector<bool> input_bits;
    for (char c : input) {
        for (int i = 0; i < 8; i++) {
            input_bits.push_back((c >> i) & 1);
        }
    }

    // append a 1/true to the end of the vector
    input_bits.push_back(true);

    // pad with 0 until the length is a multiple of 512, less 64 bits
    while (input_bits.size() % 512 != 448) {
        input_bits.push_back(false);
    }

    // append the length of the input in bits (big-endian)
    for (int i = 0; i < 64; i++) {
        input_bits.push_back((input.size() * 8) >> (63 - i));
    }

    // initialize the hash
    uint32_t h0 = 0x6a09e667;
    uint32_t h1 = 0xbb67ae85;
    uint32_t h2 = 0x3c6ef372;
    uint32_t h3 = 0xa54ff53a;
    uint32_t h4 = 0x510e527f;
    uint32_t h5 = 0x9b05688c;
    uint32_t h6 = 0x1f83d9ab;
    uint32_t h7 = 0x5be0cd19;
    
    // initialize round constants
    std::vector<uint32_t> k = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    // chunk loop
    for (unsigned i = 0; i < input_bits.size() / 512; i++) {
        // create message schedule
        std::vector<uint32_t> w;
        for (int j = 0; j < 64; j++) {
            w.push_back(0);
        }

        // copy chunk into message schedule
        for (int j = 0; j < 512; j++) {
            w[j / 32] |= (input_bits[i * 512 + j] << (31 - (j % 32)));
        }

        // extend the schedule
        for (int j = 16; j < 64; j++) {
            uint32_t s0 = rightRotate(w[j - 15], 7) ^ rightRotate(w[j - 15], 18) ^ (w[j - 15] >> 3);
            uint32_t s1 = rightRotate(w[j - 2], 17) ^ rightRotate(w[j - 2], 19) ^ (w[j - 2] >> 10);
            w[j] = w[j - 16] + s0 + w[j - 7] + s1;
        }

        // initialize the working variables
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;
        uint32_t f = h5;
        uint32_t g = h6;
        uint32_t h = h7;

        // perform the rounds
        for (int j = 0; j < 64; j++) {
            uint32_t S1 = rightRotate(e, 6) ^ rightRotate(e, 11) ^ rightRotate(e, 25);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = h + S1 + ch + k[j] + w[j];
            uint32_t S0 = rightRotate(a, 2) ^ rightRotate(a, 13) ^ rightRotate(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        // add the working variables to the hash
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
        h5 += f;
        h6 += g;
        h7 += h;
    }

    // convert hash to hex
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    ss << std::setw(8) << h0 << std::setw(8) << h1 << std::setw(8) << h2 << std::setw(8) << h3 << std::setw(8) << h4 << std::setw(8) << h5 << std::setw(8) << h6 << std::setw(8) << h7;
    return ss.str();
}
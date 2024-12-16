#include <cstddef>
#include <iostream>
#include <ctime>
#include <vector>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/cryptlib.h>

std::string toHex(std::vector<unsigned char> byte_array) {
    std::string digest;

    CryptoPP::ArraySource as(byte_array.data(), byte_array.size(), true,
        new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(digest)
        )
    );

    return digest;
}

class Block {
    int index;
    time_t timestamp;
    std::vector<unsigned char> prev_hash;
    std::vector<unsigned char> hash;
    std::string data;

    public:
        // For a regular block
        Block(int index, std::vector<unsigned char> prev_hash, std::string data) {
            this->index = index;
            this->timestamp = time(0);
            this->prev_hash = prev_hash;
            this->data = data;
            this->hash = calculate_hash();
        }

        // For the genesis block
        Block(int index, std::string data) {
            this->index = index;
            this->timestamp = time(0);
            this->prev_hash.resize(32, 0);
            this->data = data;
            this->hash = calculate_hash();
        }

        std::vector<unsigned char> calculate_hash() {
            std::vector<unsigned char> buffer;

            // Turn every piece of info into binary data and add to buffer to use for hashing
            // Convert block index from integer form to binary form in little endian format
            for (int i = 0; i < sizeof(this->index); i++) {
                buffer.push_back((this->index >> (i * 8)) & 0xFF);
            }

            // Same process to convert timestamp into binary data, as timestamp on most platform in uint64_t
            for (int i = 0; i < sizeof(this->timestamp); i++) {
                buffer.push_back((this->timestamp >> (i * 8)) & 0xFF);
            }

            std::vector<unsigned char> str_byte(this->data.begin(), this->data.end());
            // Use a for loop to iterate through data string bytes, and append to buffer vector
            // Less expensive operation than insert
            for (unsigned char byte : str_byte) {
                buffer.push_back(byte);
            }

            // Add the previous hash binary data to the buffer
            for (unsigned char byte : prev_hash) {
                buffer.push_back(byte);
            }

            CryptoPP::SHA256 hash;
            std::vector<unsigned char> digest(hash.DigestSize());

            try {
                // compute hash
                hash.Update(buffer.data(), buffer.size());
                hash.Final(digest.data());
           } catch(const CryptoPP::Exception& e) {
                std::cerr << e.what() << std::endl;
                exit(1);
            }

            // return hash
            return digest;
        }

        // Display information about the block
        void display_block() {
            std::cout << "Block index: " << this->index << std::endl;
            std::cout << "Timestamp: " << this->timestamp << std::endl;
            std::cout << "Data: " << this->data << std::endl;
            std::cout << "Previous Hash: " << toHex(this->prev_hash) << std::endl;
            std::cout << "Block Hash: " << toHex(this->hash) << std::endl;
        }
};

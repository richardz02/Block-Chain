#include <iostream>
#include <vector>
#include "block.hpp"

int main() {
    std::vector<Block> blockChain;

    Block block1(1, "Genesis block");
    block1.display_block();

    Block block2(2, block1.calculate_hash(), "Second block");
    block2.display_block();
    return 0;
}

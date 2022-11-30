#include "randomgenerator.h"

RandomGenerator::RandomGenerator(unsigned int seed) : m_Generator(seed) {

}

int RandomGenerator::generate(int bottom, int top) noexcept {
    std::uniform_int_distribution<int> dist(bottom, top);
    return dist(m_Generator);
}

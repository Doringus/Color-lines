#include "randomgenerator.h"

RandomGenerator::RandomGenerator(std::random_device randomDevice) : m_Generator(randomDevice()) {

}

int RandomGenerator::generate(int bottom, int top) noexcept {
    std::uniform_int_distribution<int> dist(bottom, top);
    return dist(m_Generator);
}

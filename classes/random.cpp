#include <random.h>
#include <random>

float Random::generateRandomFloat(float lower, float upper) {
    std::random_device random;
    std::mt19937 gen(random());
    std::uniform_real_distribution<> distribution(lower, upper);
    
    float randomNumber = std::round(10. * distribution(gen)) / 10;
    return randomNumber;
}
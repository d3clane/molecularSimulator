#include "Utils/Rand.hpp"

#include <random>

namespace Utils
{

double Rand(const double min, const double max)
{
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}

} // namespace Utils
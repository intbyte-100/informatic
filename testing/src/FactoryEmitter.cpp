#include "FactoryEmitter.h"
#include <stdexcept>


FactoryEmitter::FactoryEmitter(double kwhPerDay, double co2PerKwh) : kwhPerDay(kwhPerDay), co2PerKwh(co2PerKwh) {
    if (kwhPerDay <= 0 || co2PerKwh <= 0) {
        throw std::invalid_argument("Invalid input: kwhPerDay and co2PerKwh must be positive");
    }
}

double FactoryEmitter::DailyCO2() {
    return kwhPerDay * co2PerKwh;
}



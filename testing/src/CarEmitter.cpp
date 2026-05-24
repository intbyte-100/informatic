#include "CarEmitter.h"
#include <stdexcept>



CarEmitter::CarEmitter(double literPerDay, double co2PerLiter) : literPerDay(literPerDay), co2PerLiter(co2PerLiter) {
    if (literPerDay < 0 || co2PerLiter < 0) {
        throw std::invalid_argument("Invalid input: literPerDay and co2PerLiter must be positive");
    }
}

double CarEmitter::DailyCO2() {
    return literPerDay * co2PerLiter;
}
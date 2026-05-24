#include "Emitter.h"

double Emitter::YearlyCO2() {
    return DailyCO2() * 365;
}
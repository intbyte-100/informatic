#pragma once
#include "Emitter.h"

class CarEmitter : public Emitter {
private:
    double literPerDay;
    double co2PerLiter;
public:
    CarEmitter(double literPerDay, double co2PerLiter);
    double DailyCO2() override;
};

#pragma once

#include "Emitter.h"

class FactoryEmitter : public Emitter {
private:
    double kwhPerDay;
    double co2PerKwh;
public:
    FactoryEmitter(double kwhPerDay, double co2PerKwh);
    virtual double DailyCO2() override;
};

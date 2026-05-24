#pragma once

class Emitter {
public:
    virtual double DailyCO2() = 0;
    virtual double YearlyCO2();
};

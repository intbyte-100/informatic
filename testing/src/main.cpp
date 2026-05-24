#include "CarEmitter.h"
#include "FactoryEmitter.h"
#include <iostream>


int main() {
    
    CarEmitter car(0, 2.5);
    FactoryEmitter factory(100, 2.5);
    
    std::cout << "Yearly CO2 emissions for car: " << car.YearlyCO2() << std::endl;
    std::cout << "Yearly CO2 emissions for factory: " << factory.YearlyCO2() << std::endl;
}
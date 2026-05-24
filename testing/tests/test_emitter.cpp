#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include "CarEmitter.h"
#include "FactoryEmitter.h"
#include "Emitter.h"

TEST(CarEmitterBasic, DailyAndYearlyComputation) {
    CarEmitter car(10.0, 2.0); 
    CarEmitter car_with_zero(0, 0);
    
    EXPECT_DOUBLE_EQ(car.DailyCO2(), 20.0);
    EXPECT_DOUBLE_EQ(car.YearlyCO2(), 20.0 * 365);
    
    EXPECT_DOUBLE_EQ(car_with_zero.DailyCO2(), 0);
    EXPECT_DOUBLE_EQ(car_with_zero.YearlyCO2(), 0);
}


TEST(CarEmitterConstructor, ThrowsOnNonPositiveInputs) {
    EXPECT_THROW(CarEmitter(-1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(CarEmitter(1.0, -2.0), std::invalid_argument);
}


TEST(EmitterPolymorphism, YearlyFromBasePointer) {
    std::unique_ptr<Emitter> e = std::make_unique<CarEmitter>(1.5, 3.0);
    
    EXPECT_DOUBLE_EQ(e->DailyCO2(), 1.5 * 3.0);
    EXPECT_DOUBLE_EQ(e->YearlyCO2(), (1.5 * 3.0) * 365);
}

TEST(FactoryEmitterBasic, DailyAndYearlyComputation) {
    FactoryEmitter factory(100.0, 2.5);
    
    EXPECT_DOUBLE_EQ(factory.DailyCO2(), 100.0 * 2.5);
    EXPECT_DOUBLE_EQ(factory.YearlyCO2(), (100.0 * 2.5) * 365);
}

TEST(FactoryEmitterConstructor, ThrowsOnNonPositiveInputs) {
    EXPECT_THROW(FactoryEmitter(0.0, 1.0), std::invalid_argument);
    EXPECT_THROW(FactoryEmitter(1.0, 0.0), std::invalid_argument);
    EXPECT_THROW(FactoryEmitter(-1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(FactoryEmitter(1.0, -2.0), std::invalid_argument);
}

TEST(EmitterPolymorphismFactory, YearlyFromBasePointer) {
    std::unique_ptr<Emitter> e = std::make_unique<FactoryEmitter>(200.0, 0.1);
    
    EXPECT_DOUBLE_EQ(e->DailyCO2(), 200.0 * 0.1);
    EXPECT_DOUBLE_EQ(e->YearlyCO2(), (200.0 * 0.1) * 365);
}


#define CATCH_CONFIG_MAIN

#include "Polynomial.h"
#include "catch.hpp"

const long double eps = 1e-3;

TEST_CASE("Single Term (Degree 0)") {
    std::vector<long double> coefficients = {1.0};
    Polynomial a(coefficients);
    REQUIRE(a.evaluate(1) == 1);
    REQUIRE(a.evaluate(-1) == 1);
    REQUIRE(a.evaluate(INT64_MAX) == 1);
}

TEST_CASE("Single Nonzero Term (Degree 0)") {
    std::vector<long double> coefficients = {1.0, 0, 0, 0};
    Polynomial a(coefficients);
    REQUIRE(a.evaluate(1) == 1);
    REQUIRE(a.evaluate(-1) == 1);
    REQUIRE(a.evaluate(INT64_MAX) == 1);
}

TEST_CASE("Multiple Nonzero Terms") {
    std::vector<long double> coefficients = {1.0, 2.0, 3.0, 4.0};
    Polynomial a(coefficients);
    REQUIRE(a.evaluate(0) == 1);
    REQUIRE(a.evaluate(1) == 10);
    REQUIRE(a.evaluate(2) == 49);
    REQUIRE(a.evaluate(-1) == -2);
}

TEST_CASE("All Negative Coefficients") {
    std::vector<long double> coefficients = {-1.0, -2.0, -3.0, -4.0};
    Polynomial a(coefficients);
    REQUIRE(a.evaluate(0) == -1);
    REQUIRE(a.evaluate(1) == -10);
    REQUIRE(a.evaluate(2) == -49);
    REQUIRE(a.evaluate(-1) == 2);
}

TEST_CASE("Negative and Positive Coefficients") {
    std::vector<long double> coefficients = {-1.0,   12.0,  -32.0,
                                             -214.0, 100.0, 7};
    Polynomial a(coefficients);
    REQUIRE(a.evaluate(0) == -1);
    REQUIRE(a.evaluate(1) == -128);
    REQUIRE(a.evaluate(2) == 7);
    REQUIRE(a.evaluate(-1) == 262);
}

TEST_CASE("Decimal Coefficients") {
    std::vector<long double> coefficients = {-23.2938, 2.23158,  -6.3789, -23.6,
                                             50.656,   0.007654, 80.1029};
    Polynomial a(coefficients);
    REQUIRE(abs(a.evaluate(0) - -23.2938) < eps);
    REQUIRE(abs(a.evaluate(1) - 79.7254) < eps);
    REQUIRE(abs(a.evaluate(2) - 5704.1803) < eps);
    REQUIRE(abs(a.evaluate(-1) - 122.4470) < eps);
}

TEST_CASE("Single Point Interpolation") {
    std::vector<std::pair<long double, long double>> pts = {{5, 1}};
    Polynomial a(pts);
    REQUIRE(a.interpolate(1) == 1);
    REQUIRE(a.interpolate(-1) == 1);
    REQUIRE(a.interpolate(INT64_MAX) == 1);
}

TEST_CASE("Negative Points Interpolation") {
    std::vector<std::pair<long double, long double>> pts = {{-1, -1}, {-2, -2}};
    Polynomial a(pts);
    REQUIRE(a.interpolate(1) == 1);
    REQUIRE(a.interpolate(0) == 0);
    REQUIRE(a.interpolate(INT64_MAX) == INT64_MAX);
    REQUIRE(a.interpolate(INT64_MIN) == INT64_MIN);
}

TEST_CASE("Mixed Points Interpolation") {
    std::vector<std::pair<long double, long double>> pts = {
        {1, -1}, {-2, 5.2}, {3, 33}, {-4, -7}, {5, 17}};
    Polynomial a(pts);
    REQUIRE(a.interpolate(1) == -1);
    REQUIRE(a.interpolate(0) == -7.8);
    REQUIRE(abs(a.interpolate(1.5) - 6.1519) < eps);
    REQUIRE(abs(a.interpolate(6.8) + 183.132) < eps);
    REQUIRE(abs(a.interpolate(3.1) - 34.4621) < eps);
    REQUIRE(abs(a.interpolate(2.4) - 22.5444) < eps);
}
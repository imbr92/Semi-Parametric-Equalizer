#include "Polynomial.h"

Polynomial::Polynomial(std::vector<std::pair<long double, long double>>& pts) {
    form = false;
    for (auto i : pts) {
        points.push_back(i);
    }
}

Polynomial::Polynomial(std::vector<long double>& poly) {
    form = true;
    for (auto i : poly) {
        coefficients.push_back(i);
    }
}

long double Polynomial::evaluate(long double x) {
    long double ret = 0;
    long double mult = 1;
    for (auto i : coefficients) {
        ret += mult * i;
        mult *= x;
    }
    return ret;
}

// based off of https://www.geeksforgeeks.org/lagranges-interpolation/
long double Polynomial::interpolate(long double x) {
    long double ret = 0;
    for (const auto& i : points) {
        long double term = i.second;
        for (const auto& j : points) {
            if (j != i) term *= (x - j.first) / (i.first - j.first);
        }
        ret += term;
    }
    return ret;
}

std::vector<long double> Polynomial::graph(int lower_bound, int upper_bound) {
    std::vector<long double> ret;
    if (form) {
        for (int i = lower_bound; i <= upper_bound; ++i) {
            ret.push_back(evaluate(i));
        }
    } else {
        for (int i = lower_bound; i <= upper_bound; ++i) {
            ret.push_back(interpolate(i));
        }
    }
    return ret;
}
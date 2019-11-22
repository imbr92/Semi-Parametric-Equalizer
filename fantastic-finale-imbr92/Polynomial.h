#include <iostream>
#include <vector>

class Polynomial {
   public:
    Polynomial(std::vector<std::pair<long double, long double>>& pts);
    Polynomial(std::vector<long double>& poly);
    long double evaluate(long double x_value);
    long double interpolate(long double x);
    // [lower_bound, upper_bound] inclusive
    std::vector<long double> graph(int lower_bound, int upper_bound);

   private:
    // true = coefficients, false = interpolated
    bool form;
    std::vector<long double> coefficients;
    std::vector<std::pair<long double, long double>> points;
};

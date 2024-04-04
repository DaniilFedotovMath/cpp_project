#include <include/modelling.h>

#include <include/correlation.h>
#include <include/utils.h>

#include <iostream>
#include <random>

namespace modelling {

namespace {

template<class Type>
void removePoint(std::vector<Type>& data, const Type& el)
{
    auto it = std::find(data.begin(), data.end(), el);
    if (it != data.end()) {
        data.erase(it);
    }
}

} // namespace

std::vector<Eigen::Vector2i> Model::compute(size_t iterCount)
{
    math::FedotovCorrelation correlation;
    correlation.setLaplacianN(10);
    correlation.setLaplacianM(20);
    correlation.setLaplacianScale(100000);
    correlation.setN(500);
    correlation.setLength(5000);
    correlation.addDetector({101, 0}, 500);
    correlation.addDetector({102, 0}, 500);
    correlation.setTimeInterval(0.1);
    correlation.setStep(0.00002);

    std::vector<Eigen::Vector2i> result;
    const auto calculateCorrelation = [&]() {
        correlation.build(result);
        correlation.setRow(66);
        const auto c1 = correlation.calculate(func1_);
        correlation.setRow(21);
        const auto c2 = correlation.calculate(func2_);
        return std::pair{c1, c2};
    };
    auto [cor1, cor2] = calculateCorrelation();
    double newCor1;
    double newCor2;
    std::cout << cor1 << " " << cor2 << std::endl;

    size_t retryCount = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, points_.size() - 1);
    std::uniform_real_distribution<> dis(0, 1);

    for (size_t iterNum = 0; iterNum < iterCount; ++iterNum) {
        ++retryCount;
        if (retryCount > 2 * points_.size()) {
            result.clear();
            std::tie(cor1, cor2) = calculateCorrelation();
        }
        std::cout << "Iteration " << iterNum << std::endl;
        if (std::abs(cor1 - 1) < precision_) {
            break;
        }
        bool flag = false;
        const auto& point = points_[distr(gen)];
        std::cout << "(" << point[0] << ", " << point[1] << ")" << std::endl;
        if (IsIn(result, point)) {
            removePoint(result, point);
            flag = true;
        } else {
            result.push_back(point);
        }
        std::tie(newCor1, newCor2) = calculateCorrelation();
        if ((newCor1 > cor1) && (newCor2 > cor2)) {
            cor1 = newCor1;
            cor2 = newCor2;
            retryCount = 0;
            std::cout << "We accepted (" << point[0] << ", " << point[1] << ") new corr1 = " << cor1
                      << " new corr2 = " << cor2 << std::endl;
        } else {
            const auto rnd = dis(gen);
            const auto acceptCoef =
                std::exp(temperatureCoef_ * std::min(newCor1 - cor1, newCor2 - cor2));
            std::cout << "Probability of acceptance = " << acceptCoef << std::endl;
            std::cout << "random number = " << rnd << std::endl;
            if (rnd < acceptCoef) {
                cor1 = newCor1;
                cor2 = newCor2;
                retryCount = 0;
                std::cout << "We accepted (" << point[0] << ", " << point[1]
                          << ") new corr1=" << cor1 << " new corr2=" << cor2 << std::endl;
            } else {
                if (flag) {
                    result.push_back(point);
                } else {
                    removePoint(result, point);
                }
            }
        }
        std::cout << "corr1 after iteration = " << cor1 << " corr2 after iteration = " << cor2
                  << std::endl;
        if (std::abs(cor1 - 1) < precision_) {
            break;
        }
    }
    return result;
}

} // namespace modelling

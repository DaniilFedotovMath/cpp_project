#pragma once

#include <Eigen/Dense>

#include <vector>

namespace modelling {

class Model {
public:
    Model(const Eigen::VectorXd& func1, const Eigen::VectorXd& func2) noexcept
        : func1_(func1)
        , func2_(func2)
    { }

    void addPoint(const Eigen::Vector2i& point) { points_.push_back(point); }

    void setTemperatureCoef(double temperatureCoef) noexcept { temperatureCoef_ = temperatureCoef; }

    void setPrecision(double precision) noexcept { precision_ = precision; }

    std::vector<Eigen::Vector2i> compute(size_t iterCount);

private:
    Eigen::VectorXd func1_;
    Eigen::VectorXd func2_;
    std::vector<Eigen::Vector2i> points_;
    double temperatureCoef_;
    double precision_;
};

} // namespace modelling

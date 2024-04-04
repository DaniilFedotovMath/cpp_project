#pragma once

#include <Eigen/Dense>

#include <functional>

namespace math {

Eigen::VectorXd heunStep(
    const Eigen::VectorXd& state,
    const Eigen::MatrixXd& K,
    const std::function<Eigen::VectorXd(const Eigen::VectorXd&, const Eigen::MatrixXd&)>& rhs,
    double dt,
    double etol = 0.000001,
    size_t maxIters = 100);

} // namespace math

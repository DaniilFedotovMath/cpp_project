#pragma once

#include <Eigen/Dense>

namespace math {

Eigen::VectorXd waveEquation(const Eigen::VectorXd& state, const Eigen::MatrixXd& K);

} // namespace math

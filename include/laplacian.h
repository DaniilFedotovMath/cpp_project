#pragma once

#include <Eigen/Dense>

#include <vector>

namespace math {

Eigen::MatrixXd makeLaplacian(
    const std::vector<Eigen::Vector2i>& defects, size_t n, size_t m, double scale = 1.0);

} // namespace math

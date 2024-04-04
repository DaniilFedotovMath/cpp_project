#include <include/utils.h>

#include <Eigen/Dense>

#include <vector>

namespace math {

Eigen::MatrixXd makeLaplacian(
    const std::vector<Eigen::Vector2i>& defects, size_t n, size_t m, double scale = 1.0)
{
    const size_t size = n * m;
    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(size, size);

    for (size_t i = 0; i < size; ++i) {
        A(i, i) = 4;
        if (i % n == 0) {
            A(i, i + 1) = -2;
        } else if (i % n == n - 1) {
            A(i, i - 1) = -2;
        } else {
            A(i, i + 1) = -1;
            A(i, i - 1) = -1;
        }

        if (i < n) {
            A(i, i + n) = -2;
        } else if (i >= n * (m - 1)) {
            A(i, i - n) = -2;
        } else {
            A(i, i - n) = -1;
            A(i, i + n) = -1;
        }
    }

    for (const auto& point: defects) {
        A(point(0), point(1)) = 0;
        A(point(1), point(0)) = 0;
        A(point(0), (2 * point(0) + size - point(1)) % size) = -2;
        A(point(1), (2 * point(1) + size - point(0)) % size) = -2;

        const auto minCoord = std::min(point(0), point(1));
        if (minCoord / 10 == 0) {
            A(minCoord, minCoord) = 2;
            A(minCoord, (minCoord + size - 1) % size) = 0;
            A((minCoord + size - 1) % size, minCoord) = 0;
        }
        if (IsIn(defects, Eigen::Vector2i{minCoord, (minCoord + size - 1) % size}) &&
            IsIn(defects, Eigen::Vector2i{minCoord, (minCoord + 1) % size})) {
            A(minCoord, minCoord) -= 2;
            A(minCoord, (minCoord + 1) % size) = 0;
            A((minCoord + 1) % size, minCoord) = 0;
            A((minCoord + size - 1) % size, minCoord) = 0;
            A(minCoord, (minCoord + size - 1) % size) = 0;
        }
        if (IsIn(defects, Eigen::Vector2i{minCoord, (minCoord + 10) % size}) &&
            IsIn(defects, Eigen::Vector2i{(minCoord + size - 10) % size, minCoord})) {
            A(minCoord, minCoord) -= 2;
            A(minCoord, (minCoord + 10) % size) = 0;
            A((minCoord + 10) % size, minCoord) = 0;
            A((minCoord + size - 10) % size, minCoord) = 0;
            A(minCoord, (minCoord + size - 10) % size) = 0;
        }
    }
    return A * scale;
}

} // namespace math

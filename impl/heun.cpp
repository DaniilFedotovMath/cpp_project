#include <include/wave_function.h>

#include <Eigen/Dense>

namespace math {

Eigen::VectorXd heunStep(
    const Eigen::VectorXd& state,
    const Eigen::MatrixXd& K,
    const std::function<Eigen::VectorXd(const Eigen::VectorXd&, const Eigen::MatrixXd&)>& rhs,
    double dt,
    double etol,
    size_t maxIters)
{
    const auto eps = std::numeric_limits<double>::epsilon();
    Eigen::VectorXd nextState = state + rhs(state, K) * dt;
    for (size_t n = 0; n < maxIters; ++n) {
        auto nextStateOld = nextState;
        nextState = state + (rhs(state, K) + rhs(nextState, K)) / 2 * dt;
        double e = 0.0;
        for (size_t i = 0; i < state.size(); ++i) {
            e += std::abs((nextState(i) - nextStateOld(i)) / (nextState(i) + eps));
        }
        if (e < etol) {
            break;
        }
    }
    return nextState;
}

} // namespace math

#include <include/wave_function.h>

#include <Eigen/Dense>

namespace math {

Eigen::VectorXd waveEquation(const Eigen::VectorXd& state, const Eigen::MatrixXd& K)
{
    Eigen::VectorXd dstate = Eigen::VectorXd::Zero(state.size()); // Initialize dstate with zeros

    size_t i = 0;

    // Splitting and operations similar to the Python version
    for (; i < state.size() / 2; ++i) {
        dstate(i) = state(i + state.size() / 2);
    }

    // Matrix-vector multiplication for the second half
    Eigen::VectorXd result = -K * state.head(state.size() / 2);
    for (; i < state.size(); ++i) {
        dstate(i) = result(i - state.size() / 2);
    }

    return dstate;
}

} // namespace math

#include <include/correlation.h>

#include <include/heun.h>
#include <include/laplacian.h>
#include <include/utils.h>
#include <include/wave_function.h>

#include <Eigen/Dense>

namespace math {

double PearsonCorrelation::calculate(const Eigen::VectorXd& vec) const
{
    ASSERT(vec_.size() == vec.size());
    double mean_x = vec_.mean();
    double mean_y = vec.mean();
    double sum_xy = 0.0;
    double sum_xx = 0.0;
    double sum_yy = 0.0;
    for (size_t i = 0; i < vec_.size(); ++i) {
        sum_xy += (vec_[i] - mean_x) * (vec[i] - mean_y);
        sum_xx += (vec_[i] - mean_x) * (vec_[i] - mean_x);
        sum_yy += (vec[i] - mean_y) * (vec[i] - mean_y);
    }
    return sum_xy / (sqrt(sum_xx) * sqrt(sum_yy));
}

double pearsonCorrelation(const Eigen::VectorXd& x, const Eigen::VectorXd& y)
{
    return PearsonCorrelation(x).calculate(y);
}

void FedotovCorrelation::build(const std::vector<Eigen::Vector2i>& defects)
{
    const auto K = makeLaplacian(defects, laplacianN_, laplacianM_, laplacianScale_);
    const auto tsteps = static_cast<size_t>(timeInterval_ / step_);
    result_ = Eigen::MatrixXd::Zero(n_, tsteps);
    for (const auto& detector: detectors_) {
        result_(detector.first[0], detector.first[1]) = detector.second;
    }
    Eigen::MatrixXd dy = Eigen::MatrixXd::Zero(n_, tsteps);

    for (size_t i = 0; i < tsteps - 1; ++i) {
        Eigen::VectorXd state =
            Eigen::VectorXd::Zero(n_ * 2); // Assuming state concatenates y and dy
        for (size_t j = 0; j < n_; ++j) {
            state(j) = result_(j, i);
            state(j + n_) = dy(j, i);
        }

        Eigen::VectorXd next_state = heunStep(state, K, waveEquation, step_);
        for (size_t j = 0; j < n_; ++j) {
            result_(j, i + 1) = next_state(j);
            dy(j, i + 1) = next_state(j + n_);
        }
    }
}

double FedotovCorrelation::calculate(const Eigen::VectorXd& vec) const
{
    // Extracting specific function results and computing correlations
    return pearsonCorrelation(result_.row(row_).head(length_), vec.head(length_));
}

} // namespace math

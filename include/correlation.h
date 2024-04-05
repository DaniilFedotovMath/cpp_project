#pragma once

#include <Eigen/Dense>

#include <vector>

namespace math {

class Correlation {
public:
    virtual double calculate(const Eigen::VectorXd& vec) const = 0;
};

class PearsonCorrelation : public Correlation {
public:
    PearsonCorrelation(const Eigen::VectorXd& vec) noexcept
        : vec_(vec)
    { }

    double calculate(const Eigen::VectorXd& vec) const override;

private:
    Eigen::VectorXd vec_;
};

double pearsonCorrelation(const Eigen::VectorXd& x, const Eigen::VectorXd& y);

class FedotovCorrelation : public Correlation {
public:
    void build(const std::vector<Eigen::Vector2i>& defects);

    double calculate(const Eigen::VectorXd& vec) const override;

    void setRow(size_t row) noexcept { row_ = row; }

    void addDetector(const Eigen::Vector2i& position, double value)
    {
        detectors_.push_back({position, value});
    }

    void setLength(size_t value) noexcept { length_ = value; }

    void setN(size_t value) noexcept { n_ = value; }

    void setLaplacianN(size_t value) noexcept { laplacianN_ = value; }

    void setLaplacianM(size_t value) noexcept { laplacianM_ = value; }

    void setLaplacianScale(double value) noexcept { laplacianScale_ = value; }

    void setTimeInterval(double value) noexcept { timeInterval_ = value; }

    void setStep(double value) noexcept { step_ = value; }

private:
    std::vector<std::pair<Eigen::Vector2i, double>> detectors_;
    size_t row_;
    size_t laplacianN_;
    size_t laplacianM_;
    size_t n_;
    size_t length_;
    double laplacianScale_;
    double timeInterval_;
    double step_;

    Eigen::MatrixXd result_;
};

} // namespace math

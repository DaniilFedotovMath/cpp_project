#include <include/correlation.h>
#include <include/laplacian.h>
#include <include/modelling.h>
#include <include/utils.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

#include <iostream>

Eigen::VectorXd readVector(std::string_view name)
{
    const auto data = readFileToString(name);
    rapidjson::Document d;
    d.Parse(data.data());
    const auto& array = d.GetArray();
    Eigen::VectorXd result = Eigen::VectorXd::Zero(array.Size());
    for (size_t i = 0; i < array.Size(); ++i) {
        result(i) = array[i].GetDouble();
    }
    return result;
}

int main()
{
    const auto f1 = readVector("wavefunction66with3def_30deg_double_source");
    const auto f2 = readVector("wavefunction21with3def_30deg_double_source");
    std::vector<Eigen::Vector2i> def = {{61, 51}, {52, 42}, {51, 52}};
    std::vector<Eigen::Vector2i> points = {
        {51, 52},
        {52, 53},
        {61, 62},
        {62, 63},
        {71, 72},
        {72, 73},
        {41, 51},
        {51, 61},
        {61, 71},
        {42, 52},
        {52, 62},
        {62, 72},
        {43, 53},
        {53, 63},
        {63, 73},
        {44, 54},
        {54, 64},
        {64, 74}};
    modelling::Model model(f1, f2);
    model.setPrecision(0.00001);
    model.setTemperatureCoef(200);
    for (const auto& point: points) {
        model.addPoint(point);
    }
    for (const auto& point: model.compute(1000)) {
        std::cout << point << std::endl << std::endl;
    }

    return 0;
}

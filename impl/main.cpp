#include <include/correlation.h>
#include <include/laplacian.h>
#include <include/modelling.h>
#include <include/utils.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

#include <iostream>

#include <fstream>

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
    // run the Metropolis algorithm in two modes:
    //one test with output for every iterations
    //another for data aggregation
    bool choiceParameter = 1;
    std::cout << "choose the mode of the program:" << std::endl;
    std::cout << "Press 1 - if you want to see example of how Metropolis algorithm works" << std::endl;
    std::cout << "Press 0 - if you want to run the code for data aggregation (significant computation time >90 min)" << std::endl;
    std::cin >> choiceParameter;
    std::cout << std::endl;
    if (choiceParameter){
        std::cout << model.compute(1000, choiceParameter) << std::endl;
    }
    // here we firstly write the code for the behaviour of the algorithm depending on temperature coefficient
    // secondly we run more iteration for certain value of temperature coefficient which is close to optimum
    if (!choiceParameter){
        std::cout << "Everything will be saved into phase_transition.txt and optimal_config.txt" << std::endl;
        std::ofstream phaseTransition ("phase_transition.txt");
        for (size_t tempCoef = 10; tempCoef < 200; tempCoef*=2){
            phaseTransition << tempCoef << " ";
            model.setTemperatureCoef(tempCoef);
            for (size_t i = 0; i < 10; ++i) {
                phaseTransition << model.compute(300, choiceParameter) << " ";
            }
            phaseTransition << std::endl;
        }
        phaseTransition.close();
        model.setTemperatureCoef(200);
        std::ofstream optimalConfig ("optimal_config.txt");
        for (size_t i = 0; i < 50; ++i) {
            optimalConfig << model.compute(300, choiceParameter) << " ";
        }
        optimalConfig.close();
    }

    return 0;
}

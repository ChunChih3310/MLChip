#ifndef SOFTMAX_H
#define SOFTMAX_H
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

class Softmax
{
    string module_name;

public:
    Softmax(string module_name);
    vector<float> forward(vector<float> input_vector);
};

Softmax::Softmax(string module_name)
{
    /*
    Constructor of Softmax module
    */
    cout << "Initializing " << module_name << "..." << endl;

    this->module_name = module_name;

    cout << "Initializing " << module_name << " done" << endl;
}

vector<float> Softmax::forward(vector<float> input_vector)
{
    /*
    Forward pass of Softmax module
    */
    cout << "Forward pass of " << this->module_name << "..." << endl;

    vector<float> output_vector;
    float sum = 0;
    for (int i = 0; i < input_vector.size(); i++)
    {
        sum += exp(input_vector[i]);
    }

    for (int i = 0; i < input_vector.size(); i++)
    {
        output_vector.push_back(exp(input_vector[i]) / sum);
    }

    return output_vector;
}

#endif
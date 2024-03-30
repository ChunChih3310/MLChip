#ifndef FULLYCONNECTED_H
#define FULLYCONNECTED_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class FullyConnected
{
    string module_name;
    int input_size;
    int output_size;

    vector<vector<float>> weights;
    vector<float> bias;

public:
    FullyConnected(string module_name, int input_size, int output_size);
    void read_weights();
    void show_weights();
    vector<float> forward(vector<float> input_vector);
};

FullyConnected::FullyConnected(string module_name, int input_size, int output_size)
{
    /*
    Constructor of FullyConnected module
    */
    cout << "Initializing " << module_name << "..." << endl;

    this->module_name = module_name;
    this->input_size = input_size;
    this->output_size = output_size;

    // Read weights and biases from file
    this->read_weights();

    cout << "Initializing " << module_name << " done" << endl;
}

void FullyConnected::read_weights()
{
    /*
    Read weights and biases from file
    */
    cout << "Reading weights of " << this->module_name << "..." << endl;

    string data_path = "data/";
    ifstream file(data_path + this->module_name + "_weight.txt");

    // Resize weights and biases
    weights.resize(output_size);
    for (int i = 0; i < output_size; i++)
    {
        weights[i].resize(input_size);
    }
    bias.resize(output_size);

    // Read weights
    for (int i = 0; i < output_size; i++)
    {
        for (int j = 0; j < input_size; j++)
        {
            file >> weights[i][j];
        }
    }

    // Read biases
    for (int i = 0; i < output_size; i++)
    {
        file >> bias[i];
    }

    cout << "Reading weights of " << this->module_name << " done" << endl;
}

void FullyConnected::show_weights()
{
    /*
    Show weights and biases
    */
    cout << "Weights of " << this->module_name << ":" << endl;

    // Show weights
    for (int i = 0; i < output_size; i++)
    {
        for (int j = 0; j < input_size; j++)
        {
            cout << weights[i][j] << " ";
        }
        cout << bias[i] << endl;
    }
}

vector<float> FullyConnected::forward(vector<float> input_vector)
{
    /*
    Forward pass of FullyConnected module
    */
    cout << "Forward pass of " << this->module_name << "..." << endl;

    vector<float> output_vector;
    output_vector.resize(output_size);

    // Iterate over output neurons
    for (int i = 0; i < output_size; i++)
    {
        output_vector[i] = bias[i];

        // Iterate over input neurons
        for (int j = 0; j < input_size; j++)
        {
            output_vector[i] += input_vector[j] * weights[i][j];
        }
    }

    return output_vector;
}

#endif
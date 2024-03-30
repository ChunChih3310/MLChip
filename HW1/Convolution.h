#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

class Convolution
{
    string module_name;
    int input_channels;
    int output_channels;
    int kernel_size;
    int stride;
    int padding;

    vector<vector<vector<vector<float>>>> weights;
    vector<float> bias;

public:
    Convolution(string module_name, int input_channels, int output_channels, int kernel_size, int stride, int padding);
    void read_weights();
    void show_weights();
    vector<vector<vector<float>>> forward(vector<vector<vector<float>>> input_image);
};

Convolution::Convolution(string module_name, int input_channels, int output_channels, int kernel_size, int stride, int padding)
{
    /*
    Constructor of Convolution module
    */
    cout << "Initializing " << module_name << "..." << endl;

    this->module_name = module_name;
    this->input_channels = input_channels;
    this->output_channels = output_channels;
    this->kernel_size = kernel_size;
    this->stride = stride;
    this->padding = padding;

    // Read weights and biases from file
    this->read_weights();

    cout << "Initializing " << module_name << " done" << endl;
}

void Convolution::read_weights()
{
    /*
    Read weights and biases from file
    */
    cout << "Reading weights of " << this->module_name << "..." << endl;

    string data_path = "data/";
    ifstream file(data_path + this->module_name + "_weight.txt");

    // Resize weights and biases
    weights.resize(output_channels);
    for (int i = 0; i < output_channels; i++)
    {
        weights[i].resize(input_channels);
        for (int j = 0; j < input_channels; j++)
        {
            weights[i][j].resize(kernel_size);
            for (int k = 0; k < kernel_size; k++)
            {
                weights[i][j][k].resize(kernel_size);
            }
        }
    }
    bias.resize(output_channels);

    // Read weights from file
    for (int i = 0; i < output_channels; i++)
    {
        for (int j = 0; j < input_channels; j++)
        {
            for (int k = 0; k < kernel_size; k++)
            {
                for (int l = 0; l < kernel_size; l++)
                {
                    file >> weights[i][j][k][l];
                }
            }
        }
    }
    file.close();

    // Read biases from file
    file.open(data_path + this->module_name + "_bias.txt");
    for (int i = 0; i < output_channels; i++)
    {
        file >> bias[i];
    }
    file.close();

    cout << "Reading weights of " << this->module_name << " done" << endl;
}

void Convolution::show_weights()
{
    /*
    Show weights and biases
    */
    cout << "Weights of " << this->module_name << ":" << endl;
    for (int i = 0; i < output_channels; i++)
    {
        for (int j = 0; j < input_channels; j++)
        {
            for (int k = 0; k < kernel_size; k++)
            {
                for (int l = 0; l < kernel_size; l++)
                {
                    cout << weights[i][j][k][l] << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
        cout << endl;
    }

    cout << "Biases of " << this->module_name << ":" << endl;
    for (int i = 0; i < output_channels; i++)
    {
        cout << bias[i] << " ";
    }
    cout << endl;
}

vector<vector<vector<float>>> Convolution::forward(vector<vector<vector<float>>> input_image)
{
    /*
    Forward pass of Convolution module
    Input is a 3D array of size (input_channels, input_height, input_width)
    Output is a 3D array of size (output_channels, output_height, output_width)
    Weights is a 4D array of size (output_channels, input_channels, kernel_size, kernel_size)
    Biases is a 1D array of size (output_channels)
    Padding: zero padding
    */
    cout << "Forward pass of " << this->module_name << "..." << endl;

    int input_height = input_image[0].size();
    int input_width = input_image[0][0].size();
    int output_height = (input_height + 2 * padding - kernel_size) / stride + 1;
    int output_width = (input_width + 2 * padding - kernel_size) / stride + 1;

    // Resize output_image
    vector<vector<vector<float>>> output_image;
    output_image.resize(output_channels);
    for (int i = 0; i < output_channels; i++)
    {
        output_image[i].resize(output_height);
        for (int j = 0; j < output_height; j++)
        {
            output_image[i][j].resize(output_width);
        }
    }

    // Convolution
    for (int i = 0; i < output_channels; i++) // iterate over conv filters
    {
        for (int j = 0; j < output_height; j++) // iterate over height of output image
        {
            for (int k = 0; k < output_width; k++) // iterate over width of output image
            {
                float sum = 0;
                for (int l = 0; l < input_channels; l++) // iterate over input channels
                {
                    for (int m = 0; m < kernel_size; m++) // iterate over height of kernel
                    {
                        for (int n = 0; n < kernel_size; n++) // iterate over width of kernel
                        {
                            int input_height_index = j * stride + m - padding;
                            int input_width_index = k * stride + n - padding;
                            if (input_height_index >= 0 && input_height_index < input_height && input_width_index >= 0 && input_width_index < input_width)
                            {
                                sum += input_image[l][input_height_index][input_width_index] * weights[i][l][m][n];
                            }
                        }
                    }
                }
                output_image[i][j][k] = sum + bias[i];
            }
        }
    }

    cout << "Forward pass of " << this->module_name << " done" << endl;

    return output_image;
}

#endif
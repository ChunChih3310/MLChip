#ifndef MAXPOOL_H
#define MAXPOOL_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class MaxPool
{
    string module_name;
    int kernel_size;
    int stride;

public:
    MaxPool(string module_name, int kernel_size, int stride);
    vector<vector<vector<float>>> forward(vector<vector<vector<float>>> input_image);
};

MaxPool::MaxPool(string module_name, int kernel_size, int stride)
{
    /*
    Constructor of MaxPool module
    */
    cout << "Initializing " << module_name << "..." << endl;

    this->module_name = module_name;
    this->kernel_size = kernel_size;
    this->stride = stride;

    cout << "Initializing " << module_name << " done" << endl;
}

vector<vector<vector<float>>> MaxPool::forward(vector<vector<vector<float>>> input_image)
{
    /*
    Forward pass of MaxPool module
    */
    cout << "Forward pass of " << this->module_name << "..." << endl;

    vector<vector<vector<float>>> output_image;
    int output_height = (input_image[0].size() - this->kernel_size) / this->stride + 1;
    int output_width = (input_image[0][0].size() - this->kernel_size) / this->stride + 1;
    // Resize output image
    output_image.resize(input_image.size());
    for (int i = 0; i < input_image.size(); i++)
    {
        output_image[i].resize(output_height);
        for (int j = 0; j < output_height; j++)
        {
            output_image[i][j].resize(output_width);
        }
    }

    // iterate over channels
    for (int i = 0; i < input_image.size(); i++)
    {
        for (int j = 0; j < output_height; j++)
        {
            for (int k = 0; k < output_width; k++)
            {
                float max_value = input_image[i][j * this->stride][k * this->stride];
                for (int l = 0; l < this->kernel_size; l++)
                {
                    for (int m = 0; m < this->kernel_size; m++)
                    {
                        if (input_image[i][j * this->stride + l][k * this->stride + m] > max_value)
                        {
                            max_value = input_image[i][j * this->stride + l][k * this->stride + m];
                        }
                    }
                }
                output_image[i][j][k] = max_value;
            }
        }
    }

    cout << "Forward pass of " << this->module_name << " done" << endl;

    return output_image;
}

#endif
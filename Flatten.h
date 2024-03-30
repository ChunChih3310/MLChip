#ifndef FLATTEN_H
#define FLATTEN_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Flatten
{
    string module_name;

public:
    Flatten(string module_name);
    vector<float> forward(vector<vector<vector<float>>> input_image);
};

Flatten::Flatten(string module_name)
{
    /*
    Constructor of Flatten module
    */
    cout << "Initializing " << module_name << "..." << endl;

    this->module_name = module_name;

    cout << "Initializing " << module_name << " done" << endl;
}

vector<float> Flatten::forward(vector<vector<vector<float>>> input_image)
{
    /*
    Forward pass of Flatten module
    */
    cout << "Forward pass of " << this->module_name << "..." << endl;

    vector<float> output_image;
    int input_channels = input_image.size();
    int input_height = input_image[0].size();
    int input_width = input_image[0][0].size();

    // Resize output image
    output_image.resize(input_channels * input_height * input_width);

    // iterate over channels
    int idx = 0;
    for (int i = 0; i < input_channels; i++)
    {
        for (int j = 0; j < input_height; j++)
        {
            for (int k = 0; k < input_width; k++)
            {
                output_image[idx] = input_image[i][j][k];
                idx++;
            }
        }
    }

    return output_image;
}

#endif
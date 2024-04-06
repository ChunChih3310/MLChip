#ifndef ADATIVEAVGPOOL_H
#define ADATIVEAVGPOOL_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class AdaptiveAvgPool
{
    string module_name;
    int output_size_h;
    int output_size_w;

public:
    AdaptiveAvgPool(string module_name, int output_size_h, int output_size_w);
    vector<vector<vector<float>>> forward(vector<vector<vector<float>>> input_image);
};

AdaptiveAvgPool::AdaptiveAvgPool(string module_name, int output_size_h, int output_size_w)
{
    /*
    Constructor of AdaptiveAvgPool module
    */
    cout << "Initializing " << module_name << "..." << endl;

    this->module_name = module_name;
    this->output_size_h = output_size_h;
    this->output_size_w = output_size_w;

    cout << "Initializing " << module_name << " done" << endl;
}

vector<vector<vector<float>>> AdaptiveAvgPool::forward(vector<vector<vector<float>>> input_image)
{
    /*
    Forward pass of AdaptiveAvgPool module
    */
    cout << "Forward pass of " << this->module_name << "..." << endl;

    vector<vector<vector<float>>> output_image;
    int input_height = input_image[0].size();
    int input_width = input_image[0][0].size();
    int output_height = this->output_size_h;
    int output_width = this->output_size_w;
    int stride_h = input_height / output_height;
    int stride_w = input_width / output_width;

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
                float sum = 0;
                for (int l = 0; l < stride_h; l++)
                {
                    for (int m = 0; m < stride_w; m++)
                    {
                        sum += input_image[i][j * stride_h + l][k * stride_w + m];
                    }
                }
                output_image[i][j][k] = sum / (stride_h * stride_w);
            }
        }
    }

    cout << "Forward pass of " << this->module_name << " done" << endl;

    return output_image;
}

#endif
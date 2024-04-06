#ifndef RELU_H
#define RELU_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class ReLU
{
    string module_name;
    public:
    ReLU(string module_name);
    vector<vector<vector<float>>> forward(vector<vector<vector<float>>> input_image);
    vector<float> forward(vector<float> input_image);
};

ReLU::ReLU(string module_name)
{
    /*
    Constructor of ReLU module
    */
    cout << "Initializing " << module_name << "..." << endl;

    this->module_name = module_name;

    cout << "Initializing " << module_name << " done" << endl;
}

vector<vector<vector<float>>> ReLU::forward(vector<vector<vector<float>>> input_image)
{
    /*
    Forward pass of ReLU module
    */
    cout << "Forward pass of " << this->module_name << " (3d)..." << endl;

    vector<vector<vector<float>>> output_image;
    for (int i = 0; i < input_image.size(); i++)
    {
        vector<vector<float>> output_row;
        for (int j = 0; j < input_image[i].size(); j++)
        {
            vector<float> output_col;
            for (int k = 0; k < input_image[i][j].size(); k++)
            {
                if (input_image[i][j][k] < 0)
                {
                    output_col.push_back(0);
                }
                else
                {
                    output_col.push_back(input_image[i][j][k]);
                }
            }
            output_row.push_back(output_col);
        }
        output_image.push_back(output_row);
    }

    cout << "Forward pass of " << this->module_name << " done" << endl;

    return output_image;
}

vector<float> ReLU::forward(vector<float> input_image)
{
    /*
    Forward pass of ReLU module
    */
    cout << "Forward pass of " << this->module_name << " (1d)..." << endl;

    vector<float> output_image;
    for (int i = 0; i < input_image.size(); i++)
    {
        if (input_image[i] < 0)
        {
            output_image.push_back(0);
        }
        else
        {
            output_image.push_back(input_image[i]);
        }
    }

    cout << "Forward pass of " << this->module_name << " done" << endl;

    return output_image;
}

#endif
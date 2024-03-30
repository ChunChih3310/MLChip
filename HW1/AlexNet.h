#ifndef ALEXNET_H
#define ALEXNET_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Convolution.h"
#include "MaxPool.h"
#include "ReLU.h"
#include "AdaptiveAvgPool.h"
#include "Flatten.h"
#include "FullyConnected.h"
#include "Softmax.h"
using namespace std;

class AlexNet
{
    string module_name;
    Convolution conv1;
    ReLU relu1;
    MaxPool pool1;
    Convolution conv2;
    ReLU relu2;
    MaxPool pool2;
    Convolution conv3;
    ReLU relu3;
    Convolution conv4;
    ReLU relu4;
    Convolution conv5;
    ReLU relu5;
    MaxPool pool5;
    AdaptiveAvgPool adaptive_avg_pool;
    Flatten flatten;
    FullyConnected fc6;
    ReLU relu6;
    FullyConnected fc7;
    ReLU relu7;
    FullyConnected fc8;
    Softmax softmax;

public:
    // AlexNet(string module_name);
    AlexNet();
    vector<float> forward(vector<vector<vector<float>>> input_image);
    string check_size(vector<vector<vector<float>>> input_image);
    void debug_layer_3d(int i, vector<vector<vector<float>>> input_image);
};

AlexNet::AlexNet() : module_name("AlexNet"),
                     conv1("conv1", 3, 64, 11, 4, 2),
                     relu1("relu1"),
                     pool1("pool1", 3, 2),
                     conv2("conv2", 64, 192, 5, 1, 2),
                     relu2("relu2"),
                     pool2("pool2", 3, 2),
                     conv3("conv3", 192, 384, 3, 1, 1),
                     relu3("relu3"),
                     conv4("conv4", 384, 256, 3, 1, 1),
                     relu4("relu4"),
                     conv5("conv5", 256, 256, 3, 1, 1),
                     relu5("relu5"),
                     pool5("pool5", 3, 2),
                     adaptive_avg_pool("adaptive_avg_pool", 6, 6),
                     flatten("flatten"),
                     fc6("fc6", 9216, 4096),
                     relu6("relu6"),
                     fc7("fc7", 4096, 4096),
                     relu7("relu7"),
                     fc8("fc8", 4096, 1000),
                     softmax("softmax")
{
    /*
    Constructor of AlexNet module
    */
    cout << "Initializing AlexNet..." << endl;
}

vector<float> AlexNet::forward(vector<vector<vector<float>>> input_image)
{
    /*
    Forward pass of AlexNet
    */
    bool debug = false;

    cout << "Forward pass of " << this->module_name << "..." << endl;

    // Convolution 1
    input_image = this->conv1.forward(input_image);
    input_image = this->relu1.forward(input_image);
    input_image = this->pool1.forward(input_image);
    cout << "Output image size after conv1: " << this->check_size(input_image) << endl;

    // Convolution 2
    input_image = this->conv2.forward(input_image);
    input_image = this->relu2.forward(input_image);
    input_image = this->pool2.forward(input_image);
    cout << "Output image size after conv2: " << this->check_size(input_image) << endl;

    // Convolution 3
    input_image = this->conv3.forward(input_image);
    input_image = this->relu3.forward(input_image);
    cout << "Output image size after conv3: " << this->check_size(input_image) << endl;

    // Convolution 4
    input_image = this->conv4.forward(input_image);
    input_image = this->relu4.forward(input_image);
    cout << "Output image size after conv4: " << this->check_size(input_image) << endl;

    // Convolution 5
    input_image = this->conv5.forward(input_image);
    input_image = this->relu5.forward(input_image);
    input_image = this->pool5.forward(input_image);
    cout << "Output image size after conv5: " << this->check_size(input_image) << endl;

    // AdaptiveAvgPool
    input_image = this->adaptive_avg_pool.forward(input_image);
    cout << "Output image size after adaptive_avg_pool: " << this->check_size(input_image) << endl;

    // Flatten
    vector<float> output_image = this->flatten.forward(input_image);
    cout << "Output image size after flatten: " << output_image.size() << endl;

    // FullyConnected 6
    output_image = this->fc6.forward(output_image);
    output_image = this->relu6.forward(output_image);
    cout << "Output image size after fc6: " << output_image.size() << endl;

    // FullyConnected 7
    output_image = this->fc7.forward(output_image);
    output_image = this->relu7.forward(output_image);
    cout << "Output image size after fc7: " << output_image.size() << endl;

    // FullyConnected 8
    output_image = this->fc8.forward(output_image);
    cout << "Output image size after fc8: " << output_image.size() << endl;

    // Softmax
    output_image = this->softmax.forward(output_image);
    cout << "Output image size after softmax: " << output_image.size() << endl;

    cout << "Forward pass of " << this->module_name << " done" << endl;

    return output_image;
}

string AlexNet::check_size(vector<vector<vector<float>>> input_image)
{
    string size = to_string(input_image.size()) + "x" + to_string(input_image[0].size()) + "x" + to_string(input_image[0][0].size());
    return size;
}

void AlexNet::debug_layer_3d(int i, vector<vector<vector<float>>> input_image)
{
    cout << "Debugging layer (3D) " << i << "..." << endl;
    for (int j = 0; j < 5; j++)
        cout << input_image[1][j][4] << endl;
}

#endif
#include <systemc.h>
#include <iostream>
#include <vector>
#include "AlexNet.h"
using namespace std;

SC_MODULE(SC_AlexNet)
{
    sc_vector<sc_vector<sc_vector<sc_in<float>>>> input_image;
    sc_vector<sc_out<float>> output_distribution;

    AlexNet alexnet;

    void forward()
    {
        // 3 * 224 * 224
        vector<vector<vector<float>>> input_image_vector(3, vector<vector<float>>(224, vector<float>(224, 0.0)));
        // 1000
        vector<float> output_distribution_vector(1000, 0.0);

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 224; j++)
                for (int k = 0; k < 224; k++)
                {
                    input_image_vector[i][j][k] = input_image[i][j][k].read();
                }

        output_distribution_vector = alexnet.forward(input_image_vector);

        for (int i = 0; i < 1000; i++)
        {
            output_distribution[i].write(output_distribution_vector[i]);
        }
    }

    SC_CTOR(SC_AlexNet) : alexnet()
    {
        // Resize input_image to 3 * 224 * 224
        input_image.init(3);
        for (int i = 0; i < 3; i++)
        {
            input_image[i].init(224);
            for (int j = 0; j < 224; j++)
            {
                input_image[i][j].init(224);
            }
        }
        // Resize output_distribution to 1000
        output_distribution.init(1000);

        SC_METHOD(forward);

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 224; j++)
            {
                for (int k = 0; k < 224; k++)
                {
                    sensitive << input_image[i][j][k];
                }
            }
        }
    }
};
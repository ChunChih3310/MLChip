#include <systemc.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "SC_AlexNet.h"
using namespace std;

vector<vector<vector<float>>> read_image(string txt_file)
{
    vector<vector<vector<float>>> input_image(3, vector<vector<float>>(224, vector<float>(224, 0.0)));
    ifstream file(txt_file);
    int channel = 0;
    int row = 0;
    int col = 0;
    float value;
    if (file.is_open())
    {
        while (file >> value)
        {
            input_image[channel][row][col] = value;
            col++;
            if (col == 224)
            {
                col = 0;
                row++;
                if (row == 224)
                {
                    row = 0;
                    channel++;
                    if (channel == 3)
                    {
                        break;
                    }
                }
            }
        }
        file.close();
    }
    return input_image;
}

vector<string> read_class_name()
{
    vector<string> class_name;
    ifstream file("data/imagenet_classes.txt");
    string name;
    if (file.is_open())
    {
        while (getline(file, name))
        {
            class_name.push_back(name);
        }
        file.close();
    }
    return class_name;
}

void distribution_to_class(vector<float> distribution, vector<string> class_names)
{
    /*
    Convert distribution to class
    */
    cout << "Converting distribution to class..." << endl
         << endl;

    int max_idx = 0;
    for (int i = 0; i < distribution.size(); i++)
    {
        if (distribution[i] > distribution[max_idx])
        {
            max_idx = i;
        }

        if (distribution[i] < 0)
        {
            cout << "Error: Negative probability" << endl;
            return;
        }
    }

    cout << "Predicted class: " << class_names[max_idx] << endl;
    cout << "Predicted probability: " << distribution[max_idx] << endl;
    cout << "ImageNet class index: " << max_idx << endl
         << endl;
}

int sc_main(int argc, char *argv[])
{
    string txt_file = argv[1];

    SC_AlexNet alexnet("alexnet");
    sc_vector<sc_vector<sc_vector<sc_signal<float>>>> input_image;
    sc_vector<sc_signal<float>> output_distribution;

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

    // Read image from txt file
    vector<vector<vector<float>>> input_image_vec = read_image(txt_file);

    // Read class names from file
    vector<string> class_names = read_class_name();

    // Write input_image_vec to input_image
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 224; j++)
            for (int k = 0; k < 224; k++)
            {
                input_image[i][j][k].write(input_image_vec[i][j][k]);
            }

    // Connect input_image to SC_AlexNet module
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 224; j++)
        {
            for (int k = 0; k < 224; k++)
            {
                alexnet.input_image[i][j][k](input_image[i][j][k]);
            }
        }
    }

    // Connect output_distribution to SC_AlexNet module
    for (int i = 0; i < 1000; i++)
    {
        alexnet.output_distribution[i](output_distribution[i]);
    }

    sc_start();

    // Print output_distribution
    vector<float> output_distribution_vec(1000);
    for (int i = 0; i < 1000; i++)
    {
        // cout << "output_distribution[" << i << "]: " << output_distribution[i].read() << endl;
        output_distribution_vec[i] = output_distribution[i].read();
    }

    // Convert output_distribution to class
    distribution_to_class(output_distribution_vec, class_names);

    return 0;
}

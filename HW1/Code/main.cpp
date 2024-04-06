#include <systemc.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
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
    else
    {
        cout << "Error: Unable to open file " << txt_file << endl;
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
    else
    {
        cout << "Error: Unable to open file "
             << "data/imagenet_classes.txt" << endl;
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

void print_top_k(vector<float> prob, vector<float> logit, vector<string> class_names, int k)
{
    vector<pair<float, int>> prob_idx;
    for (int i = 0; i < prob.size(); i++)
    {
        prob_idx.push_back(make_pair(prob[i], i));
    }

    sort(prob_idx.begin(), prob_idx.end(), greater<pair<float, int>>());

    cout << endl;
    cout << "===================== "
         << "Top " << k << " =====================" << endl;
    cout << setw(3) << "idx"
         << " | " << setw(11) << "logit"
         << " | " << setw(11) << "probability"
         << " | class name" << endl;
    cout << "-------------------------------------------------" << endl;

    for (int i = 0; i < k; i++)
    {
        int idx = prob_idx[i].second;
        cout << setw(3) << idx << " | ";
        cout << setw(11) << fixed << setprecision(6) << logit[idx] << " | ";
        cout << setw(11) << fixed << setprecision(6) << prob[idx] * 100 << " | ";
        cout << class_names[idx] << endl;
    }
    cout << "=================================================" << endl;
}

int sc_main(int argc, char *argv[])
{
    string txt_file = argv[1];

    SC_AlexNet alexnet("alexnet");
    sc_vector<sc_vector<sc_vector<sc_signal<float>>>> input_image;
    sc_vector<sc_signal<float>> output_prob;
    sc_vector<sc_signal<float>> output_logit;

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

    // Resize dim(output) to 1000
    output_prob.init(1000);
    output_logit.init(1000);

    // Read image from txt file
    vector<vector<vector<float>>> input_image_vec = read_image(txt_file);

    // Read class names from file
    vector<string> class_names = read_class_name();

    // Write input_image_vec to input_image
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 224; j++)
            for (int k = 0; k < 224; k++)
                input_image[i][j][k].write(input_image_vec[i][j][k]);

    // Connect input_image to SC_AlexNet module
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 224; j++)
            for (int k = 0; k < 224; k++)
                alexnet.input_image[i][j][k](input_image[i][j][k]);

    // Connect output_distribution to SC_AlexNet module
    for (int i = 0; i < 1000; i++)
    {
        alexnet.output_prob[i](output_prob[i]);
        alexnet.output_logit[i](output_logit[i]);
    }

    sc_start();

    // Print output_distribution
    vector<float> output_prob_vec(1000, 0.0);
    vector<float> output_logit_vec(1000, 0.0);
    for (int i = 0; i < 1000; i++)
    {
        output_prob_vec[i] = output_prob[i].read();
        output_logit_vec[i] = output_logit[i].read();
    }

    // Convert output_distribution to class
    // distribution_to_class(output_distribution_vec, class_names);
    print_top_k(output_prob_vec, output_logit_vec, class_names, 10);
    cout << "Ground Truth: " << txt_file << endl;

    return 0;
}

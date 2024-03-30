// #include <iostream>
// #include <vector>
// #include <string>
// #include "AlexNet.h"
// using namespace std;

// void distribution_to_class(vector<float> distribution, vector<string> class_names)
// {
//     /*
//     Convert distribution to class
//     */
//     cout << "Converting distribution to class..." << endl
//          << endl;

//     int max_idx = 0;
//     for (int i = 0; i < distribution.size(); i++)
//     {
//         if (distribution[i] > distribution[max_idx])
//         {
//             max_idx = i;
//         }

//         if (distribution[i] < 0)
//         {
//             cout << "Error: Negative probability" << endl;
//             return;
//         }
//     }

//     cout << "Predicted class: " << class_names[max_idx] << endl;
//     cout << "Predicted probability: " << distribution[max_idx] << endl;
//     cout << "ImageNet class index: " << max_idx << endl
//          << endl;
// }

// vector<string> read_class_names()
// {
//     /*
//     Read class names from file
//     */
//     cout << "Reading class names..." << endl;

//     vector<string> class_names;
//     string data_path = "data/";
//     ifstream file(data_path + "imagenet_classes.txt");

//     // each line is a class name
//     string class_name;
//     while (getline(file, class_name))
//     {
//         class_names.push_back(class_name);
//     }

//     cout << "Reading class names done" << endl
//          << endl;

//     return class_names;
// }

// vector<vector<vector<float>>> read_image(string image_path)
// {
//     /*
//     Read image from file
//     */
//     cout << "Reading image " << image_path << "..." << endl;

//     vector<vector<vector<float>>> image;
//     ifstream file(image_path);
//     float value;

//     int image_channels = 3;
//     int image_height = 224;
//     int image_width = 224;

//     int channel = 0;
//     int row = 0;
//     int col = 0;

//     // Resize image
//     image.resize(image_channels);
//     for (int i = 0; i < image_channels; i++)
//     {
//         image[i].resize(image_height);
//         for (int j = 0; j < image_height; j++)
//         {
//             image[i][j].resize(image_width);
//         }
//     }

//     while (file >> value)
//     {
//         image[channel][row][col] = value;
//         col++;
//         if (col == image_width)
//         {
//             col = 0;
//             row++;
//             if (row == image_height)
//             {
//                 row = 0;
//                 channel++;
//                 if (channel == image_channels)
//                 {
//                     break;
//                 }
//             }
//         }
//     }

//     cout << "Reading image done" << endl
//          << endl;

//     return image;
// }

// int main()
// {
//     vector<vector<vector<float>>> input_image;
//     input_image = read_image("data/dog.txt");
//     cout << "Input image size: " << input_image.size() << "x" << input_image[0].size() << "x" << input_image[0][0].size() << endl;
//     vector<float> output_distribution;
//     AlexNet alexnet;
//     output_distribution = alexnet.forward(input_image);
//     cout << "Output distribution size: " << output_distribution.size() << endl;

//     vector<string> class_names = read_class_names();
//     distribution_to_class(output_distribution, class_names);

//     return 0;
// }
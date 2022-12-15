//
// Created by Иван Назаров on 09.12.2022.
//
#pragma once
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>

struct Input {
    cv::Mat image;
    std::string result_path;
    int64_t split_size;
    bool is_shuffled;
};

void get_arguments(int argc, char** argv, Input& user_input);
//
// Created by Иван Назаров on 09.12.2022.
//
#include "input_struct.h"

bool get_image(int argc, char** argv, Input& user_input) {
    for (int i = 0; i < argc - 1; ++i) {
        if (strcmp(argv[i], "-i") == 0) {
            user_input.image = cv::imread(argv[i + 1], cv::IMREAD_COLOR);
            if (user_input.image.data != NULL) {
                return true;
            }
        }
    }
    return false;
}

bool get_result_path(int argc, char** argv, Input& user_input) {
    for (int i = 0; i < argc - 1; ++i) {
        if (strcmp(argv[i], "-o") == 0) {
            user_input.result_path = std::string(argv[i + 1]);
            if (user_input.result_path != "") {
                return true;
            }
        }
    }
    return false;
}

bool get_split_size(int argc, char** argv, Input& user_input) {
    for (int i = 0; i < argc - 1; ++i) {
        if (strcmp(argv[i], "-n") == 0) {
            int64_t n = atoi(argv[i + 1]);
            if (n > 0) {
                user_input.split_size = n;
                return true;
            }
        }
    }
    return false;
}

bool get_shuffled(int argc, char** argv, Input& user_input) {
    for (int i = 0; i < argc - 1; ++i) {
        if (strcmp(argv[i], "-s") == 0) {
            int64_t s = atoi(argv[i + 1]);
            if (s == 0 || s == 1) {
                user_input.is_shuffled = (s == 1);
                return true;
            }
        }
    }
    return false;
}

void get_arguments(int argc, char** argv, Input& user_input ) {
    if (argc != 9) {
        throw std::runtime_error("Wrong number of arguments!");
    }
    if (!get_image(argc, argv, user_input)) {
        throw std::runtime_error("Input image access error!");
    }
    if (!get_split_size(argc, argv, user_input)) {
        throw std::runtime_error("Split size is set incorrectly!");
    }
    if (!get_shuffled(argc, argv, user_input)) {
        throw std::runtime_error("Flag is_shuffled is incorrect!");
    }
    if (!get_result_path(argc, argv, user_input)) {
        throw std::runtime_error("Incorrect output path!");
    }
}
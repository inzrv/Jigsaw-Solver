#include <iostream>
#include "jigsaw_maker.h"
#include "solver.h"
#include <opencv2/core/utility.hpp>
#include <string>
#include "input_struct.h"

int main(int argc, char** argv) {

    Input input;
    get_arguments(argc, argv, input);

    cv::Mat shuffled_image;
    if (input.is_shuffled == 0) {
        std::string shuffled_image_path = "../puzzles/shuffled.jpg";
        shuffled_image = input.image.clone();
        make_jigsaw(input.image, input.split_size, shuffled_image);
        cv::imwrite(shuffled_image_path.data(), shuffled_image);
    } else {
        shuffled_image = input.image;
    }

    Solver solver(shuffled_image, input.split_size);
    solver.solve();
    cv::Mat result_img = solver.get_result_image();

    cv::imwrite(input.result_path.data(), result_img);
}


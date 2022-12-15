//
// Created by Иван Назаров on 03.12.2022.
//
#include "jigsaw_maker.h"
#include "opencv2/opencv.hpp"
#include <random>


void make_jigsaw(const cv::Mat& src_img, int64_t n, cv::Mat dst_img) {
    std::vector<cv::Mat> parts;
    split_img(src_img, n, parts);
    concat_img(parts, n, dst_img);
}

void split_img(const cv::Mat& img, int64_t n, std::vector<cv::Mat>& parts) {
    cv::Size part_size(img.rows / n, img.cols / n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            cv::Mat cur_part(img, cv::Rect(part_size.width * j, part_size.height * i,
                                           part_size.width, part_size.height));
            parts.push_back(cur_part);
        }
    }
    auto rng = std::default_random_engine{2};
    std::shuffle(parts.begin(), parts.end(), rng);
}

void concat_img(std::vector<cv::Mat>& parts, int64_t n, cv::Mat& img) {
    cv::Size part_size(parts[0].rows, parts[0].cols);
    size_t k = 0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            parts[k++].copyTo(img(cv::Rect(part_size.width * j, part_size.height * i,
                                           part_size.width, part_size.height )));
        }
    }
}
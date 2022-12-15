//
// Created by Иван Назаров on 03.12.2022.
//
#pragma once
#include <cstdlib>
#include "opencv2/opencv.hpp"
#include <vector>

// Creates a jigsaw based on the given picture
void make_jigsaw(const cv::Mat& src_img, int64_t n, cv::Mat dst_img);

// Split the image into n^2 parts, shuffle them and push them into vector
void split_img(const cv::Mat& img, int64_t n, std::vector<cv::Mat>& parts);

// Concatenate parts from a vector into one image
void concat_img(std::vector<cv::Mat>& parts, int64_t n, cv::Mat& img);



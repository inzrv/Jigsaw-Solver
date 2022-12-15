//
// Created by Иван Назаров on 03.12.2022.
//
#include <vector>
#include <cstdlib>
#include <opencv2/opencv.hpp>

enum sides {left, right, top, bottom};

const double MAX_ERROR = 2.0;

class Solver {
public:
    Solver(cv::Mat img, int64_t n);
    void solve();
    cv::Mat get_result_image() const;
private:
    std::vector<cv::Mat> pieces;
    int64_t split_size;
    cv::Mat result_image;

    void make_image(std::vector<cv::Mat>& rows);
    void make_rows(std::vector<cv::Mat>& rows) const;
    cv::Mat make_row(std::vector<uint8_t>& used) const;
    std::pair<size_t, sides> find_best_row(const cv::Mat& cur_img,
                                           const std::vector<cv::Mat>& rows,
                                           std::vector<uint8_t>& used) const;
    std::pair<size_t, sides> find_best_piece(const cv::Mat& cur_row, std::vector<uint8_t>& used) const;
    double get_error_for_pair(const cv::Mat& attachable, const cv::Mat& receiving, sides side) const;
    double calculate_error(const cv::Mat& edge1, const cv::Mat& edge2) const;
    size_t find_not_used(const std::vector<uint8_t>& used) const;
};
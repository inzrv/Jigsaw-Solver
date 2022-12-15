//
// Created by Иван Назаров on 04.12.2022.
//
#include "solver.h"
#include <vector>
#include <string>

Solver::Solver(cv::Mat img, int64_t n) : split_size(n) {
    cv::Size part_size(img.rows / split_size, img.cols / split_size);
    for (size_t i = 0; i < split_size; ++i) {
        for (size_t j = 0; j < split_size; ++j) {
            cv::Mat cur_img(img, cv::Rect(part_size.width * j, part_size.height * i,
                                           part_size.width, part_size.height));
            pieces.push_back(cur_img);
        }
    }
}

cv::Mat Solver::get_result_image() const {
    return result_image;
}

double Solver::get_error_for_pair(const cv::Mat& attachable, const cv::Mat& receiving, sides side) const {
    cv::Mat attachable_edge;
    cv::Mat receiving_edge;
    switch (side) {
        case left:
            receiving_edge = receiving.col(0);
            attachable_edge = attachable.col(attachable.cols - 1);
            break;
        case right:
            attachable_edge = attachable.col(0);
            receiving_edge = receiving.col(receiving.cols - 1);
            break;
        case bottom:
            attachable_edge = attachable.row(0);
            receiving_edge = receiving.row(receiving.rows - 1);
            break;
        default:
            attachable_edge = attachable.row(attachable.rows - 1);
            receiving_edge = receiving.row(0);
    }
    return calculate_error(attachable_edge, receiving_edge);
}

double Solver::calculate_error(const cv::Mat& edge1, const cv::Mat& edge2) const {
    cv::Mat diff;
    cv::absdiff(edge1, edge2, diff);
    cv::Scalar sum_by_channels = cv::sum(diff);
    double error = static_cast<double>(sum_by_channels[0] + sum_by_channels[1] + sum_by_channels[2]);
    double norm_const = 1 / (255.0 * 3 * edge1.total());
    return error * norm_const;
}

std::pair<size_t, sides> Solver::find_best_piece(const cv::Mat& cur_row, std::vector<uint8_t> &used) const {
    double min_error = MAX_ERROR;
    size_t piece_pos = 0;
    sides side = left;

    for (size_t i = 0; i < pieces.size(); ++i) {
        if (used[i] == 0) {
            double left_error = get_error_for_pair(pieces[i], cur_row, left);
            double right_error = get_error_for_pair(pieces[i], cur_row, right);
            if (left_error < min_error) {
                min_error = left_error;
                piece_pos = i;
                side = left;
            }
            if (right_error < min_error) {
                min_error = right_error;
                piece_pos = i;
                side = right;
            }
        }
    }
    return std::make_pair(piece_pos, side);
}

size_t Solver::find_not_used(const std::vector<uint8_t> &used) const {
    for (size_t i = 0; i < used.size(); ++i) {
        if (used[i] == 0) {
            return i;
        }
    }
    return 0;
}

cv::Mat Solver::make_row(std::vector<uint8_t>& used) const {
    cv::Mat cur_row;

    size_t not_used_piece = find_not_used(used);
    cur_row = pieces[not_used_piece].clone();
    used[not_used_piece] = 1;

    for (size_t k = 0; k < split_size - 1; ++k) {
        const auto best_piece = find_best_piece(cur_row, used);
        size_t piece_pos = best_piece.first;
        sides side = best_piece.second;
        used[piece_pos] = 1;

        if (side == left) {
            cv::hconcat(pieces[piece_pos], cur_row, cur_row);
        } else {
            cv::hconcat(cur_row, pieces[piece_pos], cur_row);
        }
    }
    return cur_row;
}

void Solver::make_rows(std::vector<cv::Mat> &rows) const{
    std::vector<uint8_t> used(pieces.size(), 0);
    for (size_t k = 0; k < split_size; ++k) {
        rows.push_back(make_row(used));
    }
}

void Solver::solve() {
    std::vector<cv::Mat> rows;
    make_rows(rows);
    make_image(rows);
}

std::pair<size_t, sides> Solver::find_best_row(const cv::Mat& cur_img,
                                               const std::vector<cv::Mat>& rows,
                                               std::vector<uint8_t>& used) const {
    double min_error = MAX_ERROR;
    size_t row_pos = 0;
    sides side = top;

    for (size_t i = 0; i < rows.size(); ++i) {
        if (used[i] == 0) {
            double top_error = get_error_for_pair(rows[i], cur_img, top);
            double bottom_error = get_error_for_pair(rows[i], cur_img, bottom);
            if (top_error < min_error) {
                min_error = top_error;
                row_pos = i;
                side = top;
            }
            if (bottom_error < min_error) {
                min_error = bottom_error;
                row_pos = i;
                side = bottom;
            }
        }
    }
    return std::make_pair(row_pos, side);
}

void Solver::make_image(std::vector<cv::Mat> &rows) {
    std::vector<uint8_t> used(rows.size(), 0);
    cv::Mat cur_img;
    cur_img = rows[0].clone();
    used[0] = 1;

    for (size_t k = 0; k < rows.size() - 1; ++k) {

        const auto best_row = find_best_row(cur_img, rows, used);
        size_t row_pos = best_row.first;
        sides side = best_row.second;

        used[row_pos] = 1;

        if (side == top) {
            cv::vconcat(rows[row_pos], cur_img, cur_img);
        } else {
            cv::vconcat(cur_img, rows[row_pos], cur_img);
        }
    }
    result_image = cur_img;
}












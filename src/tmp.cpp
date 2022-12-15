//
// Created by Иван Назаров on 04.12.2022.
//
#include "solver.h"
#include <vector>
#include <string>

Solver::Solver(cv::Mat img, int64_t n) : n(n) {
    cv::Size part_size(img.rows / n, img.cols / n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            cv::Mat cur_img(img, cv::Rect(part_size.width * j, part_size.height * i,
                                          part_size.width, part_size.height));
            pieces.push_back(cur_img);
        }
    }
}

void Solver::get_pieces(int64_t it) const {
    std::cout << pieces.size() << std::endl;
    for (size_t i = 0; i < pieces.size(); ++i) {
        std::string name = std::to_string(it) + " piece_" + std::to_string(i + 1) + ".jpg";
        cv::imwrite(name.data(), pieces[i]);
    }
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

double Solver::calculate_error(cv::Mat edge1, cv::Mat edge2) const {
    cv::Mat diff;
    cv::absdiff(edge1, edge2, diff);
    cv::Scalar sum_by_channels = cv::sum(diff);
    double error = static_cast<double>(sum_by_channels[0] + sum_by_channels[1] + sum_by_channels[2]);
    double norm_const = 1 / (255.0 * 3 * edge1.total());
    return error * norm_const;
}

void Solver::make_row(cv::Mat& cur_row, std::vector<uint8_t>& used) {

    for (size_t k = 0; k < n - 1; ++k) {

        double min_error = MAX_ERROR;
        size_t best_match = 0;
        sides best_side = left;

        for (size_t i = 0; i < pieces.size(); ++i) {
            if (used[i] == 0) {
                double left_error = get_error_for_pair(pieces[i], cur_row, left);
                double right_error = get_error_for_pair(pieces[i], cur_row, right);
                if (left_error < min_error) {
                    min_error = left_error;
                    best_match = i;
                    best_side = left;
                }
                if (right_error < min_error) {
                    min_error = right_error;
                    best_match = i;
                    best_side = right;
                }
            }
        }

        used[best_match] = 1;

        if (best_side == left) {
            cv::hconcat(pieces[best_match], cur_row, cur_row);
        } else {
            cv::hconcat(cur_row, pieces[best_match], cur_row);
        }

        //cv::imshow("cur.jpg", cur_row);
        //cv::waitKey(1000);
    }
}

void Solver::solve2() {
    std::vector<uint8_t> used(pieces.size(), 0);
    std::vector<cv::Mat> rows;
    for (size_t i = 0; i < used.size(); ++i) {
        if (used[i] == 0) {
            used[i] = 1;
            cv::Mat cur_row = pieces[i].clone();
            make_row(cur_row, used);
            rows.push_back(cur_row);
        }
    }

    cv::Mat img = assemble(rows);
    cv::imshow("result.jpg", img);
    cv::waitKey(0);

    /*
    for (size_t i = 0; i < rows.size(); ++i) {
        std::string name = std::to_string(i) + "_row.jpg";
        cv::imshow(name.data(), rows[i]);
    }

    cv::imshow("result.jpg", img);

    cv::waitKey(0);
     */
}

cv::Mat Solver::assemble(std::vector<cv::Mat> &rows) {
    std::vector<uint8_t> used(rows.size(), 0);
    cv::Mat cur_img;
    cur_img = rows[0].clone();

    for (size_t k = 0; k < rows.size() - 1; ++k) {

        double min_error = MAX_ERROR;
        size_t best_match = 0;
        sides best_side = top;

        for (size_t i = 0; i < rows.size(); ++i) {
            if (used[i] == 0) {
                double top_error = get_error_for_pair(rows[i], cur_img, top);
                double bottom_error = get_error_for_pair(rows[i], cur_img, bottom);
                if (top_error < min_error) {
                    min_error = top_error;
                    best_match = i;
                    best_side = top;
                }
                if (bottom_error < min_error) {
                    min_error = bottom_error;
                    best_match = i;
                    best_side = bottom;
                }
            }
        }

        used[best_match] = 1;

        if (best_side == top) {
            cv::vconcat(rows[best_match], cur_img, cur_img);
        } else {
            cv::vconcat(cur_img, rows[best_match], cur_img);
        }
    }
    return cur_img;
}












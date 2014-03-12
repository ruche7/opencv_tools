/// @file
/// @brief �摜�֘A�̃��[�e�B���e�B������`�w�b�_�B
/// @author ruche

#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <string>

namespace cvtool { namespace image
{
    /// @brief ��O�𓊂����G���[�o�͂��s��Ȃ� cv::imread ���b�v�֐��B
    cv::Mat read(const std::string& filename, int flags = cv::IMREAD_COLOR);

    /// @brief ��O�𓊂����G���[�o�͂��s��Ȃ� cv::imwrite ���b�v�֐��B
    bool write(
        const std::string& filename,
        cv::InputArray img,
        const cv::vector<int>& params = cv::vector<int>());
}}

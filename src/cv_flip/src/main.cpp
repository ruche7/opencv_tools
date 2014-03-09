/// @file
/// @brief �摜�̍��E�܂��͏㉺�𔽓]������c�[�� cv_flip �̃\�[�X�B
/// @author ruche

#include <cvtool/program.hpp>
#include <cvtool/utility/error.hpp>
#include <cvtool/utility/file.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <iostream>

namespace
{
    /// �v���O���������l�\���́B
    struct arg_store
    {
        std::string prefix = "";
        std::string suffix = "";
        bool horz = false;
        bool vert = false;
    };

    /// ���C���������s���B
    bool execute(const std::string& file, const arg_store& store)
    {
        using namespace std;

        cout << "INPUT  : " << file << endl;

        // �t�@�C���ǂݍ���
        cv::Mat src_img = cv::imread(file, -1);
        if (src_img.empty())
        {
            cvtool::utility::print_error("Cannot read image file '" + file + "'.");
            return false;
        }

        // 0:�c���], +1:�����], -1:�����]
        const int flip_code = store.vert ? (store.horz ? -1 : 0) : +1;

        const char* const flip_names[] = { "rotation", "vertical", "horizontal" };
        cout << "FLIP   : " << flip_names[flip_code + 1] << endl;

        // ���]�摜�쐬
        cv::Mat dest_img = src_img.clone();
        cv::flip(src_img, dest_img, flip_code);

        // �o�̓t�@�C���p�X�쐬
        const std::string outfile =
            cvtool::utility::change_filename(file, store.prefix, store.suffix);

        // �o�͐�f�B���N�g���쐬
        if (!cvtool::utility::create_parent_directories(outfile))
        {
            cvtool::utility::print_error(
                "Cannot create parent directory of '" + outfile + "'.");
            return false;
        }

        cout << "OUTPUT : " << outfile << endl;

        // �o��
        if (!cv::imwrite(outfile, dest_img))
        {
            cvtool::utility::print_error("Cannot write image file '" + outfile + "'.");
            return false;
        }

        return true;
    }
}

/// ���C���֐��B
int main(int argc, char* argv[])
{
    arg_store store;
    cvtool::program program(
        argc,
        argv,
        "files...",
        -1,
        cvtool::utility::make_filename_prefix_program_arg(
            "file-prefix,P",
            store.prefix),
        cvtool::utility::make_filename_suffix_program_arg(
            "file-suffix,S",
            store.suffix),
        cvtool::make_switch_program_arg(
            "horz,h",
            "flip horizontal. (default)",
            store.horz),
        cvtool::make_switch_program_arg(
            "vert,v",
            "flip vertical.",
            store.vert));

    // �v���O�����������p�[�X
    try
    {
        program.parse_args();
    }
    catch (const std::exception& ex)
    {
        cvtool::utility::print_error(ex);
        return 1;
    }

    // �w���v�\�����ׂ��H
    if (program.help_required())
    {
        program.print_help();
        return 0;
    }

    // �t�@�C����1���n����Ȃ������H
    if (program.parsed_positional_arg_count() <= 0)
    {
        cvtool::utility::print_error("No input files.\nSet '--help' to print usage.");
        return 1;
    }

    // �t�@�C�����Ƃɏ���
    bool result = true;
    program.for_each_parsed_positional_args(
        [&](const std::string& arg) { result &= execute(arg, store); });

    return result ? 0 : 1;
}

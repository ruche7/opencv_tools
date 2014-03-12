/// @file
/// @brief �摜�t�@�C�������l�[������c�[�� cv_rename �̃\�[�X�B
/// @author ruche

#include <cvtool/program.hpp>
#include <cvtool/utility/error.hpp>
#include <cvtool/utility/file.hpp>
#include <cvtool/utility/image.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <string>

namespace fs = boost::filesystem;

namespace
{
    /// �v���O���������l�\���́B
    struct arg_store
    {
        std::string format = "";
        std::string prefix = "";
        std::string suffix = "";
        bool copy = false;
        bool overwrite = false;
    };

    /// ���C���������s���B
    bool execute(const std::string& file, const arg_store& store)
    {
        using namespace std;
        namespace al = boost::algorithm;

        // �t�@�C���p�X�쐬
        const auto from_file = cvtool::file::format_filename("$0", file);
        const auto to_file =
            cvtool::file::change_filename(
                cvtool::file::format_filename(store.format, file),
                store.prefix,
                store.suffix);

        cout << "FROM : " << from_file << endl;

        // �t�@�C���p�X���ς��Ȃ��Ȃ牽�����Ȃ�
        if (fs::path(from_file) == fs::path(to_file))
        {
            cout << " --> Do not rename." << endl;
            return true;
        }

        // �㏑���ł��Ȃ��Ȃ牽�����Ȃ�
        if (!store.overwrite && fs::exists(to_file))
        {
            cout << " --> '" << to_file << "' is already exists. Do nothing." << endl;
            return true;
        }

        // �g���q�擾
        const auto from_ext =
            al::to_lower_copy(cvtool::file::filename_extension(from_file));
        const auto to_ext =
            al::to_lower_copy(cvtool::file::filename_extension(to_file));

        // �摜�ϊ��s�v���H
        bool copy_only = (from_ext == to_ext || to_ext.empty());

        if (!copy_only)
        {
            // �t�@�C���ǂݍ���
            cv::Mat img = cvtool::image::read(from_file, -1);

            // �ǂݍ��ݎ��s�Ȃ�P���R�s�[����
            copy_only = img.empty();
            if (copy_only)
            {
                cout << " --> '" << from_file << "' is not image file. -->" << endl;
            }
            else
            {
                // �t�@�C�������o��
                if (cvtool::image::write(to_file, img))
                {
                    cout << " --> Convert image... -->" << endl;
                }
                else
                {
                    // �����o�����s�Ȃ�P���R�s�[����
                    copy_only = true;
                    cout << " --> Cannot write as image file. -->" << endl;
                }
            }
        }

        // �P���R�s�[
        if (copy_only)
        {
            fs::copy_file(from_file, to_file, fs::copy_option::overwrite_if_exists);
            cout << " --> Rename... -->" << endl;
        }

        cout << "  TO : " << to_file << endl;

        // �R�s�[�łȂ��Ȃ猳�t�@�C���폜
        if (!store.copy && !fs::remove(from_file))
        {
            cerr << "WARNING: Cannot remove file '" << from_file << "'." << endl;
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
        cvtool::file::make_filename_format_program_arg(
            "format,F",
            store.format),
        cvtool::file::make_filename_prefix_program_arg(
            "prefix,P",
            store.prefix),
        cvtool::file::make_filename_suffix_program_arg(
            "suffix,S",
            store.suffix),
        cvtool::make_switch_program_arg(
            "copy,c",
            "Original files are not removed.",
            store.copy),
        cvtool::make_switch_program_arg(
            "overwrite,w",
            "Even if renamed files are exists, these files are overwritten.",
            store.overwrite));

    // �v���O�����������p�[�X
    try
    {
        program.parse_args();
    }
    catch (const std::exception& ex)
    {
        cvtool::error::print(ex);
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
        cvtool::error::print("No input files.\nSet '--help' to print usage.");
        return 1;
    }

    // ���l�[���I�v�V������1���n����Ȃ������H
    if (store.format.empty() && store.prefix.empty() && store.suffix.empty())
    {
        cvtool::error::print("No rename options.\nSet '--help' to print usage.");
        return 1;
    }

    // �t�@�C�����Ƃɏ���
    bool result = true;
    try
    {
        program.for_each_parsed_positional_args(
            [&](const std::string& arg) { result &= execute(arg, store); });
    }
    catch (const std::exception& ex)
    {
        cvtool::error::print(ex);
        return 1;
    }

    return result ? 0 : 1;
}

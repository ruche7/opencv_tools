/// @file
/// @brief 画像の左右または上下を反転させるツール cv_flip のソース。
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
    /// プログラム引数値構造体。
    struct arg_store
    {
        std::string prefix = "";
        std::string suffix = "";
        bool horz = false;
        bool vert = false;
    };

    /// メイン処理を行う。
    bool execute(const std::string& file, const arg_store& store)
    {
        using namespace std;

        cout << "INPUT  : " << file << endl;

        // ファイル読み込み
        cv::Mat src_img = cv::imread(file, -1);
        if (src_img.empty())
        {
            cvtool::utility::print_error("Cannot read image file '" + file + "'.");
            return false;
        }

        // 0:縦反転, +1:横反転, -1:両反転
        const int flip_code = store.vert ? (store.horz ? -1 : 0) : +1;

        const char* const flip_names[] = { "rotation", "vertical", "horizontal" };
        cout << "FLIP   : " << flip_names[flip_code + 1] << endl;

        // 反転画像作成
        cv::Mat dest_img = src_img.clone();
        cv::flip(src_img, dest_img, flip_code);

        // 出力ファイルパス作成
        const std::string outfile =
            cvtool::utility::change_filename(file, store.prefix, store.suffix);

        // 出力先ディレクトリ作成
        if (!cvtool::utility::create_parent_directories(outfile))
        {
            cvtool::utility::print_error(
                "Cannot create parent directory of '" + outfile + "'.");
            return false;
        }

        cout << "OUTPUT : " << outfile << endl;

        // 出力
        if (!cv::imwrite(outfile, dest_img))
        {
            cvtool::utility::print_error("Cannot write image file '" + outfile + "'.");
            return false;
        }

        return true;
    }
}

/// メイン関数。
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

    // プログラム引数をパース
    try
    {
        program.parse_args();
    }
    catch (const std::exception& ex)
    {
        cvtool::utility::print_error(ex);
        return 1;
    }

    // ヘルプ表示すべき？
    if (program.help_required())
    {
        program.print_help();
        return 0;
    }

    // ファイルが1つも渡されなかった？
    if (program.parsed_positional_arg_count() <= 0)
    {
        cvtool::utility::print_error("No input files.\nSet '--help' to print usage.");
        return 1;
    }

    // ファイルごとに処理
    bool result = true;
    program.for_each_parsed_positional_args(
        [&](const std::string& arg) { result &= execute(arg, store); });

    return result ? 0 : 1;
}

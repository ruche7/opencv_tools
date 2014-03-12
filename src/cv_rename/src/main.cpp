/// @file
/// @brief 画像ファイルをリネームするツール cv_rename のソース。
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
    /// プログラム引数値構造体。
    struct arg_store
    {
        std::string format = "";
        std::string prefix = "";
        std::string suffix = "";
        bool copy = false;
        bool overwrite = false;
    };

    /// メイン処理を行う。
    bool execute(const std::string& file, const arg_store& store)
    {
        using namespace std;
        namespace al = boost::algorithm;

        // ファイルパス作成
        const auto from_file = cvtool::file::format_filename("$0", file);
        const auto to_file =
            cvtool::file::change_filename(
                cvtool::file::format_filename(store.format, file),
                store.prefix,
                store.suffix);

        cout << "FROM : " << from_file << endl;

        // ファイルパスが変わらないなら何もしない
        if (fs::path(from_file) == fs::path(to_file))
        {
            cout << " --> Do not rename." << endl;
            return true;
        }

        // 上書きできないなら何もしない
        if (!store.overwrite && fs::exists(to_file))
        {
            cout << " --> '" << to_file << "' is already exists. Do nothing." << endl;
            return true;
        }

        // 拡張子取得
        const auto from_ext =
            al::to_lower_copy(cvtool::file::filename_extension(from_file));
        const auto to_ext =
            al::to_lower_copy(cvtool::file::filename_extension(to_file));

        // 画像変換不要か？
        bool copy_only = (from_ext == to_ext || to_ext.empty());

        if (!copy_only)
        {
            // ファイル読み込み
            cv::Mat img = cvtool::image::read(from_file, -1);

            // 読み込み失敗なら単純コピーする
            copy_only = img.empty();
            if (copy_only)
            {
                cout << " --> '" << from_file << "' is not image file. -->" << endl;
            }
            else
            {
                // ファイル書き出し
                if (cvtool::image::write(to_file, img))
                {
                    cout << " --> Convert image... -->" << endl;
                }
                else
                {
                    // 書き出し失敗なら単純コピーする
                    copy_only = true;
                    cout << " --> Cannot write as image file. -->" << endl;
                }
            }
        }

        // 単純コピー
        if (copy_only)
        {
            fs::copy_file(from_file, to_file, fs::copy_option::overwrite_if_exists);
            cout << " --> Rename... -->" << endl;
        }

        cout << "  TO : " << to_file << endl;

        // コピーでないなら元ファイル削除
        if (!store.copy && !fs::remove(from_file))
        {
            cerr << "WARNING: Cannot remove file '" << from_file << "'." << endl;
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

    // プログラム引数をパース
    try
    {
        program.parse_args();
    }
    catch (const std::exception& ex)
    {
        cvtool::error::print(ex);
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
        cvtool::error::print("No input files.\nSet '--help' to print usage.");
        return 1;
    }

    // リネームオプションが1つも渡されなかった？
    if (store.format.empty() && store.prefix.empty() && store.suffix.empty())
    {
        cvtool::error::print("No rename options.\nSet '--help' to print usage.");
        return 1;
    }

    // ファイルごとに処理
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

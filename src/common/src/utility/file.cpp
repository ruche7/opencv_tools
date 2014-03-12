/// @file
/// @brief ファイル関連のユーティリティ処理定義ソース。
/// @author ruche

#include <cvtool/utility/file.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <algorithm>
#include <utility>
#include <functional>
#include <cctype>

namespace fs = boost::filesystem;

namespace
{
    /// フォーマット変数情報構造体。
    struct format_var_info
    {
        /// 変数名。('$' や '%' に続く文字)
        char name;

        /// 説明文。
        std::string description;

        /// フォーマット関数。
        std::function<std::string (const fs::path&)> formatter;
    };

    /// ファイルパスからピリオド抜きの拡張子文字列を取得する関数オブジェクト。
    const auto filepath_extension_getter =
        [](const fs::path& p)
        {
            const auto ext = p.extension().string();
            return (ext.empty() || ext.front() != '.') ? ext : ext.substr(1);
        };

    /// フォーマット変数情報配列。
    const format_var_info format_var_infos[] =
        {
            {
                'f',
                "file name with extension.",
                [](const fs::path& p) { return p.filename().string(); },
            },
            {
                'n',
                "file name without extension.",
                [](const fs::path& p) { return p.stem().string(); },
            },
            {
                'e',
                "file extension without period.",
                filepath_extension_getter,
            },
            {
                'p',
                "parent directory path.",
                [](const fs::path& p)
                {
                    return !p.has_parent_path() ?
                        std::string() :
                        (p.parent_path().string() +
                         static_cast<char>(fs::path::preferred_separator));
                },
            },
            {
                '0',
                "full name.",
                [](const fs::path& p) { return p.string(); },
            },
        };

    /// フォーマット変数のプレフィクス文字であるか否かを取得する。
    inline bool is_format_var_prefix(char c)
    {
        return (c == '$' || c == '%');
    }

    /// フォーマット変数名から対応するフォーマット変数情報を検索する。
    const format_var_info* find_format_var_info(char name)
    {
        const char lc_name = static_cast<char>(std::tolower(name));

        const auto itr_end = std::cend(format_var_infos);
        const auto itr =
            std::find_if(
                std::cbegin(format_var_infos),
                itr_end,
                [lc_name](const format_var_info& info)
                {
                    return (info.name == lc_name);
                });

        return (itr == itr_end) ? nullptr : &(*itr);
    }

    /// フォーマット変数名一覧の説明文を作成する。
    std::string make_format_var_infos_description()
    {
        std::string result;
        std::for_each(
            std::cbegin(format_var_infos),
            std::cend(format_var_infos),
            [&result](const format_var_info& info)
            {
                result += '$';
                result += info.name;
                result += " or ";
                result += '%';
                result += info.name;
                result += " : \t";
                result += info.description;
                result += "\n";
            });
        boost::algorithm::trim_right(result);

        return result;
    }

    /// フォーマット変数名一覧の説明文。
    const auto format_var_infos_description = make_format_var_infos_description();

    /// ファイルパスのプレフィクスを変更する。
    fs::path& change_filepath_prefix(fs::path& fpath, const std::string& prefix)
    {
        if (!prefix.empty())
        {
            const fs::path fname(fpath.filename());

            fs::path pre(prefix);
            if (pre.has_parent_path() || !fpath.has_parent_path())
            {
                fpath.swap(pre);
            }
            else
            {
                fpath.remove_filename();
                fpath /= pre;
            }
            fpath += fname;
        }

        return fpath;
    }

    /// ファイルパスのサフィックスを変更する。
    fs::path& change_filepath_suffix(fs::path& fpath, const std::string& suffix)
    {
        if (!suffix.empty())
        {
            const fs::path fext(fpath.extension());

            fpath.replace_extension();
            fpath += suffix;
            if (!fpath.has_extension())
            {
                fpath.replace_extension(fext);
            }
        }

        return fpath;
    }
}

namespace cvtool { namespace file
{
    bool create_parent_directories(const std::string& filename)
    {
        const fs::path fpath(filename);
        return (
            !fpath.has_parent_path()                ||
            fs::is_directory(fpath.parent_path())   ||
            fs::create_directories(fpath.parent_path()));
    }

    std::string filename_extension(const std::string& filename)
    {
        return filepath_extension_getter(fs::path(filename));
    }

    std::string format_filename(
        const std::string& format,
        const std::string& filename)
    {
        std::string result;

        if (format.empty())
        {
            result = filename;
        }
        else
        {
            const fs::path fpath = filename;
            for (std::string::size_type i = 0; i < format.size(); ++i)
            {
                const char c = format[i];
                const auto* const info =
                    (i + 1 < format.size() && is_format_var_prefix(c)) ?
                        find_format_var_info(format[i + 1]) : nullptr;
                if (info == nullptr)
                {
                    result += c;
                }
                else
                {
                    result += info->formatter(fpath);
                    ++i;
                }
            }
        }

        return result;
    }

    std::string change_filename_prefix(
        const std::string& filename,
        const std::string& prefix)
    {
        fs::path fpath = filename;
        return change_filepath_prefix(fpath, prefix).string();
    }

    std::string change_filename_suffix(
        const std::string& filename,
        const std::string& suffix)
    {
        fs::path fpath = filename;
        return change_filepath_suffix(fpath, suffix).string();
    }

    std::string change_filename(
        const std::string& filename,
        const std::string& prefix,
        const std::string& suffix)
    {
        fs::path fpath = filename;
        return
            change_filepath_suffix(
                change_filepath_prefix(fpath, prefix), suffix).string();
    }

    program_arg<std::string> make_filename_format_program_arg(
        const std::string& name,
        std::string& store)
    {
        return
            make_program_arg(
                name,
                "The format of file names. "
                "This value can be contained some letters and following variables.\n" +
                format_var_infos_description,
                store);
    }

    program_arg<std::string> make_filename_prefix_program_arg(
        const std::string& name,
        std::string& store)
    {
        return
            make_program_arg(
                name,
                "The prefix of file names. "
                "If this value contained directory path, directory pathes are replaced. "
                "Otherwise, directory pathes are kept.",
                store);
    }

    program_arg<std::string> make_filename_suffix_program_arg(
        const std::string& name,
        std::string& store)
    {
        return
            make_program_arg(
                name,
                "The suffix of file names. "
                "If this value contained extension, file extensions are replaced. "
                "Otherwise, file extensions are kept.",
                store);
    }
}}

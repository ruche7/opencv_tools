/// @file
/// @brief ファイル関連のユーティリティ処理定義ソース。
/// @author ruche

#include <cvtool/utility/file.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

namespace
{
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

namespace cvtool { namespace utility
{
    bool create_parent_directories(const std::string& filename)
    {
        const fs::path fpath(filename);
        return (
            !fpath.has_parent_path()                ||
            fs::is_directory(fpath.parent_path())   ||
            fs::create_directories(fpath.parent_path()));
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

    program_arg<std::string> make_filename_prefix_program_arg(
        const std::string& name,
        std::string& store)
    {
        return
            make_program_arg(
                name,
                "Change the prefix of file names. "
                "If value contained directory path, directory pathes are replaced. "
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
                "Change the suffix of file names. "
                "If value contained extension, file extensions are replaced. "
                "Otherwise, file extensions are kept.",
                store);
    }
}}

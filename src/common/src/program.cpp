/// @file
/// @brief ツールの基本的な処理を行うクラス program のソース。
/// @author ruche

#include <cvtool/program.hpp>

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem/path.hpp>
#include <vector>
#include <iostream>

namespace
{
    /// 既定のプログラム名。
    const char* const default_program_name = "program";

    /// 位置オプション登録名。
    const char* const positional_name = "__POSITIONAL__";
}

namespace cvtool
{
    void program::parse_args()
    {
        namespace po = boost::program_options;

        po::basic_command_line_parser<char> parser(argc_, argv_);

        po::options_description desc;
        po::positional_options_description pos_desc;

        help_required_ = false;
        positional_store_.clear();

        if (allow_positional())
        {
            desc.add_options()(positional_name, po::value(&positional_store_));
            desc.add(desc_);
            parser.options(desc);

            pos_desc.add(positional_name, positional_arg_count_);
            parser.positional(pos_desc);
        }
        else
        {
            parser.options(desc_);
        }

        po::variables_map vm;
        po::store(parser.run(), vm);
        po::notify(vm);
    }

    bool program::allow_positional() const
    {
        return (!positional_arg_name_.empty() && positional_arg_count_ != 0);
    }

    std::size_t program::parsed_positional_arg_count() const
    {
        return allow_positional() ? positional_store_.size() : 0;
    }

    const char* program::parsed_positional_arg(std::size_t index) const
    {
        return (allow_positional() && index < positional_store_.size()) ?
            positional_store_[index].c_str() : nullptr;
    }

    bool program::help_required() const
    {
        return help_required_;
    }

    void program::print_usage() const
    {
        print_usage(std::cout);
    }

    void program::print_usage(std::ostream& s) const
    {
        s << "Usage: " << make_usage() << std::endl;
    }

    void program::print_help() const
    {
        print_help(std::cout);
    }

    void program::print_help(std::ostream& s) const
    {
        print_usage(s);
        s << std::endl;
        s << desc_ << std::endl;
    }

    std::string program::make_usage() const
    {
        return
            program_name_   +
            " [options...]" +
            (allow_positional() ? (" " + positional_arg_name_) : "");
    }

    void program::make_options(boost::program_options::options_description& desc)
    {
        // ヘルプ表示オプションを追加
        add_to_options(
            desc,
            make_switch_program_arg("help", "Show this help.", help_required_));
    }

    std::string program::make_program_name(int argc, const char* const argv[])
    {
        return (argc >= 1 && argv != 0 && argv[0] != 0) ?
            boost::filesystem::path(argv[0]).filename().string() :
            default_program_name;
    }
}

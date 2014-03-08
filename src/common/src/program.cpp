/// @file
/// @brief ツールの基本的な処理を行うクラス program のソース。
/// @author ruche

#pragma warning(push)
#pragma warning(disable: 4512)

#include <cvtool/program.hpp>

#pragma warning(pop)

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/filesystem/path.hpp>
#include <vector>
#include <iostream>

namespace
{
    const char* const default_program_name = "program";
}

namespace cvtool
{
    void program::parse_args()
    {
        namespace po = boost::program_options;

        po::basic_command_line_parser<char> parser(argc_, argv_);

        parser.options(desc_);

        po::positional_options_description pos_desc;
        if (allow_positional())
        {
            pos_desc.add(positional_arg_name_.c_str(), positional_arg_count_);
            parser.positional(pos_desc);
        }

        po::store(parser.run(), vm_);
    }

    bool program::allow_positional() const
    {
        return (!positional_arg_name_.empty() && positional_arg_count_ != 0);
    }

    std::size_t program::parsed_positional_arg_count() const
    {
        return allow_positional() ? vm_.count(positional_arg_name_) : 0;
    }

    const char* program::parsed_positional_arg(std::size_t index) const
    {
        return (allow_positional() && index < parsed_positional_arg_count()) ?
            vm_[positional_arg_name_].as<std::vector<std::string>>()[index].c_str() :
            nullptr;
    }

    bool program::help_required() const
    {
        return (vm_.count("help") > 0);
    }

    void program::print_help() const
    {
        print_help(std::cout);
    }

    void program::print_help(std::ostream& s) const
    {
        s << "Usage: " << make_usage() << std::endl;
        s << std::endl;
        s << desc_ << std::endl;
    }

    void program::print_error(const std::exception& ex) const
    {
        print_error(std::cerr, ex);
    }

    void program::print_error(std::ostream& s, const std::exception& ex) const
    {
        auto msg = ex.what();
        s << "ERROR: " << ((msg == nullptr) ? "Unknown error." : msg) << std::endl;
    }

    std::string program::make_usage() const
    {
        auto usage = std::string(program_name_).append(" [options...]");
        if (allow_positional())
        {
            usage.append(" ").append(positional_arg_name_);
            if (positional_arg_count_ != 1)
            {
                usage.append("...");
            }
        }

        return usage;
    }

    std::string program::make_program_name(int argc, const char* const argv[])
    {
        return (argc >= 1 && argv != 0 && argv[0] != 0) ?
            boost::filesystem::path(argv[0]).filename().string() :
            default_program_name;
    }

    void program::make_options(boost::program_options::options_description& desc)
    {
        // ヘルプ表示オプションを追加
        desc.add_options()("help,h", "Show this help.");
    }
}

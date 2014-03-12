/// @file
/// @brief 画像関連のユーティリティ処理定義ソース。
/// @author ruche

#include <cvtool/utility/image.hpp>

#include <opencv2/core/core.hpp>

namespace
{
    /// OpenCVエラーコールバックを一時的に置換するRAIIクラス。
    class cv_error_callback_redirect
    {
    public:
        explicit cv_error_callback_redirect(cv::ErrorCallback callback)
            :
            callback_old_(cv::redirectError(callback))
        {
        }

        ~cv_error_callback_redirect()
        {
            cv::redirectError(callback_old_);
        }

    private:
        cv::ErrorCallback callback_old_;
    };

    /// エラー出力を行わないOpenCVエラーコールバック。
    int CV_CDECL no_print_on_cv_error(
        int, const char*, const char*, const char*, int, void*)
    {
        // do nothing
        return 0;
    }
}

namespace cvtool { namespace image
{
    cv::Mat read(const std::string& filename, int flags)
    {
        cv_error_callback_redirect r(&no_print_on_cv_error);

        try
        {
            return cv::imread(filename, flags);
        }
        catch (...) { }
        return cv::Mat();
    }

    bool write(
        const std::string& filename,
        cv::InputArray img,
        const cv::vector<int>& params)
    {
        cv_error_callback_redirect r(&no_print_on_cv_error);

        try
        {
            return cv::imwrite(filename, img, params);
        }
        catch (...) { }
        return false;
    }
}}

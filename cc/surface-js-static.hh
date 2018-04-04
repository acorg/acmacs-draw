#pragma once

#include <memory>
#include <fstream>

// ----------------------------------------------------------------------

namespace acmacs
{
    class Size;
    class Viewport;
}

namespace acmacs::surface
{
    class JsStatic
    {
      public:
        JsStatic(std::string filename, const acmacs::Size& canvas_size, const acmacs::Viewport& viewport);
        ~JsStatic();

     private:
        std::ofstream output_;
        const char* const indent_ = "        ";

    }; // class Html

} // namespace acmacs::surface

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:

#pragma once

#include "acmacs-draw/surface-cairo.hh"

// ----------------------------------------------------------------------

namespace acmacs::drawi::inline v1
{
    class Settings;

    class Draw
    {
      public:
        Draw(std::string_view filename, double size);
        ~Draw() { draw(); }

        void draw() const;

        constexpr bool settings_present() const { return settings_ != nullptr; }
        constexpr const Settings& settings() const { return *settings_; }

      private:
        acmacs::surface::PdfCairo surface_;
        const Settings* settings_{nullptr};

        void settings(const Settings* a_settings) { settings_ = a_settings; }

        friend class Settings;
    };

} // namespace acmacs::drawi::inline v1

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:

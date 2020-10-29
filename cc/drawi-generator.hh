#pragma once

#include "acmacs-draw/viewport.hh"

// ----------------------------------------------------------------------

namespace acmacs::drawi::inline v1
{
    class Generator
    {
      public:
        void generate(std::string_view filename) const;

        constexpr Viewport& viewport() { return viewport_; }
        constexpr const Viewport& viewport() const { return viewport_; }

      private:
        Viewport viewport_{-5, -5, 10};
    };
} // namespace acmacs::drawi::inline v1

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:

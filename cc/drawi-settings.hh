#pragma once

#include "acmacs-base/settings-v3.hh"

// ----------------------------------------------------------------------

namespace acmacs::drawi::inline v1
{
    class Draw;

    using error = settings::v3::error;

    class Settings : public settings::v3::Data
    {
      public:
        Settings(Draw& draw);

        using settings::v3::Data::load;

        bool apply_built_in(std::string_view name) override; // returns true if built-in command with that name found and applied

    protected:
        bool apply_viewport();
        bool apply_grid();
        bool apply_point();

      private:
        Draw& draw_;

        void update_env();

    };

}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:

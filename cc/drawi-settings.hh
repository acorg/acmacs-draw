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

      private:
        Draw& draw_;

        void update_env();

    };

}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:

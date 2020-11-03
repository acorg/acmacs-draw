#pragma once

#include "acmacs-base/settings-v3.hh"

// ----------------------------------------------------------------------

namespace acmacs
{
    class PointStyle;
}

namespace acmacs::draw
{
    class DrawElementsToFile;
    class PointLabel;
    class PointRefs;
}

namespace acmacs::drawi::inline v1
{
    class Draw;

    using error = settings::v3::error;

    class unrecognized : public error
    {
      public:
        using error::error;
        unrecognized() : error{""} {}
    };

    class Settings : public settings::v3::Data
    {
      public:
        Settings(acmacs::draw::DrawElementsToFile& draw);

        using settings::v3::Data::load;

        bool apply_built_in(std::string_view name) override; // returns true if built-in command with that name found and applied

      protected:
        bool apply_viewport();
        bool apply_point();
        bool apply_point_modify();

        bool apply_background();
        bool apply_border();
        bool apply_grid();

      private:
        acmacs::draw::DrawElementsToFile& draw_;

        void update_env();
        void update_label(acmacs::draw::PointLabel& label, const rjson::v3::value& source);

        void update_style(acmacs::PointStyle& style, std::string_view key, const rjson::v3::value& val);
        void update_style(size_t point_no, std::string_view key, const rjson::v3::value& val);
        void update_style(acmacs::draw::PointRefs& refs, std::string_view key, const rjson::v3::value& val);
        template <typename StyleRef> void update_style(StyleRef&& ref);
    };
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:

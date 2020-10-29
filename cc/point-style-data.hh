#pragma once

#include "acmacs-base/point-style.hh"

namespace acmacs::draw
{
    class PointStylesData : public PointStyles
    {
      public:
        size_t number_of_points() const override { return data_.size(); }
        bool empty() const override { return data_.empty(); }
        PointStyle style(size_t aPointNo) const override { return data_[aPointNo]; }
        PointStylesCompacted compacted() const override;

        PointStyle& add(const PointStyle& style) { data_.push_back(style); return data_.back(); }

      private:
        std::vector<PointStyle> data_;
    };

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:

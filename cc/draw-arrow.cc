#include "acmacs-draw/draw-arrow.hh"

// ----------------------------------------------------------------------

void acmacs::draw::Line::transform(const Transformation& transformation) const
{
    if (apply_transformation_) {
        from_ = transformation.transform(from_);
        to_ = transformation.transform(to_);
    }

} // acmacs::draw::Line::transform

// ----------------------------------------------------------------------

void acmacs::draw::Line::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::procrustes_arrows)
        surface.line(from_, to_, line_color_, line_width_);

} // acmacs::draw::Line::draw

// ----------------------------------------------------------------------

void acmacs::draw::Path::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::procrustes_arrows) {
        surface.path_outline(std::begin(path_), std::end(path_), line_color_, line_width_, close_and_fill_.has_value());
        if (close_and_fill_)
            surface.path_fill(std::begin(path_), std::end(path_), *close_and_fill_);
    }

} // acmacs::draw::Path::draw

// ----------------------------------------------------------------------

void acmacs::draw::LineDefinedByEquation::transform(const Transformation& transformation) const
{
    if (apply_transformation_)
        line_ = transformation.transform(line_);

} // acmacs::draw::LineDefinedByEquation::transform

// ----------------------------------------------------------------------

void acmacs::draw::LineDefinedByEquation::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::procrustes_arrows) {
        const Viewport& viewport = surface.viewport();
        const acmacs::PointCoordinates
                from{viewport.left(), viewport.left() * line_.slope() + line_.intercept()},
                to{viewport.right(), viewport.right() * line_.slope() + line_.intercept()};
        surface.line(from, to, line_color_, line_width_);
    }

} // acmacs::draw::LineDefinedByEquation::draw

// ----------------------------------------------------------------------

void acmacs::draw::Arrow::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::procrustes_arrows) {
        const bool x_eq = float_equal(to().x(), from().x());
        const double sign2 = x_eq ? (from().y() < to().y() ? 1.0 : -1.0) : (to().x() < from().x() ? 1.0 : -1.0);
        const double angle = x_eq ? -M_PI_2 : std::atan((to().y() - from().y()) / (to().x() - from().x()));
        const auto end = surface.arrow_head(to(), angle, sign2, arrow_head_color_, arrow_width_, arrow_head_filled_);
        surface.line(from(), end, line_color(), line_width());
    }

} // acmacs::draw::Arrow::draw

// ----------------------------------------------------------------------

void acmacs::draw::Rectangle::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::procrustes_arrows) {
        const std::vector<acmacs::PointCoordinates> path{corner1_, {corner1_.x(), corner2_.y()}, corner2_, {corner2_.x(), corner1_.y()}};
        if (filled_)
            surface.path_fill(path.begin(), path.end(), color_);
        else
            surface.path_outline(path.begin(), path.end(), color_, line_width_, true);
    }

} // acmacs::draw::Rectangle::draw

// ----------------------------------------------------------------------

void acmacs::draw::Sector::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == stage_) {
        if (start_ == end_)
            surface.circle_filled(center_, size_, AspectNormal, NoRotation, outline_color_, outline_width_, outline_dash_, fill_color_);
        else
            surface.sector_filled(center_, size_, start_, end_, outline_color_, outline_width_, radius_color_, radius_width_, radius_dash_, fill_color_);
    }

} // acmacs::draw::Sector::draw

// ----------------------------------------------------------------------

void acmacs::draw::SerumCircle::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::serum_circles) {
        center(transformation_.transform(coordinates_));
        Sector::draw(stage, surface);
    }

} // acmacs::draw::SerumCircle::draw

// ----------------------------------------------------------------------

bool acmacs::draw::PathWithArrows::ArrowData::valid(size_t path_size) const
{
    return at_ < path_size && (from_.has_value() ? ((*from_ + 1) == at_ || (*from_ - 1) == at_) : (at_ == 0 || at_ == (path_size - 1)));

} // acmacs::draw::PathWithArrows::ArrowData::valid

// ----------------------------------------------------------------------

void acmacs::draw::PathWithArrows::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::procrustes_arrows) {
        if (close_)
            surface.path_fill(std::begin(path_), std::end(path_), fill_);
        surface.path_outline(std::begin(path_), std::end(path_), outline_, outline_width_, close_);
        draw_arrows(surface);
    }

} // acmacs::draw::PathWithArrows::draw

// ----------------------------------------------------------------------

void acmacs::draw::PathWithArrows::draw_arrows(surface::Surface& surface) const
{
        for (const auto& arrow : arrows_) {
            if (const auto from_no = arrow.from();
                arrow.at() < path_.size() && (from_no.has_value() ? ((*from_no + 1) == arrow.at() || (*from_no - 1) == arrow.at()) : (arrow.at() == 0 || arrow.at() == (path_.size() - 1)))) {
                const auto& to = path_[arrow.at()];
                const auto& from = from_no.has_value() ? path_.at(*from_no) : (arrow.at() == 0 ? path_.at(1) : path_.at(path_.size() - 2));

                const bool x_eq = float_equal(to.x(), from.x());
                const double sign2 = x_eq ? (from.y() < to.y() ? 1.0 : -1.0) : (to.x() < from.x() ? 1.0 : -1.0);
                const double angle = x_eq ? -M_PI_2 : std::atan((to.y() - from.y()) / (to.x() - from.x()));
                const auto end = surface.arrow_head(to, angle, sign2, arrow.width(), arrow.outline(outline_), arrow.outline_width(), arrow.fill(outline_));
            }
            else
                AD_WARNING("invalid arrow specification for arrow at {}", arrow.at());
        }

} // acmacs::draw::PathWithArrows::draw_arrows

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:

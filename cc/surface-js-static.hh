#pragma once

#include <memory>
#include <fstream>

#include "acmacs-base/color.hh"
#include "acmacs-draw/viewport.hh"

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

        template <typename ... Args> void add(Args&& ... args) { ((output_ << indent_) << ... << args) << '\n'; }

        template <typename F, typename Arg1, typename ... Args> void func(F&& func, Arg1&& arg1, Args&& ... args)
            {
                output_ << indent_ << func << '(' << convert(arg1);
                ((output_ << ',' << convert(args)), ...);
                output_ << ");\n";
            }

        template <typename F> void func(F&& func) { output_ << indent_ << func << "();\n"; }

        template <typename F, typename... Args> void context_func(F&& f, Args&&... args) { func(std::string("__context.") + f, args...); }
        template <typename Var, typename Val> void context_assign(Var&& var, Val&& val) { output_ << indent_ << "__context." << var << " = " << convert(val) << ";\n"; }

        const acmacs::Viewport& viewport() const { return viewport_; }
        double convert(Pixels a) const { return a.value() / scale_; }
        double convert(Scaled a) const { return a.value(); }
        double convert(Rotation a) const { return a.value(); }
        double convert(Aspect a) const { return a.value(); }
        double convert(double a) const { return a; }
        int convert(int a) const { return a; }
        std::string convert(Color a) const { return '"' + a.to_hex_string() + '"'; }
        const char* convert(const char* a) const { return a; }

        struct ContextSave
        {
            ContextSave(JsStatic& aParent) : parent(aParent) { parent.context_func("save"); }
            ~ContextSave() { parent.context_func("restore"); }
            JsStatic& parent;
        };

        struct WithSubsurface : public ContextSave
        {
            WithSubsurface(JsStatic& parent, const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip);
        };

     private:
        std::ofstream output_;
        const char* const indent_ = "        ";
        acmacs::Viewport viewport_;
        double scale_;

    }; // class JsStatic

} // namespace acmacs::surface

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:

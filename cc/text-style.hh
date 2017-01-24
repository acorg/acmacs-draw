#pragma once

#include <string>

// ----------------------------------------------------------------------

class TextStyle
{
 public:
    enum class Slant { Normal, Italic };
    enum class Weight { Normal, Bold };

    inline TextStyle() : mSlant(Slant::Normal), mWeight(Weight::Normal) {}
    inline TextStyle(std::string aFontFamily, Slant aSlant = Slant::Normal, Weight aWeight = Weight::Normal) : mFontFamily(aFontFamily), mSlant(aSlant), mWeight(aWeight) {}

    inline std::string font_family() const { return mFontFamily; }
    inline Slant slant() const { return mSlant; }
    inline Weight weight() const { return mWeight; }

    inline std::string slant_as_stirng() const
        {
            switch (mSlant) {
              case Slant::Normal:
                  return "normal";
              case Slant::Italic:
                  return "italic";
            }
            return "normal";
        }

    inline std::string weight_as_stirng() const
        {
            switch (mWeight) {
              case Weight::Normal:
                  return "normal";
              case Weight::Bold:
                  return "bold";
            }
            return "normal";
        }

    inline std::string& font_family() { return mFontFamily; }
    inline void font_family(std::string s) { mFontFamily = s; }
    inline void font_family(const char* s, size_t len) { mFontFamily.assign(s, len); }

    inline void slant(Slant aSlant) { mSlant = aSlant; }
    inline void slant(std::string aSlant)
        {
            if (aSlant == "normal")
                mSlant = Slant::Normal;
            else if (aSlant == "italic")
                mSlant = Slant::Italic;
            else
                throw std::runtime_error("Unrecognized TextStyle slant: " + aSlant);
        }
    inline void slant(const char* s, size_t len) { slant(std::string(s, len)); }

    inline void weight(Weight aWeight) { mWeight = aWeight; }
    inline void weight(std::string aWeight)
        {
            if (aWeight == "normal")
                mWeight = Weight::Normal;
            else if (aWeight == "bold")
                mWeight = Weight::Bold;
            else
                throw std::runtime_error("Unrecognized TextStyle weight: " + aWeight);
        }
    inline void weight(const char* s, size_t len) { weight(std::string(s, len)); }

 private:
    std::string mFontFamily;
    Slant mSlant;
    Weight mWeight;

}; // class TextStyle


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:

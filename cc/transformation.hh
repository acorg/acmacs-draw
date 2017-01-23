#pragma once

#include <iostream>
#include <vector>

// ----------------------------------------------------------------------

class Transformation : public std::vector<double>
{
  public:
    inline Transformation() : std::vector<double>{1, 0, 0, 1} {}
    inline Transformation(const Transformation&) = default;
    inline Transformation(double a11, double a12, double a21, double a22) : std::vector<double>{a11, a12, a21, a22} {}
    inline Transformation& operator=(const Transformation& t) = default;

    inline void multiply_by(const Transformation& t)
    {
        (*this)[0] = (*this)[0] * t[0] + (*this)[1] * t[2];
        (*this)[1] = (*this)[0] * t[1] + (*this)[1] * t[3];
        (*this)[2] = (*this)[2] * t[0] + (*this)[3] * t[2];
        (*this)[3] = (*this)[2] * t[1] + (*this)[3] * t[3];
    }

};

// ----------------------------------------------------------------------

inline std::ostream& operator << (std::ostream& out, const Transformation& t)
{
    return out << "[[" << t[0] << ", " << t[1] << "], [" << t[2] << ", " << t[3] << "]]";
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:

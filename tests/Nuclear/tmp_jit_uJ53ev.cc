#define _USE_MATH_DEFINES
#include <cmath>
extern "C" double f_732a6e65cf58f617e1d9e257deac354225a90936(const double *params, const double *immed, const double eps) {
double r, s[3];
s[0] = params[2];
s[1] = immed[0];
s[2] = params[1];
s[1] += s[2];
s[0] *= s[1];
s[1] = immed[1];
s[0] *= s[1];
return s[0]; }

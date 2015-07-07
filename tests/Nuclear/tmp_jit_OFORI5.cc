#define _USE_MATH_DEFINES
#include <cmath>
extern "C" double f_80cb27319d51232e8adcd0af26f637948ce0a6b4(const double *params, const double *immed, const double eps) {
double r, s[3];
s[0] = params[0];
s[0] *= s[0];
s[1] = immed[0];
s[2] = params[1];
s[1] += s[2];
s[1] *= s[1];
s[0] += s[1];
s[1] = s[0];
s[0] += s[1];
return s[0]; }

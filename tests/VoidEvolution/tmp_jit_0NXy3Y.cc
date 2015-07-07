#define _USE_MATH_DEFINES
#include <cmath>
extern "C" double f_8c402d5a328fbf2c2411ba8c902d54ba5db8533e(const double *params, const double *immed, const double eps) {
double r, s[9];
s[0] = params[1];
s[0] *= s[0];
s[1] = immed[0];
s[2] = params[0];
s[3] = s[2];
s[2] += s[3];
s[1] += s[2];
s[2] = immed[1];
s[3] = s[0];
s[4] = immed[2];
s[5] = params[1];
s[6] = immed[3];
s[5] *= s[6];
s[4] += s[5];
s[3] *= s[4];
s[2] += s[3];
s[1] *= s[2];
s[2] = params[1];
s[3] = immed[4];
s[4] = params[0];
s[3] += s[4];
s[4] = immed[5];
s[3] *= s[4];
s[4] = immed[6];
s[5] = s[0];
s[4] += s[5];
s[5] = immed[7];
s[6] = params[0];
s[6] = std::log(s[6]);
s[7] = params[0];
s[8] = immed[8];
s[7] = s[8] - s[7];
s[7] = std::log(s[7]);
s[6] -= s[7];
s[7] = immed[9];
s[6] *= s[7];
s[5] += s[6];
s[4] *= s[5];
s[3] += s[4];
s[2] *= s[3];
s[1] += s[2];
return s[1]; }

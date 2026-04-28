#include <ap_fixed.h>
#include "stdio.h"

typedef ap_fixed<32,16> FIXED_POINT;

//typedef ap_fixed <12,2> FIXED_POINT;

void cordic(FIXED_POINT theta, FIXED_POINT *s, FIXED_POINT *c);

void cordic_rotator(FIXED_POINT theta, FIXED_POINT *sin, FIXED_POINT* cos);
void cordic_top(float theta_float, float* cos, float* sin);


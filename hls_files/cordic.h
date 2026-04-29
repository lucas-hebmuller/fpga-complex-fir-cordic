#ifndef CORDIC_H
#define CORDIC_H

#include "fir.h"

void cordic_rotator(FIXED_POINT cos, FIXED_POINT sin, FIXED_POINT* magnitude, FIXED_POINT* phase);

#endif

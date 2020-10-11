#ifndef UTILITY_H
#define UTILITY_H

#define FLT_MAX         3.402823466e+38F
#define FLT_MIN         1.175494351e-38F

double small(double a, double b, double c = FLT_MAX);


double big(double a, double b, double c = FLT_MIN);


#endif
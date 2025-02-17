/*

The MIT License (MIT)

Copyright (c) 2015 Jacques-Henri Jourdan <jourgun@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef SIMD_MATH_PRIMS_H
#define SIMD_MATH_PRIMS_H

#include<math.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Workaround a lack of optimization in gcc */
float exp_cst1 = 2139095040.f;
float exp_cst2 = 0.f;

/* Relative error bounded by 1e-5 for normalized outputs
   Returns invalid outputs for nan inputs */
inline float expapprox(float val) {
  union { int i; float f; } xu, xu2;
  float val2, val3, val4, b;
  int val4i;
  val2 = 12102203.1615614f*val+1065353216.f;
  val3 = val2 < exp_cst1 ? val2 : exp_cst1;
  val4 = val3 > exp_cst2 ? val3 : exp_cst2;
  val4i = (int) val4;
  xu.i = val4i & 0x7F800000;
  xu2.i = (val4i & 0x7FFFFF) | 0x3F800000;
  b = xu2.f;
  return
    xu.f * (0.509964287281036376953125f + b *
            (0.3120158612728118896484375f + b *
             (0.1666135489940643310546875f + b *
              (-2.12528370320796966552734375e-3f + b *
               1.3534179888665676116943359375e-2f))));
}

/* Absolute error bounded by 3e-5 for normalized inputs
   Returns a finite number for +inf input
   Returns -inf for nan and <= 0 inputs. */
inline float logapprox(float val) {
  union { float f; int i; } valu;
  float exp, addcst, x;
  valu.f = val;
  exp = valu.i >> 23;
  addcst = val > 0 ? -89.93423858f : -(float)INFINITY;
  valu.i = (valu.i & 0x7FFFFF) | 0x3F800000;
  x = valu.f;

  float x2 = x*x;

  return
    (3.3977745f * x + addcst) +
    x2 * ((x - 2.2744832f) +
          x2 * (0.024982445f * x - 0.24371102f)) +
    0.69314718055995f*exp;
}

/* Correct only in [-pi, pi]
   Absolute error bounded by 5e-5 */
inline float cosapprox(float val) {
  float val2 = val*val;
  return
    0.999959766864776611328125f + val2 *
    (-0.4997930824756622314453125f + val2 *
     (4.1496001183986663818359375e-2f + val2 *
      (-1.33926304988563060760498046875e-3f + val2 *
       1.8791708498611114919185638427734375e-5f)));
}

/* Correct only in [-pi, pi]
   Absolute error bounded by 6e-6 */
inline float sinapprox(float val) {
  float val2 = val*val;
  return
    val * (0.99997937679290771484375f + val2 *
           (-0.166624367237091064453125f + val2 *
            (8.30897875130176544189453125e-3f + val2 *
             (-1.92649182281456887722015380859375e-4f + val2 *
              2.147840177713078446686267852783203125e-6f))));
}

#ifdef __cplusplus
}
#endif

#endif

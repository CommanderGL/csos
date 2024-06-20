#ifndef MATH_H
#define MATH_H

inline double sqrt(double n) {
  double i = 0;
  double j = n / 2;

  while (j != i) {
    i = j;
    j = (n / i + i) / 2;
  }

  return j;
}

#endif

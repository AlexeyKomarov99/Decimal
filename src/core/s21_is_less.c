#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

int s21_is_less(s21_decimal a, s21_decimal b) {
  int result = 0;
  int sign_a = s21_get_sign(a);
  int sign_b = s21_get_sign(b);

  if (sign_a != sign_b) {
    result = (sign_a > sign_b);
  } else {
    s21_normalize(&a, &b);
    int compare = s21_compare_bits(a, b);

    if (sign_a == 0) {
      result = (compare < 0);
    } else {
      result = (compare > 0);
    }
  }

  return result;
}
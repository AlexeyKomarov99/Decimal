#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

int s21_is_equal(s21_decimal a, s21_decimal b) {
  int result = 0;
  int sign_a = s21_get_sign(a);
  int sign_b = s21_get_sign(b);

  if (sign_a != sign_b) {
    result = (s21_is_zero(a) && s21_is_zero(b));
  } else {
    s21_normalize(&a, &b);
    int compare_result = s21_compare_bits(a, b);
    result = (compare_result == 0);
  }

  return result;
}
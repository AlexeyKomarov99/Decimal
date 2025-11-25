#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int return_code = 1;

  if (result) {
    *result = value;
    int current_sign = s21_get_sign(*result);
    s21_set_sign(result, !current_sign);
    return_code = 0;
  }

  return return_code;
}
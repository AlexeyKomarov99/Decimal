#include <stdio.h>

#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int return_code = 1;

  if (result) {
    s21_init_decimal(result);
    return_code = s21_perform_addition(value_1, value_2, result);
  }

  return return_code;
}

int s21_perform_addition(s21_decimal value_1, s21_decimal value_2,
                         s21_decimal *result) {
  int return_code = 0;

  if (s21_is_zero(value_1)) {
    *result = value_2;
  } else if (s21_is_zero(value_2)) {
    *result = value_1;
  } else {
    return_code = s21_add_non_zero_values(value_1, value_2, result);
  }

  return return_code;
}

int s21_add_non_zero_values(s21_decimal value_1, s21_decimal value_2,
                            s21_decimal *result) {
  int return_code = 0;
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);

  s21_normalize(&value_1, &value_2);
  int final_scale = s21_get_scale(value_1);

  if (sign1 == sign2) {
    return_code =
        s21_add_same_sign(value_1, value_2, result, sign1, final_scale);
  } else {
    return_code = s21_add_different_signs(value_1, value_2, result, sign1,
                                          sign2, final_scale);
  }

  return return_code;
}

int s21_add_same_sign(s21_decimal value_1, s21_decimal value_2,
                      s21_decimal *result, int sign, int scale) {
  int return_code = 0;
  int overflow = s21_add_bits(value_1, value_2, result);

  if (overflow) {
    return_code = s21_handle_overflow(value_1, value_2, result, sign, scale);
  } else {
    s21_set_sign(result, sign);
    s21_set_scale(result, scale);
  }

  return return_code;
}

int s21_handle_overflow(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal *result, int sign, int scale) {
  int return_code = 0;

  if (scale > 0) {
    return_code = s21_reduce_scale_and_retry(&value_1, &value_2, result, scale);
  } else {
    return_code = sign ? 2 : 1;
  }

  return return_code;
}

int s21_reduce_scale_and_retry(s21_decimal *value_1, s21_decimal *value_2,
                               s21_decimal *result, int scale) {
  s21_set_scale(value_1, scale - 1);
  s21_set_scale(value_2, scale - 1);
  s21_div_by_10(value_1);
  s21_div_by_10(value_2);

  return s21_add(*value_1, *value_2, result);
}

int s21_add_different_signs(s21_decimal value_1, s21_decimal value_2,
                            s21_decimal *result, int sign1, int sign2,
                            int scale) {
  int return_code = 0;

  s21_set_sign(&value_1, 0);
  s21_set_sign(&value_2, 0);

  int compare = s21_compare_bits(value_1, value_2);

  if (compare >= 0) {
    s21_sub_bits(value_1, value_2, result);
    s21_set_sign(result, sign1);
  } else {
    s21_sub_bits(value_2, value_1, result);
    s21_set_sign(result, sign2);
  }

  s21_set_scale(result, scale);

  return return_code;
}
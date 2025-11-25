#include <stdio.h>

#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

static int s21_adjust_negative_scale(s21_decimal *quotient, int *result_scale,
                                     int result_sign);
static int s21_adjust_positive_scale(s21_decimal *quotient, int *result_scale);
static int s21_adjust_division_result(s21_decimal *quotient, int *result_scale,
                                      int result_sign, s21_decimal *result);
static int s21_perform_bit_division(s21_decimal value_1, s21_decimal value_2,
                                    s21_decimal *quotient);
static int s21_increase_dividend_precision(s21_decimal *value_1, int *scale1,
                                           int scale2, int *extra_scale);
static int s21_prepare_division_values(s21_decimal *value_1,
                                       s21_decimal *value_2, int *result_sign,
                                       int *result_scale);
static int s21_execute_division(s21_decimal value_1, s21_decimal value_2,
                                s21_decimal *result);
static int s21_validate_division_parameters(s21_decimal value_2,
                                            s21_decimal *result);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int return_code = 1;

  if (result) {
    return_code = s21_validate_division_parameters(value_2, result);

    if (return_code == 0 && !s21_is_zero(value_1)) {
      return_code = s21_execute_division(value_1, value_2, result);
    }
  }

  return return_code;
}

static int s21_validate_division_parameters(s21_decimal value_2,
                                            s21_decimal *result) {
  int return_code = 0;

  if (s21_is_zero(value_2)) {
    return_code = 3;
  } else {
    s21_init_decimal(result);
  }

  return return_code;
}

static int s21_execute_division(s21_decimal value_1, s21_decimal value_2,
                                s21_decimal *result) {
  int return_code = 0;
  int result_sign = 0;
  int result_scale = 0;
  s21_decimal quotient = {0};

  return_code = s21_prepare_division_values(&value_1, &value_2, &result_sign,
                                            &result_scale);

  if (return_code == 0) {
    return_code = s21_perform_bit_division(value_1, value_2, &quotient);
  }

  if (return_code == 0) {
    return_code = s21_adjust_division_result(&quotient, &result_scale,
                                             result_sign, result);
  }

  return return_code;
}

static int s21_prepare_division_values(s21_decimal *value_1,
                                       s21_decimal *value_2, int *result_sign,
                                       int *result_scale) {
  int return_code = 0;
  int sign1 = s21_get_sign(*value_1);
  int sign2 = s21_get_sign(*value_2);
  *result_sign = sign1 ^ sign2;

  s21_set_sign(value_1, 0);
  s21_set_sign(value_2, 0);

  int scale1 = s21_get_scale(*value_1);
  int scale2 = s21_get_scale(*value_2);
  int extra_scale = 0;

  int precision_increased =
      s21_increase_dividend_precision(value_1, &scale1, scale2, &extra_scale);

  s21_set_scale(value_1, scale1);
  s21_set_scale(value_2, scale2);
  *result_scale = scale1 - scale2 + extra_scale;

  if (precision_increased == 0) {
    return_code = *result_sign ? 2 : 1;
  }

  return return_code;
}

static int s21_increase_dividend_precision(s21_decimal *value_1, int *scale1,
                                           int scale2, int *extra_scale) {
  int success = 1;
  *extra_scale = 0;

  while (*scale1 < scale2 + 10 && *extra_scale < 20 && success) {
    if (s21_mul_by_10(value_1) == 0) {
      (*scale1)++;
      (*extra_scale)++;
    } else {
      success = 0;
    }
  }

  return success;
}

static int s21_perform_bit_division(s21_decimal value_1, s21_decimal value_2,
                                    s21_decimal *quotient) {
  s21_decimal remainder = {0};
  int i = 95;

  while (i >= 0) {
    s21_shift_left(&remainder);

    if (s21_get_bit(value_1, i)) {
      remainder.bits[0] |= 1;
    }

    if (s21_compare_bits(remainder, value_2) >= 0) {
      s21_sub_bits(remainder, value_2, &remainder);
      s21_decimal bit_mask = {0};
      bit_mask.bits[i / 32] = 1 << (i % 32);
      s21_add_bits(*quotient, bit_mask, quotient);
    }

    i--;
  }

  return 0;
}

static int s21_adjust_division_result(s21_decimal *quotient, int *result_scale,
                                      int result_sign, s21_decimal *result) {
  int return_code = 0;

  return_code = s21_adjust_negative_scale(quotient, result_scale, result_sign);

  if (return_code == 0) {
    return_code = s21_adjust_positive_scale(quotient, result_scale);
  }

  if (return_code == 0) {
    *result = *quotient;
    s21_set_sign(result, result_sign);
    s21_set_scale(result, *result_scale);
  }

  return return_code;
}

static int s21_adjust_negative_scale(s21_decimal *quotient, int *result_scale,
                                     int result_sign) {
  int return_code = 0;

  while (*result_scale < 0 && return_code == 0) {
    if (s21_mul_by_10(quotient)) {
      return_code = result_sign ? 2 : 1;
    } else {
      (*result_scale)++;
    }
  }

  return return_code;
}

static int s21_adjust_positive_scale(s21_decimal *quotient, int *result_scale) {
  int return_code = 0;

  while (*result_scale > 28 && !s21_is_zero(*quotient) && return_code == 0) {
    if (s21_div_by_10(quotient)) {
      break;
    }
    (*result_scale)--;
  }

  if (*result_scale > 28 && !s21_is_zero(*quotient)) {
    return_code = 1;
  }

  return return_code;
}
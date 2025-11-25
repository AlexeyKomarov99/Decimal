#include <stdio.h>

#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

static int s21_handle_simple_round_case(s21_decimal value, s21_decimal *result);
static int s21_perform_rounding(s21_decimal value, int scale, int sign,
                                s21_decimal *result);
static int s21_get_first_fraction_digit(s21_decimal value, int scale);
static int s21_remove_fractional_part(s21_decimal *value, int iterations);
static int s21_apply_rounding_correction(s21_decimal *integer_part,
                                         int first_fraction_digit);

int s21_round(s21_decimal value, s21_decimal *result) {
  int return_code = 1;

  if (result) {
    s21_init_decimal(result);
    return_code = s21_handle_simple_round_case(value, result);
  }

  return return_code;
}

static int s21_handle_simple_round_case(s21_decimal value,
                                        s21_decimal *result) {
  int return_code = 0;
  int scale = s21_get_scale(value);

  if (scale == 0 || s21_is_zero(value)) {
    *result = value;
  } else {
    int sign = s21_get_sign(value);
    return_code = s21_perform_rounding(value, scale, sign, result);
  }

  return return_code;
}

static int s21_perform_rounding(s21_decimal value, int scale, int sign,
                                s21_decimal *result) {
  int return_code = 0;
  s21_decimal temp = value;
  s21_set_sign(&temp, 0);

  int first_fraction_digit = s21_get_first_fraction_digit(temp, scale);
  s21_decimal integer_part = temp;

  s21_remove_fractional_part(&integer_part, scale);
  s21_apply_rounding_correction(&integer_part, first_fraction_digit);

  *result = integer_part;
  s21_set_sign(result, sign);
  s21_set_scale(result, 0);

  return return_code;
}

static int s21_get_first_fraction_digit(s21_decimal value, int scale) {
  s21_decimal temp_for_decision = value;
  int iterations = (scale > 1) ? scale - 1 : 0;

  s21_remove_fractional_part(&temp_for_decision, iterations);

  int first_digit = temp_for_decision.bits[0] % 10;
  return first_digit;
}

static int s21_remove_fractional_part(s21_decimal *value, int iterations) {
  int i = 0;

  while (i < iterations) {
    s21_div_by_10(value);
    i++;
  }

  return 0;
}

static int s21_apply_rounding_correction(s21_decimal *integer_part,
                                         int first_fraction_digit) {
  int return_code = 0;

  if (first_fraction_digit >= 5) {
    s21_decimal one = {{1, 0, 0, 0}};
    return_code = s21_add_bits(*integer_part, one, integer_part);
  }

  return return_code;
}
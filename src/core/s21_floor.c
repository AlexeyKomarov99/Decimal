#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

static int s21_has_fractional_part(s21_decimal original,
                                   s21_decimal integer_part, int scale);
static int s21_remove_fractional_part(s21_decimal *value, int scale);
static int s21_handle_negative_floor(s21_decimal value, int scale,
                                     s21_decimal *result);
static int s21_handle_positive_floor(s21_decimal value, int scale,
                                     s21_decimal *result);
static int s21_calculate_floor(s21_decimal value, s21_decimal *result);

int s21_floor(s21_decimal value, s21_decimal *result) {
  int return_code = 1;

  if (result) {
    s21_init_decimal(result);
    return_code = s21_calculate_floor(value, result);
  }

  return return_code;
}

static int s21_calculate_floor(s21_decimal value, s21_decimal *result) {
  int return_code = 0;
  int scale = s21_get_scale(value);

  if (scale == 0 || s21_is_zero(value)) {
    *result = value;
  } else {
    int sign = s21_get_sign(value);

    if (sign == 0) {
      return_code = s21_handle_positive_floor(value, scale, result);
    } else {
      return_code = s21_handle_negative_floor(value, scale, result);
    }
  }

  return return_code;
}

static int s21_handle_positive_floor(s21_decimal value, int scale,
                                     s21_decimal *result) {
  s21_decimal temp = value;
  s21_remove_fractional_part(&temp, scale);
  *result = temp;
  return 0;
}

static int s21_handle_negative_floor(s21_decimal value, int scale,
                                     s21_decimal *result) {
  s21_decimal temp = value;
  s21_set_sign(&temp, 0);

  s21_decimal integer_part = temp;
  s21_remove_fractional_part(&integer_part, scale);

  if (s21_has_fractional_part(temp, integer_part, scale)) {
    s21_decimal one = {{1, 0, 0, 0}};
    s21_add_bits(integer_part, one, &integer_part);
  }

  *result = integer_part;
  s21_set_sign(result, 1);
  s21_set_scale(result, 0);

  return 0;
}

static int s21_remove_fractional_part(s21_decimal *value, int scale) {
  int i = 0;

  while (i < scale) {
    s21_div_by_10(value);
    i++;
  }

  return 0;
}

static int s21_has_fractional_part(s21_decimal original,
                                   s21_decimal integer_part, int scale) {
  s21_decimal check = integer_part;
  int i = 0;
  int has_fraction = 0;

  while (i < scale) {
    s21_mul_by_10(&check);
    i++;
  }

  if (s21_compare_bits(original, check) != 0) {
    has_fraction = 1;
  }

  return has_fraction;
}
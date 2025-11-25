#include <limits.h>

#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

static int s21_convert_to_final_value(s21_decimal temp, int sign, int *dst);
static int s21_validate_int_range(s21_decimal temp, int sign);
static int s21_remove_fractional_digits(s21_decimal *value, int count);
static int s21_prepare_decimal_for_conversion(s21_decimal src,
                                              s21_decimal *temp);
static int s21_convert_non_zero_decimal(s21_decimal src, int *dst);
static int s21_handle_zero_decimal(s21_decimal src, int *dst);

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int return_code = 1;

  if (dst) {
    return_code = s21_handle_zero_decimal(src, dst);

    if (return_code != 0) {
      return_code = s21_convert_non_zero_decimal(src, dst);
    }
  }

  return return_code;
}

static int s21_handle_zero_decimal(s21_decimal src, int *dst) {
  int return_code = 1;

  if (s21_is_zero(src)) {
    *dst = 0;
    return_code = 0;
  }

  return return_code;
}

static int s21_convert_non_zero_decimal(s21_decimal src, int *dst) {
  int return_code = 1;
  s21_decimal temp = {0};
  int sign = s21_get_sign(src);

  s21_prepare_decimal_for_conversion(src, &temp);
  return_code = s21_validate_int_range(temp, sign);

  if (return_code == 0) {
    s21_convert_to_final_value(temp, sign, dst);
  }

  return return_code;
}

static int s21_prepare_decimal_for_conversion(s21_decimal src,
                                              s21_decimal *temp) {
  int scale = s21_get_scale(src);
  *temp = src;
  s21_set_sign(temp, 0);

  s21_remove_fractional_digits(temp, scale);

  return 0;
}

static int s21_remove_fractional_digits(s21_decimal *value, int count) {
  int i = 0;

  while (i < count) {
    s21_div_by_10(value);
    i++;
  }

  return 0;
}

static int s21_validate_int_range(s21_decimal temp, int sign) {
  int return_code = 0;

  if (temp.bits[1] != 0 || temp.bits[2] != 0) {
    return_code = 1;
  } else {
    int value = temp.bits[0];
    int max_positive = INT_MAX;

    if (sign == 1) {
      if (value < 0 || value > max_positive) {
        return_code = 1;
      }
    } else {
      if (value < 0 || value > max_positive) {
        return_code = 1;
      }
    }
  }

  return return_code;
}

static int s21_convert_to_final_value(s21_decimal temp, int sign, int *dst) {
  int value = temp.bits[0];

  if (sign == 1) {
    *dst = -value;
  } else {
    *dst = value;
  }

  return 0;
}
#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

static int s21_handle_simple_truncate_case(s21_decimal value,
                                           s21_decimal *result);
static int s21_perform_truncation(s21_decimal value, int scale, int sign,
                                  s21_decimal *result);
static int s21_remove_all_fractional_digits(s21_decimal *value, int scale);

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int return_code = 1;

  if (result) {
    s21_init_decimal(result);
    return_code = s21_handle_simple_truncate_case(value, result);
  }

  return return_code;
}

static int s21_handle_simple_truncate_case(s21_decimal value,
                                           s21_decimal *result) {
  int return_code = 0;
  int scale = s21_get_scale(value);

  if (scale == 0 || s21_is_zero(value)) {
    *result = value;
  } else {
    int sign = s21_get_sign(value);
    return_code = s21_perform_truncation(value, scale, sign, result);
  }

  return return_code;
}

static int s21_perform_truncation(s21_decimal value, int scale, int sign,
                                  s21_decimal *result) {
  int return_code = 0;
  s21_decimal temp = value;

  s21_set_sign(&temp, 0);
  s21_remove_all_fractional_digits(&temp, scale);

  *result = temp;
  s21_set_sign(result, sign);
  s21_set_scale(result, 0);

  return return_code;
}

static int s21_remove_all_fractional_digits(s21_decimal *value, int scale) {
  int i = 0;

  while (i < scale) {
    s21_div_by_10(value);
    i++;
  }

  return 0;
}
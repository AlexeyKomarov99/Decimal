#include <float.h>
#include <math.h>

#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

static int s21_validate_float_range(long double result, float *dst);
static int s21_apply_scale_and_sign(long double *result, int scale, int sign);
static long double s21_calculate_power_of_two(int exponent);
static int s21_calculate_decimal_value(s21_decimal src, long double *result);
static int s21_convert_non_zero_decimal(s21_decimal src, float *dst);
static int s21_handle_zero_decimal(s21_decimal src, float *dst);

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int return_code = 1;

  if (dst) {
    return_code = s21_handle_zero_decimal(src, dst);

    if (return_code != 0) {
      return_code = s21_convert_non_zero_decimal(src, dst);
    }
  }

  return return_code;
}

static int s21_handle_zero_decimal(s21_decimal src, float *dst) {
  int return_code = 1;

  if (s21_is_zero(src)) {
    *dst = 0.0f;
    return_code = 0;
  }

  return return_code;
}

static int s21_convert_non_zero_decimal(s21_decimal src, float *dst) {
  int return_code = 1;
  long double result = 0.0;
  int sign = s21_get_sign(src);
  int scale = s21_get_scale(src);

  s21_calculate_decimal_value(src, &result);
  s21_apply_scale_and_sign(&result, scale, sign);
  return_code = s21_validate_float_range(result, dst);

  return return_code;
}

static int s21_calculate_decimal_value(s21_decimal src, long double *result) {
  int i = 0;

  while (i < 3) {
    *result += (long double)src.bits[i] * s21_calculate_power_of_two(32 * i);
    i++;
  }

  return 0;
}

static long double s21_calculate_power_of_two(int exponent) {
  long double power = 1.0;
  int i = 0;

  while (i < exponent) {
    power *= 2.0;
    i++;
  }

  return power;
}

static int s21_apply_scale_and_sign(long double *result, int scale, int sign) {
  int i = 0;

  while (i < scale) {
    *result /= 10.0;
    i++;
  }

  if (sign) {
    *result = -*result;
  }

  return 0;
}

static int s21_validate_float_range(long double result, float *dst) {
  int return_code = 1;

  if (!isinf(result) && !isnan(result) && result <= (long double)FLT_MAX &&
      result >= (long double)-FLT_MAX) {
    *dst = (float)result;
    return_code = 0;
  }

  return return_code;
}
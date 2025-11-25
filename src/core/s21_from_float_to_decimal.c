#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

static int s21_validate_float_input(float src);
static int s21_process_positive_float(float src, s21_decimal *dst);
static int s21_parse_float_string(const char *buffer, long long *integer_part,
                                  int *scale);
static int s21_handle_exponent_part(const char *exp_ptr,
                                    long long *integer_part, int *scale);
static int s21_adjust_scale_and_check_overflow(long long *integer_part,
                                               int *scale);
static int s21_convert_to_decimal_format(long long integer_part, int scale,
                                         s21_decimal *dst);
static int s21_apply_float_sign(float src, s21_decimal *dst);
static long long s21_safe_string_to_long_long(const char *str);

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int return_code = 1;

  if (dst) {
    s21_init_decimal(dst);

    if (s21_validate_float_input(src) == 1) {
      return_code = 1;
    } else if (src == 0.0f) {
      return_code = 0;
    } else {
      return_code = s21_process_positive_float(src, dst);
    }
  }

  return return_code;
}

static int s21_validate_float_input(float src) {
  int return_code = 0;

  if (isnan(src) || isinf(src)) {
    return_code = 1;
  } else if (fabsf(src) > 0 && fabsf(src) < 1e-28f) {
    return_code = 1;
  } else if (fabsf(src) > 79228162514264337593543950335.0f) {
    return_code = 1;
  }

  return return_code;
}

static int s21_process_positive_float(float src, s21_decimal *dst) {
  int return_code = 1;
  char buffer[50];
  long long integer_part = 0;
  int scale = 0;

  s21_apply_float_sign(src, dst);
  float abs_src = fabsf(src);
  snprintf(buffer, sizeof(buffer), "%.7g", abs_src);

  s21_parse_float_string(buffer, &integer_part, &scale);
  return_code = s21_adjust_scale_and_check_overflow(&integer_part, &scale);

  if (return_code == 0) {
    s21_convert_to_decimal_format(integer_part, scale, dst);
  }

  return return_code;
}

static int s21_apply_float_sign(float src, s21_decimal *dst) {
  if (src < 0) {
    s21_set_sign(dst, 1);
  }
  return 0;
}

static int s21_parse_float_string(const char *buffer, long long *integer_part,
                                  int *scale) {
  char buffer_copy[50];
  strcpy(buffer_copy, buffer);

  char *dot = strchr(buffer_copy, '.');
  char *exp = strchr(buffer_copy, 'e');

  if (exp) {
    *exp = '\0';
  }

  if (dot) {
    *dot = '\0';
    const char *fraction = dot + 1;
    int fraction_len = strlen(fraction);

    *integer_part = s21_safe_string_to_long_long(buffer_copy);
    long long fraction_val = s21_safe_string_to_long_long(fraction);

    int i = 0;
    while (i < fraction_len) {
      *integer_part *= 10;
      i++;
    }

    *integer_part += fraction_val;
    *scale = fraction_len;
  } else {
    *integer_part = s21_safe_string_to_long_long(buffer_copy);
    *scale = 0;
  }

  if (exp) {
    s21_handle_exponent_part(exp + 1, integer_part, scale);
  }

  return 0;
}

static int s21_handle_exponent_part(const char *exp_ptr,
                                    long long *integer_part, int *scale) {
  int exponent = atoi(exp_ptr);

  if (exponent > 0) {
    int i = 0;
    while (i < exponent && *integer_part <= 7922816251426433759LL) {
      *integer_part *= 10;
      i++;
    }
    *scale = (*scale > exponent) ? *scale - exponent : 0;
  } else {
    *scale += -exponent;
  }

  return 0;
}

static int s21_adjust_scale_and_check_overflow(long long *integer_part,
                                               int *scale) {
  int return_code = 0;
  long long max_decimal = 7922816251426433759LL;

  while (*scale > 28 && *integer_part > 0) {
    *integer_part /= 10;
    (*scale)--;
  }

  if (*integer_part > max_decimal || *scale > 28) {
    return_code = 1;
  }

  return return_code;
}

static int s21_convert_to_decimal_format(long long integer_part, int scale,
                                         s21_decimal *dst) {
  dst->bits[0] = (int)(integer_part & 0xFFFFFFFF);
  dst->bits[1] = (int)((integer_part >> 32) & 0xFFFFFFFF);
  dst->bits[2] = 0;
  s21_set_scale(dst, scale);

  return 0;
}

static long long s21_safe_string_to_long_long(const char *str) {
  return atoll(str);
}
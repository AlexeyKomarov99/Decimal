#include <stdint.h>

#include "s21_auxiliary.h"

int s21_mul_by_10(s21_decimal *value) {
  int result = 1;

  if (value) {
    s21_decimal temp = *value;
    s21_shift_left(value);
    s21_shift_left(&temp);
    s21_shift_left(&temp);
    s21_shift_left(&temp);

    result = s21_add_bits(*value, temp, value);
  }

  return result;
}

int s21_div_by_10(s21_decimal *value) {
  int result = 1;

  if (value) {
    uint64_t remainder = 0;
    int i = 2;

    while (i >= 0) {
      uint64_t current = ((uint64_t)remainder << 32) | (uint32_t)value->bits[i];
      value->bits[i] = (int32_t)(current / 10);
      remainder = current % 10;
      i--;
    }

    result = 0;
  }

  return result;
}

int s21_check_overflow(s21_decimal value) {
  int overflow_detected = 0;

  if ((uint32_t)value.bits[2] > 0x19999999) {
    overflow_detected = 1;
  } else if ((uint32_t)value.bits[2] == 0x19999999) {
    uint32_t max_val = 0x99999999;

    if ((uint32_t)value.bits[1] > max_val) {
      overflow_detected = 1;
    } else if ((uint32_t)value.bits[1] == max_val) {
      if ((uint32_t)value.bits[0] > max_val) {
        overflow_detected = 1;
      }
    }
  }

  return overflow_detected;
}

void s21_align_for_division(s21_decimal *a, s21_decimal *b) {
  if (a && b) {
    int scale_a = s21_get_scale(*a);
    int scale_b = s21_get_scale(*b);
    s21_perform_alignment(a, b, &scale_a, &scale_b);
  }
}

int s21_perform_alignment(s21_decimal *a, s21_decimal *b, int *scale_a,
                          int *scale_b) {
  int alignment_complete = 0;

  while (!alignment_complete && (*scale_a > 0 || *scale_b > 0)) {
    int operation_performed =
        s21_try_alignment_operation(a, b, scale_a, scale_b);
    alignment_complete = !operation_performed;
  }

  return alignment_complete ? 1 : 0;
}

int s21_try_alignment_operation(s21_decimal *a, s21_decimal *b, int *scale_a,
                                int *scale_b) {
  int operation_performed = 0;

  if (*scale_a > 0) {
    operation_performed = s21_align_single_decimal(a, scale_a);
  }

  if (!operation_performed && *scale_b > 0) {
    operation_performed = s21_align_single_decimal(b, scale_b);
  }

  return operation_performed;
}

int s21_align_single_decimal(s21_decimal *value, int *scale) {
  int operation_performed = 0;

  if (s21_mul_by_10(value) == 0) {
    (*scale)--;
    s21_set_scale(value, *scale);
    operation_performed = 1;
  }

  return operation_performed;
}
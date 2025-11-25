#include "s21_auxiliary.h"

int s21_normalize_decimal_pair(s21_decimal *a, s21_decimal *b) {
  int result = 0;

  if (a && b) {
    int scale_a = s21_get_scale(*a);
    int scale_b = s21_get_scale(*b);
    const int max_scale = 28;
    int normalization_failed = 0;

    while (!normalization_failed && scale_a != scale_b &&
           scale_a <= max_scale && scale_b <= max_scale) {
      normalization_failed =
          s21_normalize_single_step(a, b, &scale_a, &scale_b);
    }

    result = normalization_failed ? 1 : 0;
  }

  return result;
}

int s21_normalize_single_step(s21_decimal *a, s21_decimal *b, int *scale_a,
                              int *scale_b) {
  int operation_failed = 0;

  if (*scale_a < *scale_b) {
    operation_failed = s21_multiply_decimal_by_ten(a, scale_a);
  } else {
    operation_failed = s21_multiply_decimal_by_ten(b, scale_b);
  }

  return operation_failed;
}

int s21_multiply_decimal_by_ten(s21_decimal *value, int *scale) {
  int result = 0;

  if (s21_mul_by_10(value) == 0) {
    (*scale)++;
    s21_set_scale(value, *scale);
  } else {
    result = 1;
  }

  return result;
}

void s21_normalize(s21_decimal *a, s21_decimal *b) {
  if (a && b) {
    s21_normalize_decimal_pair(a, b);
  }
}

int s21_process_bitwise_addition(s21_decimal a, s21_decimal b,
                                 s21_decimal *result,
                                 unsigned long long *carry) {
  int i = 0;

  while (i < 3) {
    unsigned long long sum =
        (unsigned long long)a.bits[i] + (unsigned long long)b.bits[i] + *carry;
    result->bits[i] = (unsigned int)sum;
    *carry = sum >> 32;
    i++;
  }

  return 0;
}

int s21_add_bits(s21_decimal a, s21_decimal b, s21_decimal *result) {
  int return_code = 1;

  if (result) {
    unsigned long long carry = 0;
    s21_process_bitwise_addition(a, b, result, &carry);
    return_code = carry > 0 ? 1 : 0;
  }

  return return_code;
}

int s21_process_bitwise_subtraction(s21_decimal a, s21_decimal b,
                                    s21_decimal *result,
                                    unsigned long long *borrow) {
  int i = 0;

  while (i < 3) {
    long long diff = (long long)a.bits[i] - (long long)b.bits[i] - *borrow;

    if (diff < 0) {
      diff += (1LL << 32);
      *borrow = 1;
    } else {
      *borrow = 0;
    }

    result->bits[i] = (unsigned int)diff;
    i++;
  }

  return 0;
}

int s21_sub_bits(s21_decimal a, s21_decimal b, s21_decimal *result) {
  int return_code = 1;

  if (result) {
    unsigned long long borrow = 0;
    s21_process_bitwise_subtraction(a, b, result, &borrow);
    return_code = (int)borrow;
  }

  return return_code;
}
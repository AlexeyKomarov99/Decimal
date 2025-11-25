#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

static int s21_perform_subtraction(s21_decimal value_1, s21_decimal value_2,
                                   s21_decimal *result, int sign1);
static int s21_handle_same_signs(s21_decimal value_1, s21_decimal value_2,
                                 s21_decimal *result);
static int s21_handle_different_signs(s21_decimal value_1, s21_decimal value_2,
                                      s21_decimal *result, int *processed);
static int s21_handle_zero_cases(s21_decimal value_1, s21_decimal value_2,
                                 s21_decimal *result, int *processed);
static int s21_execute_subtraction(s21_decimal value_1, s21_decimal value_2,
                                   s21_decimal *result);

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int return_code = 1;

  if (result) {
    s21_init_decimal(result);
    return_code = s21_execute_subtraction(value_1, value_2, result);
  }

  return return_code;
}

static int s21_execute_subtraction(s21_decimal value_1, s21_decimal value_2,
                                   s21_decimal *result) {
  int return_code = 0;
  int case_processed = 0;

  return_code =
      s21_handle_zero_cases(value_1, value_2, result, &case_processed);

  if (!case_processed) {
    return_code =
        s21_handle_different_signs(value_1, value_2, result, &case_processed);
  }

  if (!case_processed) {
    return_code = s21_handle_same_signs(value_1, value_2, result);
  }

  return return_code;
}

static int s21_handle_zero_cases(s21_decimal value_1, s21_decimal value_2,
                                 s21_decimal *result, int *processed) {
  int return_code = 0;
  *processed = 0;

  if (s21_is_zero(value_2)) {
    *result = value_1;
    *processed = 1;
  } else if (s21_is_zero(value_1)) {
    *result = value_2;
    s21_set_sign(result, !s21_get_sign(value_2));
    *processed = 1;
  }

  return return_code;
}

static int s21_handle_different_signs(s21_decimal value_1, s21_decimal value_2,
                                      s21_decimal *result, int *processed) {
  int return_code = 0;
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);
  *processed = 0;

  if (sign1 != sign2) {
    s21_set_sign(&value_2, !sign2);
    return_code = s21_add(value_1, value_2, result);
    *processed = 1;
  }

  return return_code;
}

static int s21_handle_same_signs(s21_decimal value_1, s21_decimal value_2,
                                 s21_decimal *result) {
  int return_code = 0;
  int sign1 = s21_get_sign(value_1);

  return_code = s21_perform_subtraction(value_1, value_2, result, sign1);

  return return_code;
}

static int s21_perform_subtraction(s21_decimal value_1, s21_decimal value_2,
                                   s21_decimal *result, int sign1) {
  int return_code = 0;

  s21_normalize(&value_1, &value_2);
  int compare = s21_compare_bits(value_1, value_2);
  int final_sign = sign1;

  if (compare >= 0) {
    s21_sub_bits(value_1, value_2, result);
  } else {
    s21_sub_bits(value_2, value_1, result);
    final_sign = !final_sign;
  }

  s21_set_scale(result, s21_get_scale(value_1));
  s21_set_sign(result, final_sign);

  return return_code;
}
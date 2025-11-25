#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

static int s21_process_bit_and_add(s21_decimal value_1, int bit_position,
                                   s21_decimal *acc, int result_sign);
static int s21_perform_bit_multiplication(s21_decimal value_1,
                                          s21_decimal value_2, s21_decimal *acc,
                                          int result_sign);
static int s21_adjust_multiplication_result(s21_decimal *acc, int *result_scale,
                                            int result_sign,
                                            s21_decimal *result);
static int s21_shift_with_overflow_check(s21_decimal *value, int shifts,
                                         int result_sign);
static int s21_process_single_bit(s21_decimal value_1, int bit_position,
                                  s21_decimal *temp, int result_sign);
static int s21_prepare_multiplication_values(s21_decimal *value_1,
                                             s21_decimal *value_2,
                                             int *result_sign,
                                             int *result_scale);
static int s21_execute_multiplication(s21_decimal value_1, s21_decimal value_2,
                                      s21_decimal *result);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int return_code = 1;

  if (result) {
    s21_init_decimal(result);

    if (s21_is_zero(value_1) || s21_is_zero(value_2)) {
      return_code = 0;
    } else {
      return_code = s21_execute_multiplication(value_1, value_2, result);
    }
  }

  return return_code;
}

static int s21_execute_multiplication(s21_decimal value_1, s21_decimal value_2,
                                      s21_decimal *result) {
  int return_code = 0;
  int result_sign = 0;
  int result_scale = 0;
  s21_decimal acc = {0};

  s21_prepare_multiplication_values(&value_1, &value_2, &result_sign,
                                    &result_scale);
  return_code =
      s21_perform_bit_multiplication(value_1, value_2, &acc, result_sign);

  if (return_code == 0) {
    return_code = s21_adjust_multiplication_result(&acc, &result_scale,
                                                   result_sign, result);
  }

  return return_code;
}

static int s21_prepare_multiplication_values(s21_decimal *value_1,
                                             s21_decimal *value_2,
                                             int *result_sign,
                                             int *result_scale) {
  int return_code = 0;

  int sign1 = s21_get_sign(*value_1);
  int sign2 = s21_get_sign(*value_2);
  *result_sign = sign1 ^ sign2;

  if (s21_check_overflow(*value_1) || s21_check_overflow(*value_2)) {
    return_code = *result_sign ? 2 : 1;
  } else {
    s21_set_sign(value_1, 0);
    s21_set_sign(value_2, 0);

    int scale1 = s21_get_scale(*value_1);
    int scale2 = s21_get_scale(*value_2);
    *result_scale = scale1 + scale2;
  }

  return return_code;
}

static int s21_perform_bit_multiplication(s21_decimal value_1,
                                          s21_decimal value_2, s21_decimal *acc,
                                          int result_sign) {
  int return_code = 0;
  int i = 0;
  int multiplication_complete = 0;

  while (i < 96 && !multiplication_complete && return_code == 0) {
    int bit_set = s21_get_bit(value_2, i);

    if (bit_set) {
      return_code = s21_process_bit_and_add(value_1, i, acc, result_sign);
    }

    i++;
    multiplication_complete = (return_code != 0);
  }

  return return_code;
}

static int s21_process_bit_and_add(s21_decimal value_1, int bit_position,
                                   s21_decimal *acc, int result_sign) {
  int return_code = 0;
  s21_decimal temp = {0};

  return_code =
      s21_process_single_bit(value_1, bit_position, &temp, result_sign);

  if (return_code == 0) {
    int addition_overflow = s21_add_bits(*acc, temp, acc);

    if (addition_overflow) {
      return_code = result_sign ? 2 : 1;
    }
  }

  return return_code;
}

static int s21_process_single_bit(s21_decimal value_1, int bit_position,
                                  s21_decimal *temp, int result_sign) {
  int return_code = 0;
  *temp = value_1;

  return_code = s21_shift_with_overflow_check(temp, bit_position, result_sign);

  return return_code;
}

static int s21_shift_with_overflow_check(s21_decimal *value, int shifts,
                                         int result_sign) {
  int return_code = 0;
  int j = 0;

  while (j < shifts && return_code == 0) {
    s21_decimal temp_shift = *value;
    s21_shift_left(&temp_shift);

    if (temp_shift.bits[2] < value->bits[2]) {
      return_code = result_sign ? 2 : 1;
    } else {
      *value = temp_shift;
    }
    j++;
  }

  return return_code;
}

static int s21_adjust_multiplication_result(s21_decimal *acc, int *result_scale,
                                            int result_sign,
                                            s21_decimal *result) {
  int return_code = 0;

  while (*result_scale > 28 && !s21_is_zero(*acc) && return_code == 0) {
    if (s21_div_by_10(acc)) {
      break;
    }
    (*result_scale)--;
  }

  if (*result_scale > 28 && !s21_is_zero(*acc)) {
    return_code = result_sign ? 2 : 1;
  }

  if (return_code == 0) {
    *result = *acc;
    s21_set_sign(result, result_sign);
    s21_set_scale(result, *result_scale);
  }

  return return_code;
}
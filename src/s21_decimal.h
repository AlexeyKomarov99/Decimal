#ifndef S21_DECIMAL
#define S21_DECIMAL

#include <stdint.h>

typedef struct {
  int32_t bits[4];
} s21_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

int s21_get_sign(s21_decimal value);
void s21_set_sign(s21_decimal *value, int sign);
int s21_get_scale(s21_decimal value);
void s21_set_scale(s21_decimal *value, int scale);
int s21_is_zero(s21_decimal value);

int s21_perform_addition(s21_decimal value_1, s21_decimal value_2,
                         s21_decimal *result);
int s21_add_non_zero_values(s21_decimal value_1, s21_decimal value_2,
                            s21_decimal *result);
int s21_add_same_sign(s21_decimal value_1, s21_decimal value_2,
                      s21_decimal *result, int sign, int scale);
int s21_handle_overflow(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal *result, int sign, int scale);
int s21_reduce_scale_and_retry(s21_decimal *value_1, s21_decimal *value_2,
                               s21_decimal *result, int scale);
int s21_add_different_signs(s21_decimal value_1, s21_decimal value_2,
                            s21_decimal *result, int sign1, int sign2,
                            int scale);

#endif
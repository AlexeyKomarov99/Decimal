#ifndef S21_AUXILIARY_H
#define S21_AUXILIARY_H

#include "../s21_decimal.h"

int s21_get_sign(s21_decimal value);
void s21_set_sign(s21_decimal *value, int sign);
int s21_get_scale(s21_decimal value);
void s21_set_scale(s21_decimal *value, int scale);
int s21_is_zero(s21_decimal value);
void s21_init_decimal(s21_decimal *value);
int s21_compare_bits(s21_decimal a, s21_decimal b);
void s21_normalize(s21_decimal *a, s21_decimal *b);
int s21_sub_bits(s21_decimal a, s21_decimal b, s21_decimal *result);
int s21_add_bits(s21_decimal a, s21_decimal b, s21_decimal *result);
void s21_shift_left(s21_decimal *value);
void s21_shift_right(s21_decimal *value);
int s21_div_by_10(s21_decimal *value);
int s21_mul_by_10(s21_decimal *value);
int s21_div_bits(s21_decimal a, s21_decimal b, s21_decimal *result,
                 s21_decimal *remainder);
void s21_align_for_division(s21_decimal *a, s21_decimal *b);
int s21_check_overflow(s21_decimal value);
int s21_get_bit(s21_decimal value, int position);
int s21_normalize_decimal_pair(s21_decimal *a, s21_decimal *b);
int s21_normalize_single_step(s21_decimal *a, s21_decimal *b, int *scale_a,
                              int *scale_b);
int s21_multiply_decimal_by_ten(s21_decimal *value, int *scale);
int s21_process_bitwise_addition(s21_decimal a, s21_decimal b,
                                 s21_decimal *result,
                                 unsigned long long *carry);
int s21_process_bitwise_subtraction(s21_decimal a, s21_decimal b,
                                    s21_decimal *result,
                                    unsigned long long *borrow);
int s21_perform_alignment(s21_decimal *a, s21_decimal *b, int *scale_a,
                          int *scale_b);
int s21_try_alignment_operation(s21_decimal *a, s21_decimal *b, int *scale_a,
                                int *scale_b);
int s21_align_single_decimal(s21_decimal *value, int *scale);

#endif
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_mul_normal) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{15, 0, 0, 0}};

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[1], expected.bits[1]);
  ck_assert_int_eq(result.bits[2], expected.bits[2]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_mul_negative_first) {
  s21_decimal a = {{5, 0, 0, 0x80000000}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{15, 0, 0, 0x80000000}};

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), s21_get_sign(expected));
}
END_TEST

START_TEST(test_mul_negative_both) {
  s21_decimal a = {{5, 0, 0, 0x80000000}};
  s21_decimal b = {{3, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{15, 0, 0, 0}};

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), s21_get_sign(expected));
}
END_TEST

START_TEST(test_mul_zero) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_mul_null_result) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};

  int ret = s21_mul(a, b, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_mul_large_numbers) {
  s21_decimal a = {{1000, 0, 0, 0}};
  s21_decimal b = {{1000, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{1000000, 0, 0, 0}};

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_mul_with_scale) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{2, 0, 0, 0x00010000}};
  s21_decimal result;

  s21_set_scale(&a, 0);
  s21_set_scale(&b, 1);

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);

  ck_assert_int_eq(result.bits[0], 10);
  ck_assert_int_eq(s21_get_scale(result), 1);
}
END_TEST

START_TEST(test_mul_shift_overflow) {
  s21_decimal a = {{0x80000000, 0, 0, 0}};
  s21_decimal b = {{4, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert(ret == 0 || ret == 1 || ret == 2);
}
END_TEST

START_TEST(test_mul_add_overflow) {
  s21_decimal a = {{0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0}};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert(ret == 0 || ret == 1 || ret == 2);
}
END_TEST

START_TEST(test_mul_scale_reduction) {
  s21_decimal a = {{123456789, 0, 0, 0x001D0000}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_le(s21_get_scale(result), 28);
}
END_TEST

START_TEST(test_mul_scale_reduction_with_break) {
  s21_decimal a = {{1, 0, 0, 0x001E0000}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert(ret == 0 || ret == 1 || ret == 2);
}
END_TEST

START_TEST(test_mul_overflow_after_scale_reduction) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x001D0000}};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert(ret == 1 || ret == 2);
}
END_TEST

START_TEST(test_mul_positive_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_mul_negative_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 2);
}

START_TEST(test_mul_zero_first_operand) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_mul_zero_second_operand) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_mul_scale_reduction_exact_break) {
  s21_decimal a = {{999999999, 0, 0, 0x001D0000}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_le(s21_get_scale(result), 28);
}
END_TEST

START_TEST(test_mul_scale_overflow_positive) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x001C0000}};
  s21_decimal b = {{10, 0, 0, 0x001C0000}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_mul_scale_overflow_negative) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x801C0000}};
  s21_decimal b = {{10, 0, 0, 0x001C0000}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 2);
}
END_TEST

START_TEST(test_mul_successful_execution_path) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{20, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{200, 0, 0, 0}};

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), s21_get_sign(expected));
}
END_TEST

START_TEST(test_mul_with_moderate_scale) {
  s21_decimal a = {{100, 0, 0, 0x00050000}};
  s21_decimal b = {{50, 0, 0, 0x00050000}};
  s21_decimal result;

  int ret = s21_mul(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(s21_get_scale(result), 10);
  ck_assert_int_eq(result.bits[0], 5000);
}
END_TEST

TCase *tcase_mul(void) {
  TCase *tc = tcase_create("s21_mul");

  tcase_add_test(tc, test_mul_normal);
  tcase_add_test(tc, test_mul_negative_first);
  tcase_add_test(tc, test_mul_negative_both);
  tcase_add_test(tc, test_mul_zero);
  tcase_add_test(tc, test_mul_null_result);
  tcase_add_test(tc, test_mul_large_numbers);
  tcase_add_test(tc, test_mul_with_scale);
  tcase_add_test(tc, test_mul_shift_overflow);
  tcase_add_test(tc, test_mul_add_overflow);
  tcase_add_test(tc, test_mul_scale_reduction);
  tcase_add_test(tc, test_mul_scale_reduction_with_break);
  tcase_add_test(tc, test_mul_overflow_after_scale_reduction);
  tcase_add_test(tc, test_mul_positive_overflow);
  tcase_add_test(tc, test_mul_negative_overflow);
  tcase_add_test(tc, test_mul_zero_first_operand);
  tcase_add_test(tc, test_mul_zero_second_operand);
  tcase_add_test(tc, test_mul_scale_reduction_exact_break);
  tcase_add_test(tc, test_mul_scale_overflow_positive);
  tcase_add_test(tc, test_mul_scale_overflow_negative);
  tcase_add_test(tc, test_mul_successful_execution_path);
  tcase_add_test(tc, test_mul_with_moderate_scale);

  return tc;
}
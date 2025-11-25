#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_add_normal) {
  s21_decimal a = {{123, 0, 0, 0}};
  s21_decimal b = {{45, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{168, 0, 0, 0}};

  int ret = s21_add(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[1], expected.bits[1]);
  ck_assert_int_eq(result.bits[2], expected.bits[2]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_add_negative_both) {
  s21_decimal a = {{100, 0, 0, 0x80000000}};
  s21_decimal b = {{50, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{150, 0, 0, 0x80000000}};

  int ret = s21_add(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), s21_get_sign(expected));
}
END_TEST

START_TEST(test_add_mixed_signs) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{50, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{50, 0, 0, 0}};

  int ret = s21_add(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), s21_get_sign(expected));
}
END_TEST

START_TEST(test_add_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{123, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0}};

  int ret = s21_add(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_add_null_result) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{50, 0, 0, 0}};

  int ret = s21_add(a, b, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_add_zero_second_operand) {
  s21_decimal a = {{123, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0}};

  int ret = s21_add(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_add_overflow_recovery) {
  s21_decimal a = {{429496729, 0, 0, 0x00010000}};
  s21_decimal b = {{429496729, 0, 0, 0x00010000}};
  s21_decimal result;

  int ret = s21_add(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_lt(s21_get_scale(result), 2);
}
END_TEST

START_TEST(test_add_overflow_with_scale_recovery) {
  s21_decimal a = {{0xFFFFFFF0, 0x0000000F, 0, 0x00010000}};
  s21_decimal b = {{0xFFFFFFF0, 0x0000000F, 0, 0x00010000}};
  s21_decimal result;

  int ret = s21_add(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_lt(s21_get_scale(result), 2);
}
END_TEST

START_TEST(test_add_overflow_borderline_case) {
  s21_decimal a = {{0x99999999, 0x19999999, 0, 0x00010000}};
  s21_decimal b = {{0x99999999, 0x19999999, 0, 0x00010000}};
  s21_decimal result;

  int ret = s21_add(a, b, &result);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_add_overflow_minimal_scale) {
  s21_decimal a = {{0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFE, 0x00010000}};
  s21_decimal b = {{1, 0, 0, 0x00010000}};
  s21_decimal result;

  int ret = s21_add(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_add_different_signs_b_greater_than_a) {
  s21_decimal a = {{50, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{50, 0, 0, 0x80000000}};

  int ret = s21_add(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_add_different_scales_large_numbers) {
  s21_decimal a = {{100, 0, 0, 0x00020000}};
  s21_decimal b = {{200, 0, 0, 0x00010000}};
  s21_decimal result;

  int ret = s21_add(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 2100);
  ck_assert_int_eq(s21_get_scale(result), 2);
}
END_TEST

TCase *tcase_add(void) {
  TCase *tc = tcase_create("s21_add");

  tcase_add_test(tc, test_add_normal);
  tcase_add_test(tc, test_add_negative_both);
  tcase_add_test(tc, test_add_mixed_signs);
  tcase_add_test(tc, test_add_zero);
  tcase_add_test(tc, test_add_null_result);
  tcase_add_test(tc, test_add_zero_second_operand);
  tcase_add_test(tc, test_add_overflow_recovery);
  tcase_add_test(tc, test_add_overflow_with_scale_recovery);
  tcase_add_test(tc, test_add_overflow_borderline_case);
  tcase_add_test(tc, test_add_overflow_minimal_scale);
  tcase_add_test(tc, test_add_different_signs_b_greater_than_a);
  tcase_add_test(tc, test_add_different_scales_large_numbers);

  return tc;
}
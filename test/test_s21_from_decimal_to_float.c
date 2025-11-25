#include <check.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_decimal_to_float_positive) {
  s21_decimal src = {{12345, 0, 0, 0}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq_tol(result, 12345.0f, 1e-6);
}
END_TEST

START_TEST(test_decimal_to_float_negative) {
  s21_decimal src = {{6789, 0, 0, 0x80000000}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq_tol(result, -6789.0f, 1e-6);
}
END_TEST

START_TEST(test_decimal_to_float_zero) {
  s21_decimal src = {{0, 0, 0, 0}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq_tol(result, 0.0f, 1e-6);
}
END_TEST

START_TEST(test_decimal_to_float_with_scale) {
  s21_decimal src = {{12345, 0, 0, 0x00020000}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq_tol(result, 123.45f, 1e-6);
}
END_TEST

START_TEST(test_decimal_to_float_small_fraction) {
  s21_decimal src = {{1, 0, 0, 0x00060000}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq_tol(result, 0.000001f, 1e-8);
}
END_TEST

START_TEST(test_decimal_to_float_large_number) {
  s21_decimal src = {{1234567, 0, 0, 0}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq_tol(result, 1234567.0f, 1e-6);
}
END_TEST

START_TEST(test_decimal_to_float_negative_with_scale) {
  s21_decimal src = {{12345, 0, 0, 0x80010000}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq_tol(result, -1234.5f, 1e-6);
}
END_TEST

START_TEST(test_decimal_to_float_null_dst) {
  s21_decimal src = {{123, 0, 0, 0}};

  int ret = s21_from_decimal_to_float(src, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_float_precision) {
  s21_decimal src = {{1234567, 0, 0, 0x00060000}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq_tol(result, 1.234567f, 1e-6);
}
END_TEST

START_TEST(test_decimal_to_float_large_scale) {
  s21_decimal src = {{123456789, 0, 0, 0x00140000}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq_tol(result, 1.23456789e-12f, 1e-20);
}
END_TEST

START_TEST(test_decimal_to_float_complex_calculation) {
  s21_decimal src = {{0x12345678, 0xABCD, 0x42, 0x00030000}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_decimal_to_float_boundary_value) {
  s21_decimal src = {{0, 0, 0x1FFFFF, 0}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert(result > 0 && result < FLT_MAX);
}
END_TEST

START_TEST(test_decimal_to_float_infinity_case) {
  s21_decimal src = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}};
  float result;

  int ret = s21_from_decimal_to_float(src, &result);
  ck_assert(ret == 0 || ret == 1);
}
END_TEST

TCase *tcase_from_decimal_to_float(void) {
  TCase *tc = tcase_create("s21_from_decimal_to_float");
  tcase_add_test(tc, test_decimal_to_float_positive);
  tcase_add_test(tc, test_decimal_to_float_negative);
  tcase_add_test(tc, test_decimal_to_float_zero);
  tcase_add_test(tc, test_decimal_to_float_with_scale);
  tcase_add_test(tc, test_decimal_to_float_small_fraction);
  tcase_add_test(tc, test_decimal_to_float_large_number);
  tcase_add_test(tc, test_decimal_to_float_negative_with_scale);
  tcase_add_test(tc, test_decimal_to_float_null_dst);
  tcase_add_test(tc, test_decimal_to_float_precision);
  tcase_add_test(tc, test_decimal_to_float_large_scale);
  tcase_add_test(tc, test_decimal_to_float_complex_calculation);
  tcase_add_test(tc, test_decimal_to_float_boundary_value);
  tcase_add_test(tc, test_decimal_to_float_infinity_case);
  return tc;
}
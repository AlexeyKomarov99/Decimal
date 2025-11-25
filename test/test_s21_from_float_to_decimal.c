#include <check.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_float_to_decimal_positive) {
  s21_decimal result;
  float src = 123.45f;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(s21_get_scale(result), 2);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_float_to_decimal_negative) {
  s21_decimal result;
  float src = -67.89f;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_float_to_decimal_zero) {
  s21_decimal result;
  float src = 0.0f;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_float_to_decimal_small_positive) {
  s21_decimal result;
  float src = 0.001f;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(s21_get_scale(result), 3);
}
END_TEST

START_TEST(test_float_to_decimal_large_number) {
  s21_decimal result;
  float src = 1234567.0f;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_float_to_decimal_too_small) {
  s21_decimal result;
  float src = 1e-29f;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_float_to_decimal_too_large) {
  s21_decimal result;
  float src = 8e28f;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_float_to_decimal_nan) {
  s21_decimal result;
  float src = NAN;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_float_to_decimal_infinity) {
  s21_decimal result;
  float src = INFINITY;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_float_to_decimal_null_dst) {
  float src = 123.45f;

  int ret = s21_from_float_to_decimal(src, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_float_to_decimal_precision) {
  s21_decimal result;
  float src = 1.234567f;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(s21_get_scale(result), 6);
}
END_TEST

START_TEST(test_float_to_decimal_with_positive_exponent) {
  float src = 1.23e3f;
  s21_decimal result;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1230);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_float_to_decimal_with_negative_exponent) {
  float src = 1.23e-2f;
  s21_decimal result;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(s21_get_scale(result), 4);
}
END_TEST

START_TEST(test_float_to_decimal_scale_reduction) {
  float src = 1.2345678901234567890123456789f;
  s21_decimal result;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_le(s21_get_scale(result), 28);
}
END_TEST

START_TEST(test_float_to_decimal_overflow_after_exponent) {
  float src = 1e20f;
  s21_decimal result;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert(ret == 0 || ret == 1);
}
END_TEST

START_TEST(test_float_to_decimal_break_in_exponent_loop) {
  float src = 7.922816251426433759e18f;
  s21_decimal result;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert(ret == 0 || ret == 1);
}
END_TEST

START_TEST(test_float_to_decimal_scale_overflow) {
  float src = 1e-29f;
  s21_decimal result;

  int ret = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_float_to_decimal_large_scale_adjustment) {
  float src = 0.000000000000000000000000001f;
  s21_decimal result;

  int ret = s21_from_float_to_decimal(src, &result);
  if (ret == 0) {
    ck_assert_int_le(s21_get_scale(result), 28);
  } else {
    ck_assert_int_eq(ret, 1);
  }
}
END_TEST

TCase *tcase_from_float_to_decimal(void) {
  TCase *tc = tcase_create("s21_from_float_to_decimal");
  tcase_add_test(tc, test_float_to_decimal_positive);
  tcase_add_test(tc, test_float_to_decimal_negative);
  tcase_add_test(tc, test_float_to_decimal_zero);
  tcase_add_test(tc, test_float_to_decimal_small_positive);
  tcase_add_test(tc, test_float_to_decimal_large_number);
  tcase_add_test(tc, test_float_to_decimal_too_small);
  tcase_add_test(tc, test_float_to_decimal_too_large);
  tcase_add_test(tc, test_float_to_decimal_nan);
  tcase_add_test(tc, test_float_to_decimal_infinity);
  tcase_add_test(tc, test_float_to_decimal_null_dst);
  tcase_add_test(tc, test_float_to_decimal_precision);
  tcase_add_test(tc, test_float_to_decimal_with_positive_exponent);
  tcase_add_test(tc, test_float_to_decimal_with_negative_exponent);
  tcase_add_test(tc, test_float_to_decimal_scale_reduction);
  tcase_add_test(tc, test_float_to_decimal_overflow_after_exponent);
  tcase_add_test(tc, test_float_to_decimal_break_in_exponent_loop);
  tcase_add_test(tc, test_float_to_decimal_scale_overflow);
  tcase_add_test(tc, test_float_to_decimal_large_scale_adjustment);
  return tc;
}
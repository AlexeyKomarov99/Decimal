#include <check.h>
#include <limits.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_int_to_decimal_positive) {
  s21_decimal result;
  int src = 12345;

  int ret = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(s21_get_sign(result), 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_int_to_decimal_negative) {
  s21_decimal result;
  int src = -6789;

  int ret = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 6789);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_int_to_decimal_zero) {
  s21_decimal result;
  int src = 0;

  int ret = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_int_to_decimal_max_int) {
  s21_decimal result;
  int src = INT_MAX;

  int ret = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], INT_MAX);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_int_to_decimal_min_int) {
  s21_decimal result;
  int src = INT_MIN;

  int ret = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(ret, 0);

  ck_assert_int_eq(s21_get_sign(result), 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_int_to_decimal_null_dst) {
  int src = 123;

  int ret = s21_from_int_to_decimal(src, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

TCase *tcase_from_int_to_decimal(void) {
  TCase *tc = tcase_create("s21_from_int_to_decimal");
  tcase_add_test(tc, test_int_to_decimal_positive);
  tcase_add_test(tc, test_int_to_decimal_negative);
  tcase_add_test(tc, test_int_to_decimal_zero);
  tcase_add_test(tc, test_int_to_decimal_max_int);
  tcase_add_test(tc, test_int_to_decimal_min_int);
  tcase_add_test(tc, test_int_to_decimal_null_dst);
  return tc;
}
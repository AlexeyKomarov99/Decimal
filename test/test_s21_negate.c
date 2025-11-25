#include <check.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_negate_positive) {
  s21_decimal value = {{123, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0x80000000}};

  int ret = s21_negate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_negate_negative) {
  s21_decimal value = {{456, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{456, 0, 0, 0}};

  int ret = s21_negate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_negate_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_negate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_is_zero(result), 1);
}
END_TEST

START_TEST(test_negate_negative_zero) {
  s21_decimal value = {{0, 0, 0, 0x80000000}};
  s21_decimal result;

  int ret = s21_negate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_is_zero(result), 1);
}
END_TEST

START_TEST(test_negate_with_scale) {
  s21_decimal value = {{12345, 0, 0, 0x00020000}};
  s21_decimal result;

  int ret = s21_negate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(s21_get_scale(result), 2);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_negate_large_number) {
  s21_decimal value = {{1234567, 89, 12, 0}};
  s21_decimal result;

  int ret = s21_negate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(result.bits[1], 89);
  ck_assert_int_eq(result.bits[2], 12);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_negate_null_result) {
  s21_decimal value = {{123, 0, 0, 0}};

  int ret = s21_negate(value, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

TCase *tcase_negate(void) {
  TCase *tc = tcase_create("s21_negate");
  tcase_add_test(tc, test_negate_positive);
  tcase_add_test(tc, test_negate_negative);
  tcase_add_test(tc, test_negate_zero);
  tcase_add_test(tc, test_negate_negative_zero);
  tcase_add_test(tc, test_negate_with_scale);
  tcase_add_test(tc, test_negate_large_number);
  tcase_add_test(tc, test_negate_null_result);
  return tc;
}
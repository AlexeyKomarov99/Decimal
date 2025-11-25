#include <check.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_round_positive_no_rounding) {
  s21_decimal value = {{1234, 0, 0, 0x00020000}};
  s21_decimal result;
  s21_decimal expected = {{12, 0, 0, 0}};

  int ret = s21_round(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_round_positive_with_rounding_up) {
  s21_decimal value = {{1235, 0, 0, 0x00020000}};
  s21_decimal result;
  s21_decimal expected = {{12, 0, 0, 0}};

  int ret = s21_round(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_round_positive_with_rounding_down) {
  s21_decimal value = {{1236, 0, 0, 0x00020000}};
  s21_decimal result;
  s21_decimal expected = {{12, 0, 0, 0}};

  int ret = s21_round(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_round_negative_no_rounding) {
  s21_decimal value = {{1234, 0, 0, 0x80020000}};
  s21_decimal result;
  s21_decimal expected = {{12, 0, 0, 0x80000000}};

  int ret = s21_round(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_round_negative_with_rounding) {
  s21_decimal value = {{1236, 0, 0, 0x80020000}};
  s21_decimal result;
  s21_decimal expected = {{12, 0, 0, 0x80000000}};

  int ret = s21_round(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_round_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_round(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_round_half_up) {
  s21_decimal value = {{125, 0, 0, 0x00010000}};
  s21_decimal result;
  s21_decimal expected = {{13, 0, 0, 0}};

  int ret = s21_round(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_round_null_result) {
  s21_decimal value = {{123, 0, 0, 0}};

  int ret = s21_round(value, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

TCase *tcase_round(void) {
  TCase *tc = tcase_create("s21_round");
  tcase_add_test(tc, test_round_positive_no_rounding);
  tcase_add_test(tc, test_round_positive_with_rounding_up);
  tcase_add_test(tc, test_round_positive_with_rounding_down);
  tcase_add_test(tc, test_round_negative_no_rounding);
  tcase_add_test(tc, test_round_negative_with_rounding);
  tcase_add_test(tc, test_round_zero);
  tcase_add_test(tc, test_round_half_up);
  tcase_add_test(tc, test_round_null_result);
  return tc;
}
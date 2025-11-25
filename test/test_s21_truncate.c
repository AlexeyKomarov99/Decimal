#include <check.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_truncate_positive_no_fraction) {
  s21_decimal value = {{123, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0}};

  int ret = s21_truncate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_truncate_positive_with_fraction) {
  s21_decimal value = {{12345, 0, 0, 0x00020000}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0}};

  int ret = s21_truncate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_truncate_negative_no_fraction) {
  s21_decimal value = {{123, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0x80000000}};

  int ret = s21_truncate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_truncate_negative_with_fraction) {
  s21_decimal value = {{12345, 0, 0, 0x80020000}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0x80000000}};

  int ret = s21_truncate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_truncate_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_truncate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_truncate_large_fraction) {
  s21_decimal value = {{999999, 0, 0, 0x00060000}};
  s21_decimal result;
  s21_decimal expected = {{0, 0, 0, 0}};

  int ret = s21_truncate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_truncate_null_result) {
  s21_decimal value = {{123, 0, 0, 0}};

  int ret = s21_truncate(value, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

TCase *tcase_truncate(void) {
  TCase *tc = tcase_create("s21_truncate");
  tcase_add_test(tc, test_truncate_positive_no_fraction);
  tcase_add_test(tc, test_truncate_positive_with_fraction);
  tcase_add_test(tc, test_truncate_negative_no_fraction);
  tcase_add_test(tc, test_truncate_negative_with_fraction);
  tcase_add_test(tc, test_truncate_zero);
  tcase_add_test(tc, test_truncate_large_fraction);
  tcase_add_test(tc, test_truncate_null_result);
  return tc;
}
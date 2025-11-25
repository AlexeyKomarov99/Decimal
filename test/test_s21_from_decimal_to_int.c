#include <check.h>
#include <limits.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_decimal_to_int_positive) {
  s21_decimal src = {{12345, 0, 0, 0}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result, 12345);
}
END_TEST

START_TEST(test_decimal_to_int_negative) {
  s21_decimal src = {{6789, 0, 0, 0x80000000}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result, -6789);
}
END_TEST

START_TEST(test_decimal_to_int_zero) {
  s21_decimal src = {{0, 0, 0, 0}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_decimal_to_int_with_scale) {
  s21_decimal src = {{12345, 0, 0, 0x00020000}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result, 123);
}
END_TEST

START_TEST(test_decimal_to_int_max_int) {
  s21_decimal src = {{INT_MAX, 0, 0, 0}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result, INT_MAX);
}
END_TEST

START_TEST(test_decimal_to_int_overflow) {
  s21_decimal src = {{INT_MAX, 1, 0, 0}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_int_negative_overflow) {
  s21_decimal src = {{INT_MAX, 0, 0, 0x80000000}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result, -INT_MAX);
}
END_TEST

START_TEST(test_decimal_to_int_null_dst) {
  s21_decimal src = {{123, 0, 0, 0}};

  int ret = s21_from_decimal_to_int(src, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_int_fraction_truncated) {
  s21_decimal src = {{999, 0, 0, 0x00010000}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result, 99);
}
END_TEST

START_TEST(test_decimal_to_int_overflow_positive) {
  s21_decimal src = {{(unsigned int)INT_MAX + 1, 0, 0, 0}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_int_overflow_negative_max) {
  s21_decimal src = {{(unsigned int)INT_MAX + 1, 0, 0, 0x80000000}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_int_overflow_large_positive) {
  s21_decimal src = {{0xFFFFFFFF, 1, 0, 0}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_int_overflow_large_negative) {
  s21_decimal src = {{0xFFFFFFFF, 1, 0, 0x80000000}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_int_max_int_positive) {
  s21_decimal src = {{INT_MAX, 0, 0, 0}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result, INT_MAX);
}
END_TEST

START_TEST(test_decimal_to_int_max_int_negative) {
  s21_decimal src = {{INT_MAX, 0, 0, 0x80000000}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result, -INT_MAX);
}
END_TEST

START_TEST(test_decimal_to_int_negative_with_fraction_overflow) {
  s21_decimal src = {{0xFFFFFFFE, 0xFFFFFFFF, 0, 0x80010000}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_int_bits1_not_zero) {
  s21_decimal src = {{0, 1, 0, 0}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_int_bits2_not_zero) {
  s21_decimal src = {{0, 0, 1, 0}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_int_value_negative_overflow) {
  s21_decimal src = {{0x80000000, 0, 0, 0}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_int_value_positive_overflow) {
  s21_decimal src = {{0x80000000, 0, 0, 0}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_int_negative_value_overflow) {
  s21_decimal src = {{0x80000000, 0, 0, 0x80000000}};
  int result;

  int ret = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

TCase *tcase_from_decimal_to_int(void) {
  TCase *tc = tcase_create("s21_from_decimal_to_int");
  tcase_add_test(tc, test_decimal_to_int_positive);
  tcase_add_test(tc, test_decimal_to_int_negative);
  tcase_add_test(tc, test_decimal_to_int_zero);
  tcase_add_test(tc, test_decimal_to_int_with_scale);
  tcase_add_test(tc, test_decimal_to_int_max_int);
  tcase_add_test(tc, test_decimal_to_int_overflow);
  tcase_add_test(tc, test_decimal_to_int_negative_overflow);
  tcase_add_test(tc, test_decimal_to_int_null_dst);
  tcase_add_test(tc, test_decimal_to_int_fraction_truncated);
  tcase_add_test(tc, test_decimal_to_int_overflow_positive);
  tcase_add_test(tc, test_decimal_to_int_overflow_negative_max);
  tcase_add_test(tc, test_decimal_to_int_overflow_large_positive);
  tcase_add_test(tc, test_decimal_to_int_overflow_large_negative);
  tcase_add_test(tc, test_decimal_to_int_max_int_positive);
  tcase_add_test(tc, test_decimal_to_int_max_int_negative);
  tcase_add_test(tc, test_decimal_to_int_negative_with_fraction_overflow);

  tcase_add_test(tc, test_decimal_to_int_bits1_not_zero);
  tcase_add_test(tc, test_decimal_to_int_bits2_not_zero);
  tcase_add_test(tc, test_decimal_to_int_value_negative_overflow);
  tcase_add_test(tc, test_decimal_to_int_value_positive_overflow);
  tcase_add_test(tc, test_decimal_to_int_negative_value_overflow);

  return tc;
}
#include <check.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_div_normal) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_div(a, b, &result);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_div_by_zero) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_div(a, b, &result);
  ck_assert_int_eq(ret, 3);
}
END_TEST

START_TEST(test_div_scale_reduction) {
  s21_decimal a = {{100, 0, 0, 0x001D0000}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_div(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_le(s21_get_scale(result), 28);
}
END_TEST

START_TEST(test_div_scale_reduction_with_break) {
  s21_decimal a = {{1, 0, 0, 0x001E0000}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_div(a, b, &result);
  ck_assert(ret == 0 || ret == 1 || ret == 2);
}
END_TEST

START_TEST(test_div_overflow_after_scale_reduction) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x001E0000}};
  s21_decimal b = {{1, 0, 0, 0x001F0000}};
  s21_decimal result;

  int ret = s21_div(a, b, &result);
  ck_assert(ret == 0 || ret == 1 || ret == 2);
}
END_TEST

START_TEST(test_div_positive_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{1, 0, 0, 0x001F0000}};
  s21_decimal result;

  int ret = s21_div(a, b, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_div_negative_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal b = {{1, 0, 0, 0x001F0000}};
  s21_decimal result;

  int ret = s21_div(a, b, &result);
  ck_assert_int_eq(ret, 2);
}
END_TEST

START_TEST(test_div_zero_by_number) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_div(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

TCase *tcase_div(void) {
  TCase *tc = tcase_create("s21_div");
  tcase_add_test(tc, test_div_normal);
  tcase_add_test(tc, test_div_by_zero);
  tcase_add_test(tc, test_div_zero_by_number);
  tcase_add_test(tc, test_div_scale_reduction);
  tcase_add_test(tc, test_div_scale_reduction_with_break);
  tcase_add_test(tc, test_div_overflow_after_scale_reduction);
  tcase_add_test(tc, test_div_positive_overflow);
  tcase_add_test(tc, test_div_negative_overflow);
  return tc;
}
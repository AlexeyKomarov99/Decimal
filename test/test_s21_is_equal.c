#include <check.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_equal_positive_true) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_equal_positive_false) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_equal_negative_true) {
  s21_decimal a = {{10, 0, 0, 0x80000000}};
  s21_decimal b = {{10, 0, 0, 0x80000000}};

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_equal_negative_false) {
  s21_decimal a = {{10, 0, 0, 0x80000000}};
  s21_decimal b = {{5, 0, 0, 0x80000000}};

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_equal_mixed_signs) {
  s21_decimal a = {{10, 0, 0, 0x80000000}};
  s21_decimal b = {{10, 0, 0, 0}};

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_equal_zero_cases) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);  // +0 == -0
}
END_TEST

START_TEST(test_equal_with_scale) {
  s21_decimal a = {{10, 0, 0, 0x00010000}};   // 1.0
  s21_decimal b = {{100, 0, 0, 0x00020000}};  // 1.00

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);  // 1.0 == 1.00
}
END_TEST

TCase *tcase_equal(void) {
  TCase *tc = tcase_create("s21_is_equal");
  tcase_add_test(tc, test_equal_positive_true);
  tcase_add_test(tc, test_equal_positive_false);
  tcase_add_test(tc, test_equal_negative_true);
  tcase_add_test(tc, test_equal_negative_false);
  tcase_add_test(tc, test_equal_mixed_signs);
  tcase_add_test(tc, test_equal_zero_cases);
  tcase_add_test(tc, test_equal_with_scale);
  return tc;
}
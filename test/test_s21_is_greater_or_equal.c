#include <check.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_greater_or_equal_positive_greater) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_greater_or_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_greater_or_equal_positive_equal) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};

  int result = s21_is_greater_or_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_greater_or_equal_positive_less) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};

  int result = s21_is_greater_or_equal(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_greater_or_equal_negative_greater) {
  s21_decimal a = {{5, 0, 0, 0x80000000}};
  s21_decimal b = {{10, 0, 0, 0x80000000}};

  int result = s21_is_greater_or_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_greater_or_equal_mixed_signs) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0x80000000}};

  int result = s21_is_greater_or_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

TCase *tcase_greater_or_equal(void) {
  TCase *tc = tcase_create("s21_is_greater_or_equal");
  tcase_add_test(tc, test_greater_or_equal_positive_greater);
  tcase_add_test(tc, test_greater_or_equal_positive_equal);
  tcase_add_test(tc, test_greater_or_equal_positive_less);
  tcase_add_test(tc, test_greater_or_equal_negative_greater);
  tcase_add_test(tc, test_greater_or_equal_mixed_signs);
  return tc;
}
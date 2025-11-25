#include <check.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_not_equal_positive_true) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_not_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_not_equal_positive_false) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};

  int result = s21_is_not_equal(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_not_equal_negative_true) {
  s21_decimal a = {{10, 0, 0, 0x80000000}};
  s21_decimal b = {{5, 0, 0, 0x80000000}};

  int result = s21_is_not_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_not_equal_mixed_signs_true) {
  s21_decimal a = {{10, 0, 0, 0x80000000}};
  s21_decimal b = {{10, 0, 0, 0}};

  int result = s21_is_not_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_not_equal_zero_false) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};

  int result = s21_is_not_equal(a, b);
  ck_assert_int_eq(result, 0);  // +0 != -0 = FALSE
}
END_TEST

TCase *tcase_not_equal(void) {
  TCase *tc = tcase_create("s21_is_not_equal");
  tcase_add_test(tc, test_not_equal_positive_true);
  tcase_add_test(tc, test_not_equal_positive_false);
  tcase_add_test(tc, test_not_equal_negative_true);
  tcase_add_test(tc, test_not_equal_mixed_signs_true);
  tcase_add_test(tc, test_not_equal_zero_false);
  return tc;
}
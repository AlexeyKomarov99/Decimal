#include <check.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_less_positive) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_negative) {
  s21_decimal a = {{10, 0, 0, 0x80000000}};
  s21_decimal b = {{5, 0, 0, 0x80000000}};

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_mixed_signs) {
  s21_decimal a = {{5, 0, 0, 0x80000000}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_equal) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_less_greater) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

TCase *tcase_less(void) {
  TCase *tc = tcase_create("s21_is_less");
  tcase_add_test(tc, test_less_positive);
  tcase_add_test(tc, test_less_negative);
  tcase_add_test(tc, test_less_mixed_signs);
  tcase_add_test(tc, test_less_equal);
  tcase_add_test(tc, test_less_greater);
  return tc;
}
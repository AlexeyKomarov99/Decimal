#include <check.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_greater_positive_true) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_greater(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_greater_positive_false) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};

  int result = s21_is_greater(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_greater_negative_true) {
  s21_decimal a = {{5, 0, 0, 0x80000000}};
  s21_decimal b = {{10, 0, 0, 0x80000000}};

  int result = s21_is_greater(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_greater_negative_false) {
  s21_decimal a = {{10, 0, 0, 0x80000000}};
  s21_decimal b = {{5, 0, 0, 0x80000000}};

  int result = s21_is_greater(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_greater_mixed_signs) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0x80000000}};

  int result = s21_is_greater(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_greater_equal) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};

  int result = s21_is_greater(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

TCase *tcase_greater(void) {
  TCase *tc = tcase_create("s21_is_greater");
  tcase_add_test(tc, test_greater_positive_true);
  tcase_add_test(tc, test_greater_positive_false);
  tcase_add_test(tc, test_greater_negative_true);
  tcase_add_test(tc, test_greater_negative_false);
  tcase_add_test(tc, test_greater_mixed_signs);
  tcase_add_test(tc, test_greater_equal);
  return tc;
}
#include <check.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_sub_normal) {
  s21_decimal a = {{123, 0, 0, 0}};
  s21_decimal b = {{45, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{78, 0, 0, 0}};

  int ret = s21_sub(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[1], expected.bits[1]);
  ck_assert_int_eq(result.bits[2], expected.bits[2]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_sub_negative_first) {
  s21_decimal a = {{100, 0, 0, 0x80000000}};
  s21_decimal b = {{50, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{150, 0, 0, 0x80000000}};

  int ret = s21_sub(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_sub_negative_second) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{50, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{150, 0, 0, 0}};

  int ret = s21_sub(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_sub_zero_result) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_sub(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_sub_null_result) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{50, 0, 0, 0}};

  int ret = s21_sub(a, b, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_sub_first_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{50, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{50, 0, 0, 0x80000000}};

  int ret = s21_sub(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_sub_second_zero) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{100, 0, 0, 0}};

  int ret = s21_sub(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_sub_b_greater_than_a) {
  s21_decimal a = {{50, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{50, 0, 0, 0x80000000}};

  int ret = s21_sub(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_sub_b_greater_than_a_negative) {
  s21_decimal a = {{50, 0, 0, 0x80000000}};
  s21_decimal b = {{100, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{50, 0, 0, 0}};

  int ret = s21_sub(a, b, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

TCase *tcase_sub(void) {
  TCase *tc = tcase_create("s21_sub");

  tcase_add_test(tc, test_sub_normal);
  tcase_add_test(tc, test_sub_negative_first);
  tcase_add_test(tc, test_sub_negative_second);
  tcase_add_test(tc, test_sub_zero_result);
  tcase_add_test(tc, test_sub_null_result);
  tcase_add_test(tc, test_sub_first_zero);
  tcase_add_test(tc, test_sub_second_zero);
  tcase_add_test(tc, test_sub_b_greater_than_a);
  tcase_add_test(tc, test_sub_b_greater_than_a_negative);

  return tc;
}
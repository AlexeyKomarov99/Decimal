#include <check.h>
#include <stdlib.h>

#include "../src/s21_decimal.h"

START_TEST(test_floor_positive_no_fraction) {
  s21_decimal value = {{123, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0}};

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), s21_get_sign(expected));
}
END_TEST

START_TEST(test_floor_positive_with_fraction) {
  s21_decimal value = {{12345, 0, 0, 0x00020000}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0}};

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_floor_negative_no_fraction) {
  s21_decimal value = {{123, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0x80000000}};

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_floor_negative_with_fraction) {
  s21_decimal value = {{12345, 0, 0, 0x80020000}};
  s21_decimal result;
  s21_decimal expected = {{124, 0, 0, 0x80000000}};

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_floor_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_floor_small_fraction) {
  s21_decimal value = {{999, 0, 0, 0x00030000}};
  s21_decimal result;
  s21_decimal expected = {{0, 0, 0, 0}};

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_floor_null_result) {
  s21_decimal value = {{123, 0, 0, 0}};

  int ret = s21_floor(value, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_floor_already_integer) {
  s21_decimal value = {{123, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0}};

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), s21_get_sign(expected));
}
END_TEST

START_TEST(test_floor_zero_value) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result;

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_floor_negative_zero) {
  s21_decimal value = {{0, 0, 0, 0x80000000}};
  s21_decimal result;

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_is_zero(result), 1);
}
END_TEST

START_TEST(test_floor_has_fractional_part) {
  s21_decimal value = {{12345, 0, 0, 0x00020000}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0}};

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), s21_get_sign(expected));
}
END_TEST

START_TEST(test_floor_no_fractional_part) {
  s21_decimal value = {{12300, 0, 0, 0x00020000}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0}};

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), s21_get_sign(expected));
}
END_TEST

START_TEST(test_floor_negative_has_fractional_part) {
  s21_decimal value = {{12345, 0, 0, 0x80020000}};
  s21_decimal result;
  s21_decimal expected = {{124, 0, 0, 0x80000000}};

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_floor_negative_no_fractional_part) {
  s21_decimal value = {{12300, 0, 0, 0x80020000}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0x80000000}};

  int ret = s21_floor(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

TCase *tcase_floor(void) {
  TCase *tc = tcase_create("s21_floor");
  tcase_add_test(tc, test_floor_positive_no_fraction);
  tcase_add_test(tc, test_floor_positive_with_fraction);
  tcase_add_test(tc, test_floor_negative_no_fraction);
  tcase_add_test(tc, test_floor_negative_with_fraction);
  tcase_add_test(tc, test_floor_zero);
  tcase_add_test(tc, test_floor_small_fraction);
  tcase_add_test(tc, test_floor_null_result);
  tcase_add_test(tc, test_floor_already_integer);
  tcase_add_test(tc, test_floor_zero_value);
  tcase_add_test(tc, test_floor_negative_zero);
  tcase_add_test(tc, test_floor_has_fractional_part);
  tcase_add_test(tc, test_floor_no_fractional_part);
  tcase_add_test(tc, test_floor_negative_has_fractional_part);
  tcase_add_test(tc, test_floor_negative_no_fractional_part);
  return tc;
}
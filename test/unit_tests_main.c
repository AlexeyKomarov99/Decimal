#include <check.h>
#include <stdio.h>
#include <stdlib.h>

TCase *tcase_add(void);
TCase *tcase_sub(void);
TCase *tcase_mul(void);
TCase *tcase_div(void);

TCase *tcase_less(void);
TCase *tcase_less_or_equal(void);
TCase *tcase_equal(void);
TCase *tcase_greater(void);
TCase *tcase_greater_or_equal(void);
TCase *tcase_not_equal(void);

TCase *tcase_from_int_to_decimal(void);
TCase *tcase_from_float_to_decimal(void);
TCase *tcase_from_decimal_to_int(void);
TCase *tcase_from_decimal_to_float(void);

TCase *tcase_floor(void);
TCase *tcase_round(void);
TCase *tcase_truncate(void);
TCase *tcase_negate(void);

Suite *decimal_suite(void) {
  Suite *s = suite_create("Decimal Library Tests");

  suite_add_tcase(s, tcase_add());
  suite_add_tcase(s, tcase_sub());
  suite_add_tcase(s, tcase_mul());
  suite_add_tcase(s, tcase_div());

  suite_add_tcase(s, tcase_less());
  suite_add_tcase(s, tcase_less_or_equal());
  suite_add_tcase(s, tcase_equal());
  suite_add_tcase(s, tcase_greater());
  suite_add_tcase(s, tcase_greater_or_equal());
  suite_add_tcase(s, tcase_not_equal());

  suite_add_tcase(s, tcase_from_int_to_decimal());
  suite_add_tcase(s, tcase_from_float_to_decimal());
  suite_add_tcase(s, tcase_from_decimal_to_int());
  suite_add_tcase(s, tcase_from_decimal_to_float());

  suite_add_tcase(s, tcase_floor());
  suite_add_tcase(s, tcase_round());
  suite_add_tcase(s, tcase_truncate());
  suite_add_tcase(s, tcase_negate());

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = decimal_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
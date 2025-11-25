#include "../auxiliary/s21_auxiliary.h"
#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int return_code = 1;

  if (dst) {
    s21_init_decimal(dst);

    if (src == 0) {
      return_code = 0;
    } else {
      if (src < 0) {
        s21_set_sign(dst, 1);
        src = -src;
      } else {
        s21_set_sign(dst, 0);
      }

      dst->bits[0] = src;
      return_code = 0;
    }
  }

  return return_code;
}
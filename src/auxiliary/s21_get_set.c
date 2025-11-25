#include <stdint.h>

#include "s21_auxiliary.h"

int s21_get_sign(s21_decimal value) {
  uint32_t bits3 = (uint32_t)value.bits[3];
  return (bits3 >> 31) & 1;
}

void s21_set_sign(s21_decimal *value, int sign) {
  if (value) {
    uint32_t bits3 = (uint32_t)value->bits[3];
    bits3 &= ~(1u << 31);
    bits3 |= ((uint32_t)(sign & 1) << 31);
    value->bits[3] = (int32_t)bits3;
  }
}

int s21_get_scale(s21_decimal value) {
  uint32_t bits3 = (uint32_t)value.bits[3];
  return (bits3 >> 16) & 0xFF;
}

void s21_set_scale(s21_decimal *value, int scale) {
  if (value && scale >= 0 && scale <= 28) {
    uint32_t bits3 = (uint32_t)value->bits[3];
    bits3 &= 0xFF00FFFF;
    bits3 |= ((uint32_t)(scale & 0xFF) << 16);
    value->bits[3] = (int32_t)bits3;
  }
}

int s21_is_zero(s21_decimal value) {
  int result = 0;

  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) {
    result = 1;
  }

  return result;
}

void s21_init_decimal(s21_decimal *value) {
  if (value) {
    int i = 0;

    while (i < 4) {
      value->bits[i] = 0;
      i++;
    }
  }
}
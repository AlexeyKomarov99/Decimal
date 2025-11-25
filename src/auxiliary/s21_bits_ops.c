#include "s21_auxiliary.h"

int s21_compare_bits(s21_decimal a, s21_decimal b) {
  int result = 0;
  int comparison_done = 0;
  int i = 2;

  while (i >= 0 && !comparison_done) {
    if (a.bits[i] > b.bits[i]) {
      result = 1;
      comparison_done = 1;
    } else if (a.bits[i] < b.bits[i]) {
      result = -1;
      comparison_done = 1;
    }
    i--;
  }

  return result;
}

void s21_shift_left(s21_decimal *value) {
  if (value) {
    uint32_t carry_prev = 0;
    int i = 0;

    while (i < 3) {
      uint32_t current = (uint32_t)value->bits[i];
      uint32_t carry_current = (current >> 31) & 1;
      value->bits[i] = (current << 1) | carry_prev;
      carry_prev = carry_current;
      i++;
    }
  }
}

void s21_shift_right(s21_decimal *value) {
  if (value) {
    uint32_t carry_prev = 0;
    int i = 2;

    while (i >= 0) {
      uint32_t current = (uint32_t)value->bits[i];
      uint32_t carry_current = current & 1;
      value->bits[i] = (current >> 1) | (carry_prev << 31);
      carry_prev = carry_current;
      i--;
    }
  }
}

int s21_get_bit(s21_decimal value, int position) {
  int result = 0;

  if (position >= 0 && position < 96) {
    int word_index = position / 32;
    int bit_index = position % 32;
    uint32_t word_value = (uint32_t)value.bits[word_index];
    result = (word_value >> bit_index) & 1;
  }

  return result;
}
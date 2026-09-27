/*
 * SPDX-FileCopyrightText: 2017-2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef _FCITX_LIBIME_PINYIN_CONSTANTS_H_
#define _FCITX_LIBIME_PINYIN_CONSTANTS_H_

namespace libime {
constexpr float PINYIN_DISTANCE_PENALTY_FACTOR = 1.8;
constexpr int PINYIN_ADVACNED_TYPO_FUZZY_FACTOR = 5;
constexpr int PINYIN_CORRECTION_FUZZY_FACTOR = 10;
// zc fork: one missing or extra letter (PinyinFuzzyFlag::EditTypo).
constexpr int PINYIN_EDIT_TYPO_FUZZY_FACTOR = 10;
// zc fork: z/zh, c/ch, s/sh each cost this many fuzzies instead of sharing one
// with every other fuzzy flag. At 1 the flat fuzzy broke 24 of 1088 correctly
// typed sentences; at 3 it breaks 2 and keeps 99% of what it corrects.
constexpr int PINYIN_FLAT_FUZZY_FACTOR = 3;
// zc fork: a syllable typed as its initial only. At 1 a mistyped last letter was
// read as one more abbreviated character; 3 corrects more swaps and omissions
// without losing mixed abbreviations, 5 starts to.
constexpr int PINYIN_INITIAL_ONLY_FUZZY_FACTOR = 3;
// zc fork: two neighbouring letters swapped (PinyinFuzzyFlag::Transpose). Against the
// hand-written swap tables it replaces: 5 breaks a correctly typed word (gezia read as zai)
// and 13 omissions, 7 breaks none and nets +29 swapped words in 3000, 10 loses the common
// swaps (hsi, gne) to an initial-only split.
constexpr int PINYIN_TRANSPOSE_FUZZY_FACTOR = 7;
// zc fork: log10 penalty for decoding a one-edit repair (PinyinRepair) instead of the input.
// 3 corrects about 100 more swaps in 3000 words but breaks 17 omissions; 4 breaks 6, 2 breaks 100.
constexpr float PINYIN_REPAIR_PENALTY = 4;
} // namespace libime

#endif // _FCITX_LIBIME_PINYIN_CONSTANTS_H_

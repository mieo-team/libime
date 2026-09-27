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
// zc fork: z/zh, c/ch, s/sh each cost this many fuzzies instead of sharing one
// with every other fuzzy flag. At 1 the flat fuzzy broke 24 of 1088 correctly
// typed sentences; at 3 it breaks 2 and keeps 99% of what it corrects.
constexpr int PINYIN_FLAT_FUZZY_FACTOR = 3;
// zc fork: a syllable typed as its initial only. At 1 a mistyped last letter was
// read as one more abbreviated character; 3 corrects more swaps and omissions
// without losing mixed abbreviations, 5 starts to.
constexpr int PINYIN_INITIAL_ONLY_FUZZY_FACTOR = 3;
} // namespace libime

#endif // _FCITX_LIBIME_PINYIN_CONSTANTS_H_

/*
 * SPDX-FileCopyrightText: 2026-2026 ZCubed
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef _FCITX_LIBIME_PINYIN_TYPOEDITS_H_
#define _FCITX_LIBIME_PINYIN_TYPOEDITS_H_

#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "pinyinencoder.h"

namespace libime {

// zc fork: the one typing-error model. A string one edit away from what was meant, and the
// kind of slip that produces it; the kind is the flag its spelling is charged under.
struct TypoEdit {
    std::string spelling;
    PinyinFuzzyFlag kind;
    size_t at; // the first letter the slip touches
};

/**
 * Every string one slip away from `s`: two neighbouring letters swapped (Transpose), a letter
 * dropped or an extra letter after one (EditTypo: the same key twice, a neighbour of it or of
 * the next letter), a letter replaced by a neighbour (Correction). The syllable spellings in
 * PinyinCorrectionProfile and the whole-input repair in PinyinRepair both come from here.
 */
std::vector<TypoEdit>
oneEdit(std::string_view s,
        const std::unordered_map<char, std::vector<char>> &neighbours);

} // namespace libime

#endif // _FCITX_LIBIME_PINYIN_TYPOEDITS_H_

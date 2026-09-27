/*
 * SPDX-FileCopyrightText: 2026-2026 ZCubed
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef _FCITX_LIBIME_PINYIN_PINYINREPAIR_H_
#define _FCITX_LIBIME_PINYIN_PINYINREPAIR_H_

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
#include <libime/pinyin/libimepinyin_export.h>

namespace libime {

class PinyinContext;
class PinyinIME;

/**
 * zc fork: repair a full-pinyin input that splits badly by one edit.
 *
 * A typo that crosses syllables (queding typed queidng) leaves letters that are no syllable
 * at all, which the per-syllable typo spellings cannot reach. The caller tries each string one
 * edit away that splits better, decodes it on a context of its own and keeps the best. Input
 * that splits into full syllables has no variants, so typing correctly is never touched.
 */
class LIBIMEPINYIN_EXPORT PinyinRepair {
public:
    explicit PinyinRepair(PinyinIME *ime);
    ~PinyinRepair();

    /** How badly `input` splits: full syllable 0, initial only 1, any other letter 5. */
    static int splitCost(std::string_view input);

    /**
     * Strings one slip away (oneEdit in typoedits.h, the same model the syllable spellings
     * come from). Empty when `input` splits into full syllables.
     */
    static std::vector<std::string>
    variants(std::string_view input,
             const std::unordered_map<char, std::vector<char>> &neighbours);

    /**
     * The first candidate for `variant` and its score, already charged the repair penalty so
     * it compares directly with the input's own first candidate. Empty text when there is none.
     */
    std::pair<std::string, float> decode(const std::string &variant);

private:
    std::unique_ptr<PinyinContext> context_;
};

} // namespace libime

#endif // _FCITX_LIBIME_PINYIN_PINYINREPAIR_H_

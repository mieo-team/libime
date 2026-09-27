/*
 * SPDX-FileCopyrightText: 2024-2024 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#include "pinyincorrectionprofile.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fcitx-utils/macros.h>
#include "pinyindata.h"
#include "pinyinencoder.h"
#include "typoedits.h"

namespace libime {

namespace {

/*
 * Helper function to create mapping based on keyboard rows.
 * Function assume that the key can only be corrected to the key adjcent to it.
 */
std::unordered_map<char, std::vector<char>>
mappingFromRows(const std::vector<std::string> &rows) {
    std::unordered_map<char, std::vector<char>> result;
    for (const auto &row : rows) {
        for (size_t i = 0; i < row.size(); i++) {
            std::vector<char> items;
            if (i > 0) {
                items.push_back(row[i - 1]);
            }
            if (i + 1 < row.size()) {
                items.push_back(row[i + 1]);
            }
            result[row[i]] = std::move(items);
        }
    }
    return result;
}

std::unordered_map<char, std::vector<char>>
getProfileMapping(BuiltinPinyinCorrectionProfile profile) {
    switch (profile) {
    case BuiltinPinyinCorrectionProfile::Qwerty:
        return mappingFromRows({"qwertyuiop", "asdfghjkl", "zxcvbnm"});
    }

    return {};
}
} // namespace

class PinyinCorrectionProfilePrivate {
public:
    PinyinMap pinyinMap_;
    std::unordered_map<char, std::vector<char>> correctionMap_;
};

PinyinCorrectionProfile::PinyinCorrectionProfile(
    BuiltinPinyinCorrectionProfile profile)
    : PinyinCorrectionProfile(getProfileMapping(profile)) {}

PinyinCorrectionProfile::PinyinCorrectionProfile(
    const std::unordered_map<char, std::vector<char>> &mapping)
    : d_ptr(std::make_unique<PinyinCorrectionProfilePrivate>()) {
    FCITX_D();
    d->correctionMap_ = mapping;
    // Fill with the original pinyin map.
    d->pinyinMap_ = getPinyinMapV2();
    if (mapping.empty()) {
        return;
    }
    // zc fork: the one typing-error model (typoedits.h). Each full syllable gets every single
    // slip once, charged by its kind: a neighbour for a letter (Correction), two neighbouring
    // letters swapped (Transpose), a letter dropped or one extra (EditTypo). A replaced key may
    // land on another syllable (da typed sa); a swap, drop or extra that already spells a
    // syllable, or only an initial, stays exact. A one-letter initial is never swapped into the
    // final (hei to ehi): spellings that start inside a final match all over correct input and
    // made every keystroke of a sentence about 50% slower; sih for shi stays.
    const auto isSyllable = [d](const std::string &spelling) {
        auto range = d->pinyinMap_.equal_range(spelling);
        return std::any_of(range.first, range.second, [](const auto &item) {
            return item.flags() == PinyinFuzzyFlag::None;
        });
    };
    std::set<std::tuple<std::string, PinyinInitial, PinyinFinal, PinyinFuzzyFlag>> slips;
    for (const auto &item : d->pinyinMap_) {
        const auto &py = item.pinyin();
        if (item.flags() != PinyinFuzzyFlag::None || py == "ng" || py == "hm" || py == "hng") {
            continue;
        }
        const auto initialSize = PinyinEncoder::initialToString(item.initial()).size();
        for (const auto &[spelling, kind, at] : oneEdit(py, mapping)) {
            if (kind == PinyinFuzzyFlag::Transpose && at == 0 && initialSize == 1) {
                continue;
            }
            const bool replaced = kind == PinyinFuzzyFlag::Correction;
            if (!replaced && (py.size() < 2 || spelling.size() < 2 || spelling == "zh" ||
                              spelling == "ch" || spelling == "sh" || isSyllable(spelling))) {
                continue;
            }
            slips.emplace(spelling, item.initial(), item.final(), kind);
        }
    }
    std::vector<PinyinEntry> newEntries;
    for (const auto &[spelling, initial, final, kind] : slips) {
        newEntries.emplace_back(spelling.data(), initial, final, kind);
    }
    for (const auto &newEntry : newEntries) {
        d->pinyinMap_.insert(newEntry);
    }
}

PinyinCorrectionProfile::~PinyinCorrectionProfile() = default;

const PinyinMap &PinyinCorrectionProfile::pinyinMap() const {
    FCITX_D();
    return d->pinyinMap_;
}

const std::unordered_map<char, std::vector<char>> &
PinyinCorrectionProfile::correctionMap() const {
    FCITX_D();
    return d->correctionMap_;
}
} // namespace libime

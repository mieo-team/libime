/*
 * SPDX-FileCopyrightText: 2026-2026 ZCubed
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#include "typoedits.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace libime {

std::vector<TypoEdit>
oneEdit(std::string_view s,
        const std::unordered_map<char, std::vector<char>> &neighbours) {
    static const std::vector<char> none;
    auto near = [&](char c) -> const std::vector<char> & {
        auto it = neighbours.find(c);
        return it == neighbours.end() ? none : it->second;
    };
    const std::string w(s);
    std::vector<TypoEdit> out;
    for (size_t i = 0; i + 1 < w.size(); i++) {
        if (w[i] != w[i + 1]) {
            out.push_back({w.substr(0, i) + w[i + 1] + w[i] + w.substr(i + 2),
                           PinyinFuzzyFlag::Transpose, i});
        }
    }
    for (size_t i = 0; i < w.size(); i++) {
        out.push_back({w.substr(0, i) + w.substr(i + 1), PinyinFuzzyFlag::EditTypo, i});
        std::string extra(1, w[i]);
        extra.append(near(w[i]).begin(), near(w[i]).end());
        if (i + 1 < w.size()) {
            extra.append(near(w[i + 1]).begin(), near(w[i + 1]).end());
        }
        for (char c : extra) {
            out.push_back({w.substr(0, i + 1) + c + w.substr(i + 1), PinyinFuzzyFlag::EditTypo, i});
        }
        for (char c : near(w[i])) {
            out.push_back({w.substr(0, i) + c + w.substr(i + 1), PinyinFuzzyFlag::Correction, i});
        }
    }
    return out;
}

} // namespace libime

/*
 * SPDX-FileCopyrightText: 2026-2026 ZCubed
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#include "pinyinrepair.h"
#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
#include "constants.h"
#include "pinyincontext.h"
#include "pinyindata.h"
#include "pinyinencoder.h"
#include "typoedits.h"

namespace libime {

PinyinRepair::PinyinRepair(PinyinIME *ime)
    : context_(std::make_unique<PinyinContext>(ime)) {}

PinyinRepair::~PinyinRepair() = default;

int PinyinRepair::splitCost(std::string_view input) {
    const auto &map = getPinyinMap();
    constexpr int unreachable = 1 << 20;
    std::vector<int> best(input.size() + 1, unreachable);
    best[0] = 0;
    for (size_t i = 0; i < input.size(); i++) {
        if (best[i] == unreachable) {
            continue;
        }
        best[i + 1] = std::min(best[i + 1], best[i] + 5);
        for (size_t j = i + 1; j <= input.size() && j <= i + 6; j++) {
            const auto seg = input.substr(i, j - i);
            auto [begin, end] = map.equal_range(seg);
            if (std::any_of(begin, end, [](const PinyinEntry &e) {
                    return e.flags() == PinyinFuzzyFlag::None;
                })) {
                best[j] = std::min(best[j], best[i]);
            } else if (PinyinEncoder::stringToInitial(std::string(seg)) !=
                       PinyinInitial::Invalid) {
                best[j] = std::min(best[j], best[i] + 1);
            }
        }
    }
    return best[input.size()];
}

std::vector<std::string> PinyinRepair::variants(
    std::string_view input,
    const std::unordered_map<char, std::vector<char>> &neighbours) {
    const int cost = splitCost(input);
    if (cost == 0) {
        return {};
    }
    std::vector<std::string> out;
    for (auto &edit : oneEdit(input, neighbours)) {
        out.push_back(std::move(edit.spelling));
    }
    std::sort(out.begin(), out.end());
    out.erase(std::unique(out.begin(), out.end()), out.end());
    std::erase_if(out, [cost](const std::string &v) { return splitCost(v) >= cost; });
    return out;
}

std::pair<std::string, float> PinyinRepair::decode(const std::string &variant) {
    context_->clear();
    context_->type(variant);
    const auto &candidates = context_->candidates();
    if (candidates.empty()) {
        return {"", 0};
    }
    return {candidates[0].toString(), candidates[0].score() - PINYIN_REPAIR_PENALTY};
}

} // namespace libime

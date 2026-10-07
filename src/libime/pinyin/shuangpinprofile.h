/*
 * SPDX-FileCopyrightText: 2017-2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef _FCITX_LIBIME_PINYIN_SHUANGPINPROFILE_H_
#define _FCITX_LIBIME_PINYIN_SHUANGPINPROFILE_H_

#include <istream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <fcitx-utils/macros.h>
#include <libime/pinyin/libimepinyin_export.h>
#include <libime/pinyin/pinyincorrectionprofile.h>
#include <libime/pinyin/pinyinencoder.h>

namespace libime {

enum class ShuangpinBuiltinProfile {
    Ziranma,
    MS,
    Ziguang,
    ABC,
    Zhongwenzhixing,
    PinyinJiajia,
    Xiaohe,
    GB,
    // Nine-key: digits 2-9, each standing for its letters. Keys are 1-6 digits long.
    T9,
};

// zc fork: a two-key scheme whose keys the caller gives, laid out like Shoudao shuangpin
// (https://shoudaoshuangpin.github.io/); ZCubed reads them from spec/shuangpin/*.json so the
// key faces and the decoder share one table. initialKeys maps a multi-letter initial to its
// key (zh -> v); every other initial keeps its own letter. finalKeys maps a final to its key
// (several finals may share one: ong -> h, iong -> h). zeroSpellings maps a zero-initial
// syllable to its two keys (ang -> ay). A single key is an initial-only prefix, and also a
// zero-initial prefix when some zero-initial spelling starts with it.
struct ShuangpinTables {
    std::unordered_map<std::string, std::string> initialKeys;
    std::unordered_map<std::string, std::string> finalKeys;
    std::unordered_map<std::string, std::string> zeroSpellings;
};

class ShuangpinProfilePrivate;

class LIBIMEPINYIN_EXPORT ShuangpinProfile {
public:
    using TableType =
        std::map<std::string, std::multimap<PinyinSyllable, PinyinFuzzyFlags>>;
    using ValidInputSetType = std::set<char>;
    explicit ShuangpinProfile(ShuangpinBuiltinProfile profile);
    explicit ShuangpinProfile(std::istream &in);

    explicit ShuangpinProfile(ShuangpinBuiltinProfile profile,
                              const PinyinCorrectionProfile *correctionProfile);
    explicit ShuangpinProfile(std::istream &in,
                              const PinyinCorrectionProfile *correctionProfile);
    explicit ShuangpinProfile(const ShuangpinTables &tables);

    FCITX_DECLARE_VIRTUAL_DTOR_COPY_AND_MOVE(ShuangpinProfile)

    const TableType &table() const;
    const ValidInputSetType &validInput() const;
    const ValidInputSetType &validInitial() const;
    bool isT9() const;

private:
    void buildShuangpinTable();
    std::unique_ptr<ShuangpinProfilePrivate> d_ptr;
    FCITX_DECLARE_PRIVATE(ShuangpinProfile);
};
} // namespace libime

#endif // _FCITX_LIBIME_PINYIN_SHUANGPINPROFILE_H_

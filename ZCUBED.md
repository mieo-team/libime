# ZCubed changes to libime

This is libime as used by the ZCubed input method for Android (https://zcubed.cn). It is based on
upstream commit `7b638a4`, the revision fcitx5-android 0.1.3 builds, and is distributed under the
same license, LGPL-2.1-or-later. Every change is a separate commit on the `zcubed` branch, and
changed code is marked `zc fork:` where it is not self-evident.

| Change | Files |
|---|---|
| Expand each segment graph node once (exponential walk on dense graphs) | `core/segmentgraph.cpp`, `pinyin/pinyindictionary.cpp` |
| `lookupWord` returns the stored cost, not its raw bits | `pinyin/pinyindictionary.cpp` |
| Per-keystroke correction costs (`PinyinContext::setKeyCosts`) | `pinyin/pinyincontext.*`, `pinyin/pinyinmatchstate*`, `pinyin/pinyindictionary.cpp` |
| Sub-dictionary override order (`PinyinDictionary::setOverrideOrder`) | `pinyin/pinyindictionary.*` |
| Nine-key input as a shuangpin profile (`ShuangpinBuiltinProfile::T9`) | `pinyin/shuangpinprofile.*`, `pinyin/pinyinencoder.cpp` |
| Shoudao shuangpin profile (`ShuangpinBuiltinProfile::Shoudao`) | `pinyin/shuangpinprofile.*` |
| Flat fuzzy (z/zh, c/ch, s/sh) and initial-only syllables weighed at 3 fuzzies | `pinyin/constants.h`, `pinyin/pinyindictionary.cpp` |
| One typing-error model: neighbour, dropped or extra letter, swapped letters (`typoedits.h`, flags `EditTypo`, `Transpose`); replaces the hand-written CommonTypo/AdvancedTypo tables except jv/qv/xv/yv | `pinyin/typoedits.*`, `pinyin/pinyincorrectionprofile.cpp`, `pinyin/pinyindata.cpp`, `pinyin/pinyinencoder.*`, `pinyin/pinyindictionary.cpp`, `pinyin/constants.h` |
| One-slip repair of a badly split input (`PinyinRepair`) | `pinyin/pinyinrepair.*`, `pinyin/constants.h` |
| Language model mapped where it sits inside another file, e.g. an uncompressed APK entry (`StaticLanguageModelFile(file, offset, length)`), prediction table from another source (`setPredictionSource`); needs the KenLM change below | `core/languagemodel.*` |
| KenLM: read a binary model from a byte range of a larger file (`Config::file_offset`, `file_length`) | submodule `core/kenlm`: `lm/config.*`, `lm/binary_format.*`, `lm/model.cc` |
| Nine-key: a half-typed final before the end of the input weighed like an initial-only syllable; the last segment skips the pinyin-keyed match caches | `pinyin/constants.h`, `pinyin/pinyindictionary.cpp` |

Building: the same as upstream. ZCubed compiles `src/libime/core` and `src/libime/pinyin` from this
tree with the Android NDK, against the boost and zstd builds of fcitx5-android/prebuilt and the
`libFcitx5Utils.so` shipped by fcitx5-android 0.1.3.

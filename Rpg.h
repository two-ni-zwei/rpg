#ifndef RPG_H
#define RPG_H

#include <iostream>
#include <random>
#include <string>

std::vector<int> concat_symbol_chars(const std::vector<int>& vec1, const std::vector<int>& vec2, const std::vector<int>& vec3);

struct RpgRange
{
    int min;
    int max;
};

class Rpg
{
    using CharPool = std::vector<int>;
public:
    Rpg(size_t length, bool upper, bool lower, bool num, bool sym);

    std::string generate();
    void generatePool();
    void useUpper(bool option);
    void useLower(bool option);
    void useNumbers(bool option);
    void useSymbols(bool option);
    void excludeSimilarChars(bool option);
    void excludeAmbiguousSymbols(bool option);
    int randomPoolElement();
    void setLength(int new_length) noexcept { m_PassLength = new_length; }
	constexpr int length() noexcept { return m_PassLength; }
private:
    static std::vector<int> charRange(RpgRange range);
private:
    size_t m_PassLength;
    CharPool m_UppercaseLetters{charRange(RpgRange{65, 90})};
    CharPool m_LowercaseLetters{charRange(RpgRange{97, 122})};
    CharPool m_Numbers{charRange(RpgRange{48, 57})};
    CharPool m_Symbols{concat_symbol_chars(charRange(RpgRange{33, 47}), charRange(RpgRange{58, 64}), charRange(RpgRange{123, 126}))};
    bool m_UseUpper{true};
    bool m_UseLower{true};
    bool m_UseNumbers{false};
    bool m_UseSymbols{false};
    bool m_ExcludeSimilarChars{false};
    bool m_ExcludeAmbiguousSymbols{false};

    std::vector<int> m_CharacterPool;

    const std::vector<char> EXCLUDED_CHARS{'0', 'O', 'o', 'L', 'l', 'i', '1'};
    const std::vector<char> EXCLUDED_SYMBOLS{'{', '}', '[', ']', '(', ')', '/', '\\', '\'', '\"', '`', '~', ',', ';', ':', '.', '<', '>'};

};

#endif // RPG_H

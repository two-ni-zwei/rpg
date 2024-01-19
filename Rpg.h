#ifndef RPG_H
#define RPG_H

#include <string>
#include <iostream>
#include <random>

std::vector<int> concat_symbol_chars(const std::vector<int>& v1, const std::vector<int>& v2, const std::vector<int>& v3);

class PassGenerator
{
    using CharPool = std::vector<int>;
public:
    PassGenerator(size_t length, bool ul, bool ll, bool num, bool sym);

    std::string Generate();
    void GeneratePool();
    void UseUpper(bool b);
    void UseLower(bool b);
    void UseNumbers(bool b);
    void UseSymbols(bool b);
    void ExcludeSimilarChars(bool b);
    void ExcludeAmbiguousSymbols(bool b);
    int RandomPoolElement();
    void SetLength(int new_length) { pass_length = new_length; }
	const int Length() { return pass_length; }
private:
    std::vector<int> CharRange(int lower, int upper);
    size_t pass_length;
    CharPool uppercase_letters{CharRange(65, 90)};
    CharPool lowercase_letters{CharRange(97, 122)};
    CharPool numbers{CharRange(48, 57)};
    CharPool symbols{concat_symbol_chars(CharRange(33, 47), CharRange(58, 64), CharRange(123, 126))};
    bool use_upper{true};
    bool use_lower{true};
    bool use_numbers{false};
    bool use_symbols{false};
    bool exclude_similar_chars{false};
    bool exclude_ambiguous_symbols{false};

    std::vector<int> pool;

};

#endif // RPG_H

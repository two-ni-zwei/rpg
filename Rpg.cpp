#include "Rpg.h"

#include <algorithm>

bool contains(const std::vector<char>& vec, char key)
{
    return (std::find(std::begin(vec), std::end(vec), key) != std::end(vec));
}

void RemoveFromPool(std::vector<int>& pool, const std::vector<char>& targets)
{
    pool.erase(std::remove_if(pool.begin(), pool.end(), [&targets](int elem) { return contains(targets, elem);}), pool.end());
}

std::vector<int> Rpg::charRange(RpgRange range)
{
    std::vector<int> retval;
    for (int character = range.min; character <= range.max; ++character)
    {
        retval.push_back(character);
    }
    return retval;
}

std::vector<int> concat_symbol_chars(const std::vector<int>& vec1, const std::vector<int>& vec2, const std::vector<int>& vec3)
{
    std::vector<int> retval;
    retval.reserve(vec1.size() + vec2.size() + vec3.size());
    retval.insert(retval.end(), vec1.begin(), vec1.end());
    retval.insert(retval.end(), vec2.begin(), vec2.end());
    retval.insert(retval.end(), vec3.begin(), vec3.end());
    return retval;
}

Rpg::Rpg(size_t length, bool upper, bool lower, bool num, bool sym) : 
    m_PassLength(length), m_UseUpper(upper), m_UseLower(lower), m_UseNumbers(num), m_UseSymbols(sym)
{
    m_CharacterPool.reserve(m_UppercaseLetters.size() + m_LowercaseLetters.size() + m_Numbers.size() + m_Symbols.size());
    generatePool();
}

std::string Rpg::generate()
{
    std::string password;
    for (size_t i = 0; i != m_PassLength; ++i)
    {
        if (!m_CharacterPool.empty())
        {
            password.append(1, m_CharacterPool[randomPoolElement()]);
        }
    }
    return password;
}

void Rpg::generatePool()
{
    m_CharacterPool.clear();
    if (m_UseLower)
    {
        m_CharacterPool.insert(m_CharacterPool.end(), m_LowercaseLetters.begin(), m_LowercaseLetters.end());
    }
    if (m_UseUpper)
    {
        m_CharacterPool.insert(m_CharacterPool.end(), m_UppercaseLetters.begin(), m_UppercaseLetters.end());
    }

    if (m_UseNumbers)
    {
        m_CharacterPool.insert(m_CharacterPool.end(), m_Numbers.begin(), m_Numbers.end());
    }

    if (m_UseSymbols)
    {
        m_CharacterPool.insert(m_CharacterPool.end(), m_Symbols.begin(), m_Symbols.end());
    }

    if (m_ExcludeSimilarChars)
    {
        RemoveFromPool(m_CharacterPool, EXCLUDED_CHARS);
    }

    if (m_ExcludeAmbiguousSymbols)
    {
        RemoveFromPool(m_CharacterPool, EXCLUDED_SYMBOLS);
    }
}

void Rpg::useUpper(bool option)
{
    m_UseUpper = option;
    generatePool();
}

void Rpg::useLower(bool option)
{
    m_UseLower = option;
    generatePool();
}

void Rpg::useNumbers(bool option)
{
    m_UseNumbers = option;
    generatePool();
}

void Rpg::useSymbols(bool option)
{
    m_UseSymbols = option;
    generatePool();
}

void Rpg::excludeSimilarChars(bool option)
{
    m_ExcludeSimilarChars = option;
    generatePool();
}

void Rpg::excludeAmbiguousSymbols(bool option)
{
    m_ExcludeAmbiguousSymbols = option;
    generatePool();
}

int Rpg::randomPoolElement()
{
    std::random_device device{"/dev/urandom"};
    std::mt19937 engine(device());
    std::uniform_int_distribution<> distribution(0, m_CharacterPool.size()-1);
    return distribution(engine);
}

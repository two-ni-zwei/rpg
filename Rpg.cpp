#include "Rpg.h"

#include <algorithm>

const std::vector<char> EXCLUDED_CHARS{'0', 'O', 'o', 'L', 'l', 'i', '1'};
const std::vector<char> EXCLUDED_SYMBOLS{'{', '}', '[', ']', '(', ')', '/', '\\', '\'', '\"', '`', '~', ',', ';', ':', '.', '<', '>'};

bool contains(const std::vector<char>& a, char key)
{
    if (std::find(std::begin(a), std::end(a), key) != std::end(a))
    {
        return true;
    }
    return false;
}

void RemoveFromPool(std::vector<int>& pool, const std::vector<char>& targets)
{
    pool.erase(std::remove_if(pool.begin(), pool.end(), [&targets](int i) { return contains(targets, i);}), pool.end());
}

std::vector<int> PassGenerator::CharRange(int lower, int upper)
{
    std::vector<int> retval;
    for (int l = lower; l <= upper; ++l)
    {
        retval.push_back(l);
    }
    return retval;
}

std::vector<int> concat_symbol_chars(const std::vector<int>& v1, const std::vector<int>& v2, const std::vector<int>& v3)
{
    std::vector<int> retval;
    retval.reserve(v1.size() + v2.size() + v3.size());
    retval.insert(retval.end(), v1.begin(), v1.end());
    retval.insert(retval.end(), v2.begin(), v2.end());
    retval.insert(retval.end(), v3.begin(), v3.end());
    return retval;
}

PassGenerator::PassGenerator(size_t length, bool ul, bool ll, bool num, bool sym) : pass_length(length), use_upper(ul), use_lower(ll), use_numbers(num), use_symbols(sym)
{
    pool.reserve(uppercase_letters.size() + lowercase_letters.size() + numbers.size() + symbols.size());
    GeneratePool();
}

std::string PassGenerator::Generate()
{
    std::string pw = "";
    for (size_t i = 0; i != pass_length; ++i)
    {
        if (pool.size())
        {
            pw.append(1, pool[RandomPoolElement()]);
        }
    }
    return pw;
}

void PassGenerator::GeneratePool()
{
    pool.clear();
    if (use_lower)
    {
        pool.insert(pool.end(), lowercase_letters.begin(), lowercase_letters.end());
    }
    if (use_upper)
    {
        pool.insert(pool.end(), uppercase_letters.begin(), uppercase_letters.end());
    }

    if (use_numbers)
    {
        pool.insert(pool.end(), numbers.begin(), numbers.end());
    }

    if (use_symbols)
    {
        pool.insert(pool.end(), symbols.begin(), symbols.end());
    }

    if (exclude_similar_chars)
    {
        RemoveFromPool(pool, EXCLUDED_CHARS);
    }

    if (exclude_ambiguous_symbols)
    {
        RemoveFromPool(pool, EXCLUDED_SYMBOLS);
    }
}

void PassGenerator::UseUpper(bool b)
{
    use_upper = b;
    GeneratePool();
}

void PassGenerator::UseLower(bool b)
{
    use_lower = b;
    GeneratePool();
}

void PassGenerator::UseNumbers(bool b)
{
    use_numbers = b;
    GeneratePool();
}

void PassGenerator::UseSymbols(bool b)
{
    use_symbols = b;
    GeneratePool();
}

void PassGenerator::ExcludeSimilarChars(bool b)
{
    exclude_similar_chars = b;
    GeneratePool();
}

void PassGenerator::ExcludeAmbiguousSymbols(bool b)
{
    exclude_ambiguous_symbols = b;
    GeneratePool();
}

void PassGenerator::PrintCharPool() const
{
    int i = 0;
    for (const auto& num : pool)
    {
        std::cout << i << " " <<  num << " " << char(num) << std::endl;
        ++i;
    }
}

int PassGenerator::RandomPoolElement()
{
    std::random_device device{"/dev/urandom"};
    std::mt19937 engine(device());
    std::uniform_int_distribution<> distribution(0, pool.size()-1);
    return distribution(engine);
}

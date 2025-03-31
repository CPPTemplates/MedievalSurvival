#pragma once

#include <vector>
#include <numeric>
#include "math/vector/vectn.h"
#include <set>

template <typename First, typename... T>
constexpr bool is_in(First &&first, T &&...t)
{
    return ((first == t) || ...);
}

template <typename T, typename containerType>
constexpr T getSum(const containerType &values)
{
    return std::accumulate(values.begin(), values.end(), T());
}

template <typename T>
constexpr T getAverage(const std::vector<T> &values)
{
    return std::accumulate(values.begin(), values.end(), T()) / values.size();
}

template <typename T, int arraySize>
constexpr int measureSize(T (&)[arraySize])
{
    return arraySize;
}

template <typename listType>
constexpr void deleteAllElements(const listType &list)
{
    for (const auto *const &element : list)
    {
        delete element;
    }
}

template <typename listType, typename T>
constexpr bool arrayContains(const listType &list, const T &value)
{
    return std::find(std::begin(list), std::end(list), value) != std::end(list);
}

template <typename listType, typename T>
auto find(const listType &v, const T &value)
{
    return std::find(std::begin(v), std::end(v), value);
}

template <typename listType, typename T>
size_t indexof(const listType &v, const T &value)
{
    return std::distance(std::begin(v), find(v, value));
}

template <typename T, size_t n>
constexpr void fillAllElements(T (&arrayToFill)[n], const T &value)
{
    std::fill(&arrayToFill[0], &arrayToFill[n], value);
}

template <typename T, size_t n>
constexpr void copyAllElements(const T (&arrayToCopy)[n], T *const &destination)
{
    std::copy(&arrayToCopy[0], &arrayToCopy[n], destination);
}

template <typename sourceType, typename destType>
constexpr void copyAndCast(const sourceType *const &sourceIt, const sourceType *const &endIt, destType *const &destIt)
{
    if constexpr (std::is_same_v<sourceType, destType>)
    {
        std::copy(sourceIt, endIt, destIt);
    }
    else
    {
        std::transform(sourceIt, endIt, destIt,
                       [](const sourceType &value)
                       { return (destType)value; });
    }
}
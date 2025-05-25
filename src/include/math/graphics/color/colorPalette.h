#pragma once
#include "color.h"
// template <typename T, fsize_t channelCount>
// struct colorPaletteTN
//{
//     typedef colortn<T, channelCount> colorType;
//     inline static constexpr colorType transparent = colorType(0, 0);
//     inline static constexpr colorType black = colorType(0, 0, 0);
//     inline static constexpr colorType red = colorType(colorType::maxValue, 0, 0);
//     inline static constexpr colorType green = colorType(0, colorType::maxValue, 0);
//     inline static constexpr colorType blue = colorType(0, 0, colorType::maxValue);
//     inline static constexpr colorType yellow = colorType(colorType::maxValue, colorType::maxValue, 0);
//     inline static constexpr colorType magenta = colorType(colorType::maxValue, 0, colorType::maxValue);
//     inline static constexpr colorType cyan = colorType(0, colorType::maxValue, colorType::maxValue);
//     inline static constexpr colorType purple = colorType(colorType::halfMaxValue, 0, colorType::maxValue);
//     inline static constexpr colorType white = colorType(colorType::maxValue, colorType::maxValue, colorType::maxValue);
//     inline static constexpr colorType gray = colorType(colorType::halfMaxValue, colorType::halfMaxValue, colorType::halfMaxValue);
//     inline static constexpr colorType orange = colorType(colorType::maxValue, colorType::halfMaxValue, 0);
//     inline static constexpr colorType brown = colorType(colorType::halfMaxValue, colorType::quarterMaxValue, colorType::quarterMaxValue);
// };
// typedef colorPaletteTN<colorChannel, defaultColorChannelCount> colorPalette;
//namespace colorPalette
//{
//    template <typename colorType = color>
//    constexpr colorType transparent = colorType(0, 0);
//    template <typename colorType = color>
//    constexpr colorType black = colorType(0, 0, 0);
//    template <typename colorType = color>
//    constexpr colorType red = colorType(colorType::maxValue, 0, 0);
//    template <typename colorType = color>
//    constexpr colorType green = colorType(0, colorType::maxValue, 0);
//    template <typename colorType = color>
//    constexpr colorType blue = colorType(0, 0, colorType::maxValue);
//    template <typename colorType = color>
//    constexpr colorType yellow = colorType(colorType::maxValue, colorType::maxValue, 0);
//    template <typename colorType = color>
//    constexpr colorType magenta = colorType(colorType::maxValue, 0, colorType::maxValue);
//    template <typename colorType = color>
//    constexpr colorType cyan = colorType(0, colorType::maxValue, colorType::maxValue);
//    template <typename colorType = color>
//    constexpr colorType purple = colorType(colorType::halfMaxValue, 0, colorType::maxValue);
//    template <typename colorType = color>
//    constexpr colorType white = colorType(colorType::maxValue, colorType::maxValue, colorType::maxValue);
//    template <typename colorType = color>
//    constexpr colorType gray = colorType(colorType::halfMaxValue, colorType::halfMaxValue, colorType::halfMaxValue);
//    template <typename colorType = color>
//    constexpr colorType orange = colorType(colorType::maxValue, colorType::halfMaxValue, 0);
//    template <typename colorType = color>
//    constexpr colorType brown = colorType(colorType::halfMaxValue, colorType::quarterMaxValue, colorType::quarterMaxValue);
//
//}

template<typename colorType>
class colorPaletteTN
{
public:
    static constexpr colorType transparent = colorType(0, 0);

    static constexpr colorType black = colorType(0, 0, 0);

    static constexpr colorType red = colorType(colorType::maxValue, 0, 0);

    static constexpr colorType green = colorType(0, colorType::maxValue, 0);

    static constexpr colorType blue = colorType(0, 0, colorType::maxValue);

    static constexpr colorType yellow = colorType(colorType::maxValue, colorType::maxValue, 0);

    static constexpr colorType magenta = colorType(colorType::maxValue, 0, colorType::maxValue);

    static constexpr colorType cyan = colorType(0, colorType::maxValue, colorType::maxValue);

    static constexpr colorType purple = colorType(colorType::halfMaxValue, 0, colorType::maxValue);

    static constexpr colorType white = colorType(colorType::maxValue, colorType::maxValue, colorType::maxValue);

    static constexpr colorType gray = colorType(colorType::halfMaxValue, colorType::halfMaxValue, colorType::halfMaxValue);

    static constexpr colorType orange = colorType(colorType::maxValue, colorType::halfMaxValue, 0);

    static constexpr colorType brown = colorType(colorType::halfMaxValue, colorType::quarterMaxValue, colorType::quarterMaxValue);
    //
};
typedef colorPaletteTN<color> colorPalette;
typedef colorPaletteTN<colorf> colorfPalette;
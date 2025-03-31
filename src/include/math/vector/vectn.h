#pragma once

#include "globalFunctions.h"
#include "..\axis.h"
// #include "array/zipiterator.h"
#include "optimization/optimization.h"
#include "..\direction.h"
#include "..\mathFunctions.h"
#include <ranges>
#include <array>
#include "type/conversion.h"

template<typename T, fsize_t n>
struct baseVec {
public:
	union {
		std::array<T, n> axis;
		struct {
			T x;
			T y;
			T z;
		};
	};
#define baseVecConstructor(n)                                    \
    constexpr ~baseVec()                                         \
        requires std::is_trivially_destructible_v<T>             \
    = default;                                                   \
    constexpr ~baseVec()                                         \
    {                                                            \
        for (T & member : axis)                                  \
            member.~T();                                         \
    }                                                            \
    constexpr baseVec(const std::array<T, n> &axis) : axis(axis) \
    {                                                            \
    }

	baseVecConstructor(n)
};

template<typename T>
struct baseVec<T, 0> {
public:
	union {
		std::array<T, 0> axis;
		// no need to declare the empty struct, it'd just issue a warning
	};

	baseVecConstructor(0)
};

template<typename T>
struct baseVec<T, 1> {
public:
	union {
		std::array<T, 1> axis;
		struct {
			T x;
		};
	};

	baseVecConstructor(1)
};

template<typename T>
struct baseVec<T, 2> {
public:
	union {
		std::array<T, 2> axis;
		struct {
			T x;
			T y;
		};
	};

	baseVecConstructor(2)
};
template<typename T>
struct baseVec<T, 4> {
public:
	union {
		std::array<T, 4> axis;
		struct {
			T x;
			T y;
			T z;
			T w;
		};
		//for bitwise comparisons etc
		uint uintValue;
	};

	baseVecConstructor(4)
};

template<typename T, fsize_t n>
// alignment is already a power of two
struct vectn
	: public baseVec<T, n> // alignas(((n == 3 || n == 4) && (alignof(T) <= 0x4)) ? (alignof(T) * 0x4) : math::getNextPowerOf2Multiplied(sizeof(T) * n)) vectn
{
	static constexpr fsize_t axisCount = n;
	typedef T axisType;
	using baseVec<T, n>::axis;
	using baseVec<T, n>::baseVec;
	// template <typename = std::enable_if_t<n>>
	// using x = baseVec<T, n>::x;
	//  using baseVec<T, n>::y;

	constexpr const auto begin() const noexcept {
		return axis.begin();
	}

	constexpr const auto end() const noexcept {
		return axis.end();
	}


	constexpr auto begin() noexcept {
		return axis.begin();
	}

	constexpr auto end() noexcept {
		return axis.end();
	}

	constexpr vectn() noexcept : baseVec<T, n>({}) {
	}

	// fill with this value
	constexpr explicit vectn(const T& initializerValue) : baseVec<T, n>(std::array<T, n>()) {
		//no faster way to do it yet
		for (T& value : axis)
		{
			value = initializerValue;
		}
		// references don'T work in constexpr functions apparently

		// both of these don'T initialize them really.
		// for (auto it = this->begin(); it != this->end(); it++)
		//{
		//	const auto& v = *it;
		//	v = value;
		// }
		// for (auto& val : (*this))
		//{
		//	val = value;
		// }

		// for (auto it = this->begin(); it != this->end(); it++)
		//{
		//	*it = value;
		// }
	}

	constexpr vectn(const T& x, const T& y) requires(axisCount >= 2) : baseVec<T, n>(std::array<T, n>({ x, y })) {}


	constexpr vectn(const T& x, const T& y, const T& z) requires(axisCount >= 3) : baseVec<T, n>({ x, y, z }) {}


	constexpr vectn(const T& x, const T& y, const T& z, const T& w) requires(axisCount >= 4) : baseVec<T, n>({ x, y, z, w }) {}

	//constexpr vectn(std::array<T, axisCount> values) : baseVec<T, n>(values) {
	//}

	constexpr vectn switchAxes() const {
		vectn result = vectn(axis[1], axis[0]);
		if constexpr (axisCount > 2) {
			for (fsize_t i = 2; i < axisCount; i++) {
				result.axis[i] = axis[i];
			}
		}
		return result;
	}

	template<typename t2, fsize_t axisCount2>
	//the input is convertible without loss of data, so make the conversion implicit
		requires non_narrowing<t2, T>
	constexpr vectn(const vectn<t2, axisCount2>& in) : baseVec<T, n>(std::array<T, n>()) {
		constexpr fsize_t minimum = math::minimum(axisCount, axisCount2);

		for (fsize_t i = 0; i < minimum; i++)
		{
			//implicit conversion
			axis[i] = in.axis[i];
		}
	}

	template<typename t2, fsize_t axisCount2>
		requires narrowing_conversion<t2, T>
	explicit constexpr vectn(const vectn<t2, axisCount2>& in) : baseVec<T, n>(std::array<T, n>()) {
		constexpr fsize_t minimum = math::minimum(axisCount, axisCount2);

		for (fsize_t i = 0; i < minimum; i++)
		{
			//explicit conversion
			axis[i] = (T)in.axis[i];
		}
	}

	constexpr vectn(
		const typename std::enable_if<(axisCount > 0), vectn<T, math::maximum(axisCount - 1,
			(fsize_t)1)>>::type& in,
		const T& valueToAdd) : vectn(in) {
		axis[axisCount - 1] = valueToAdd;
	}

	template<std::integral indexType>
	constexpr T& operator[](const indexType& axisIndex) {
		assumeInRelease((axisIndex < (indexType)axisCount) && (axisIndex >= 0));
		return this->axis[axisIndex];
	}

	template<std::integral indexType>
	constexpr const T& operator[](const indexType& axisIndex) const {
		assumeInRelease((axisIndex < (indexType)axisCount) && (axisIndex >= 0));
		return this->axis[axisIndex];
	}

	constexpr T& operator[](caxisID& axisIndex) {
		return operator[]((fsize_t)axisIndex);
	}

	constexpr const T& operator[](caxisID& axisIndex) const {
		return operator[]((fsize_t)axisIndex);
	}

	addMemberName(getX, (*this)[0], axisCount > 0);

	addMemberName(getY, (*this)[1], axisCount > 1);

	addMemberName(getZ, (*this)[2], axisCount > 2);
	// addMemberName(w, (*this)[3])

	constexpr T sum() const {
		T result = axis[0];
		for (fsize_t i = 1; i < axisCount; i++) {
			result += axis[i];
		}
		return result;
	}

	constexpr T volume() const {
		T result = axis[0];
		for (fsize_t i = 1; i < axisCount; i++) {
			result *= axis[i];
		}
		return result;
	}

	constexpr T lengthSquared() const {
		T result = math::squared(axis[0]);
		for (fsize_t i = 1; i < axisCount; i++) {
			result += math::squared(axis[i]);
		}
		return result;
	}

	inline T length() const {
		return (T)std::sqrt(lengthSquared());
	}

	inline vectn normalized() const {
		vectn result = *this;
		result.normalize();
		return result;
	}

	inline void normalize() {
		const auto& squaredLength = lengthSquared();
		if (squaredLength == 0)return;
		else {
			*this /= std::sqrt(squaredLength);
		}
		//*this *= (T)math::fastInverseSqrt((float)lengthSquared());
		//
		//// to stop getrotation() from returning nan
		//for (auto& axisIt : *this) {
		//	axisIt = math::clamp(axisIt, (T)-1, (T)1);
		//}
	}

	// can be 0, 90,180, 270
	constexpr vectn rotateXY(cfsize_t& angle) const {

		cint& sina = math::sinDegrees(angle);
		cint& cosa = math::cosDegrees(angle);
		using baseVec<T, n>::x;
		using baseVec<T, n>::y;

		return vectn(
			x * cosa + y * sina,
			x * -sina + y * cosa);
	}

	// 0 -> 0, 1
	// pi / 2 -> 1,0
	// pi -> 0, -1
	// pi * 1.5 -> -1,0
	inline static vectn getrotatedvector(const fp& yaw) {
		// rotate vector over y axis(pitch)
		return vectn(sin(yaw), cos(yaw));
	}

	inline static vectn getrotatedvector(const T& yaw, const T& pitch) {
		// rotate vector over y axis(pitch)
		const T& cosp = cos(pitch), sinp = sin(pitch), siny = sin(yaw), cosy = cos(yaw);
		return vectn(siny * cosp, cosy * cosp, sinp);
	}

	// this vector has to be normalized!
	// clockwise
	//+y = up
	// x 0, y 1 : 0
	inline fp getRotation() const {
		fp angle = asin(getX());
		if (getY() < 0) {
			angle = math::PI - angle;
		}
		if (angle < 0) {
			angle += math::PI2;
		}
		// x 0, y 1 : 0
		return angle;
	}

	constexpr vectn absolute() const {
		vectn result = vectn();
		for (fsize_t i = 0; i < axisCount; i++) {
			result[i] = abs(axis[i]);
		}
		return result;
	}

	// axis has to be normalized!
	inline static vectn rotate(const vectn& vec, const vectn& axis, const T& angle) {
		return (cos(angle) * vec) + (sin(angle) * cross(axis, vec));
	}

	inline static constexpr T dot(const vectn& lhs, const vectn& rhs) {
		T result = T();
		for (fsize_t i = 0; i < axisCount; i++) {
			result += lhs[i] * rhs[i];
		}
		return result;
	}

	inline static vectn cross(const vectn& lhs, const vectn& rhs) {
		return vectn(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
	}

	constexpr T maximum() const {
		return axis[minimumIndex([](auto&& lhs, auto&& rhs) { return lhs > rhs; })];
	}

	constexpr T minimum() const {
		return axis[minimumIndex([](auto&& lhs, auto&& rhs) { return lhs < rhs; })];
	}

	// comparefunction should return true if left < right
	template<typename compareFunction>
	constexpr fsize_t minimumIndex(compareFunction&& function) const {
		fsize_t result = 0;
		for (fsize_t i = 1; i < axisCount; i++) {
			if (function(axis[i], axis[result])) {
				result = i;
			}
		}
		return result;
	}

	// operators

	constexpr bool operator==(const vectn& other) const {
		for (fsize_t i = 0; i < axisCount; i++) {
			if (axis[i] != other[i]) {
				return false;
			}
		}
		return true;
	}

	constexpr bool operator!=(const vectn& other) const {
		return !operator==(other);
	}

	constexpr vectn operator-() const {
		vectn result = vectn();
		for (fsize_t i = 0; i < axisCount; i++) {
			result[i] = -axis[i];
		}
		return result;
	}

	// used for sorting
	// returns true if pos0 is 'lower' than pos1
	// orders: 0,0 1,0 0,1 1,1
	constexpr bool operator<(const vectn& other) const noexcept {
		for (fsize_t i = axisCount - 1;; i--) {
			if (axis[i] < other[i]) {
				return true;
			}
			else if (axis[i] > other[i]) {
				return false;
			}
			else if (i == 0) {
				break;
			}
		}
		return false;
	}

	//#pragma optimize ("", on)

#define newMacro(type, copySize) vectn<type COMMA n> result = vectn<type COMMA n>();

	addOperators(newMacro, vectn, wrap(vectn<t2, n>), constexpr)

#undef newMacro
		//#pragma optimize ("", off)
};

addTemplateTypes(vec)

// inline constexpr static std::array<int, 2> arr = std::array<int, 2>({ 2 });
// inline constexpr static vec2 vec2Test = vec2(1);
// inline constexpr static fp valueTest = vec2Test.sum();
// inline constexpr static vectn<fp, 2> lessThan1 = vec2Test - vec2(0, 4);
// inline constexpr static cvect3<fp> vectest = { 0, 1 };

constexpr veci2 directionVectors2D[directionCount<2>()]{
		cveci2(-1, 0),
		cveci2(1, 0),
		cveci2(0, -1),
		cveci2(0, 1),
};
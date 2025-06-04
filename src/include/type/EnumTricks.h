#pragma once
#include <vector>
template <typename T>
struct EnumIterator
{
public:
	T current;

	using UnderlyingType = std::underlying_type_t<T>;

	// iterator_traits:
	using value_type = T;
	using difference_type = std::make_signed_t<UnderlyingType>;
	using iterator_concept = std::bidirectional_iterator_tag;
	using iterator_category = std::bidirectional_iterator_tag;
	using reference = T;   // we return enum by value
	using pointer = void; // not used (no operator->)

	constexpr EnumIterator() = default;

	constexpr EnumIterator(const T& current) : current(current) {}

	// ++prefix operator
	constexpr EnumIterator& operator++()
	{
		current = (T)((UnderlyingType)current + 1);
		return *this;
	}
	// --prefix operator
	constexpr EnumIterator& operator--()
	{
		current = (T)((UnderlyingType)current - 1);
		return *this;
	}

	// postfix++ operator
	constexpr EnumIterator operator++(int)
	{
		const EnumIterator copy = *this;
		++(*this);
		return copy;
	}
	// postfix-- operator
	constexpr EnumIterator operator--(int)
	{
		const EnumIterator copy = *this;
		--(*this);
		return copy;
	}
	constexpr bool operator==(const EnumIterator& other) const
	{
		return other.current == current;
	}
	// this operator is used for checking if the end is reached, so it has to be fast
	// we shouldn't detect if we're iterating over the same data
	constexpr bool operator!=(const EnumIterator& other) const
	{
		return other.current != current;
	}
	constexpr const T operator*() const
	{
		return current;
	}
};

//assuming all values are an enum!
template <typename T>
	requires(std::is_enum_v<T>)
struct EnumArray {
	T from;
	T to;
	using UnderlyingType = std::underlying_type_t<T>;
	//works only if an enum has a count member
	EnumArray():from((T)0), to(T::count){
	}
	EnumArray(const T& from, const T& to) :from(from), to(to) {
	}
	EnumArray(const UnderlyingType& from, const UnderlyingType& to) :from((T)from), to((T)to) {
	}
	EnumIterator<T> begin() const {
		return EnumIterator<T>(from);
	}
	EnumIterator<T> end() const {
		return EnumIterator<T>(to);
	}
	EnumIterator<T> rbegin() const {
		return EnumIterator<T>((T)((UnderlyingType)to - 1));
	}
	EnumIterator<T> rend() const {
		return EnumIterator<T>((T)((UnderlyingType)from - 1));
	}
};


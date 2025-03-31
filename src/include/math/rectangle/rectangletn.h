#pragma once
#include "math/direction.h"
#include "math/vector\vectn.h"
#include "math/graphics/alignment.h"

template <typename T, fsize_t axisCount>
struct rectIteratortn;

template <typename T, fsize_t axisCount>
struct baseRect
{
	union
	{
		struct
		{
			vectn<T, axisCount> pos0;
			vectn<T, axisCount> size;
		};
		struct
		{
			T x;
			T y;
			T w;
			T h;
		};
	};
#define rectConstructor(axisCount)                                                                     \
	constexpr baseRect(vectn<T, axisCount> pos0, vectn<T, axisCount> size) : pos0(pos0), size(size) {} \
	constexpr ~baseRect()                                                                              \
	{                                                                                                  \
		pos0.~vectn<T, axisCount>();                                                                   \
		size.~vectn<T, axisCount>();                                                                   \
	}
	rectConstructor(axisCount) constexpr baseRect(const T &x, const T &y, const T &w, const T &h) : pos0(vect2<T>(x, y)), size(vect2<T>(w, h))
	{
	}
};
template <typename T>
struct baseRect<T, 0>
{
	union
	{
		struct
		{
			vectn<T, 1> pos0;
			vectn<T, 1> size;
		};
		// no need to declare the empty struct, it'd just issue a warning
	};
	rectConstructor(0)
	// constexpr baseRect() = default;
	// constexpr baseRect() : pos0(vectn<T, axisCount>(), size(vectn<T, axisCount>())) {}
};
template <typename T>
struct baseRect<T, 1>
{
	union
	{
		struct
		{
			vectn<T, 1> pos0;
			vectn<T, 1> size;
		};
		struct
		{
			T x;
			T w;
		};
	};
	rectConstructor(1)
	// constexpr baseRect() = default;
	// constexpr baseRect() : pos0(vectn<T, axisCount>(), size(vectn<T, axisCount>())) {}
};

// #cannot make an union out of this, because the union would need to vary in size depending on the axisCount parameter
template <typename T, fsize_t axisCount>
struct rectangletn : baseRect<T, axisCount>
{
	using base = baseRect<T, axisCount>;
	using vec = vectn<T, axisCount>;
	using cvec = cvectn<T, axisCount>;
	using base::pos0;
	using base::size;

	addMemberName(getX, pos0.getX(), axisCount > 0)
		addMemberName(getY, pos0.getY(), axisCount > 1)
			addMemberName(getW, size.getX(), axisCount > 0)
				addMemberName(getH, size.getY(), axisCount > 1)
		//
		using base::baseRect;
	constexpr rectangletn() : base(vec(), vec()) {}
	// constexpr rectangletn(cvectn<T, axisCount> &pos0, cvectn<T, axisCount> &size)
	//{
	//	this->pos0 = pos0;
	//	this->size = size;
	// }
	explicit constexpr rectangletn(cvectn<T, axisCount> &size) : base(vec(), size) {}

	template <typename t2, fsize_t axisCount2>
	// the input is convertible without loss of data, so make the conversion implicit
		requires non_narrowing<t2, T>
	constexpr rectangletn(const rectangletn<t2, axisCount2> &in) : base((vec)in.pos0, (vec)in.size)
	{
	}

	template <typename t2, fsize_t axisCount2>
		requires narrowing_conversion<t2, T>
	explicit constexpr rectangletn(const rectangletn<t2, axisCount2> &in) : base((vec)in.pos0, (vec)in.size)
	{
	}

	constexpr rectIteratortn<T, axisCount> begin() const;
	constexpr rectIteratortn<T, axisCount> end() const;

	constexpr vec pos10() const
	{
		return vec2(pos0.x + size.x, pos0.y);
	}
	constexpr vec pos01() const
	{
		return vec2(pos0.x, pos0.y + size.y);
	}
	constexpr vec pos1() const
	{
		return pos0 + size;
	}
	constexpr bool contains(const vec &pos) const
	{
		for (fsize_t i = 0; i < axisCount; i++)
		{
			if ((pos[i] < pos0[i]) || (pos[i] >= (pos0[i] + size[i])))
			{
				return false;
			}
		}

		return true;
	}

	constexpr void expand(const T &border)
	{
		pos0 -= border;
		size += border * 2;
	}

	constexpr void expandToContain(const rectangletn &rect)
	{
		const vec &oldPos11 = pos1();
		const vec &rectPos11 = rect.pos1();
		for (fsize_t i = 0; i < axisCount; i++)
		{
			if (rect.pos0[i] < pos0[i])
			{
				size[i] += pos0[i] - rect.pos0[i];
				pos0[i] = rect.pos0[i];
			}
			else if (rectPos11[i] > oldPos11[i])
			{
				size[i] += rectPos11[i] - oldPos11[i];
			}
		}
	}

	// the rectangle will not exceed the borders; else it will return false.
	constexpr bool cropClientRect(rectangletn &toCrop) const
	{
		vec toCropPos1 = toCrop.pos1();
		vec borderPos1 = pos1();

		for (fsize_t i = 0; i < axisCount; i++)
		{
			if (toCrop.pos0[i] < pos0[i])
			{
				if (toCropPos1[i] <= pos0[i])
				{
					return false;
				}
				toCrop.size[i] += (toCrop.pos0[i] - pos0[i]);
				toCrop.pos0[i] = pos0[i];
				// pos1 is not modified
			}

			if (toCropPos1[i] > borderPos1[i])
			{
				if (toCrop.pos0[i] >= borderPos1[i])
				{
					return false;
				}
				toCrop.size[i] = borderPos1[i] - toCrop.pos0[i];
			}
		}
		return true;
	}
	constexpr rectangletn cropClientRectUnsafe(rectangletn toCrop) const
	{

		vec toCropPos1 = toCrop.pos1();
		vec borderPos1 = pos1();

		for (fsize_t i = 0; i < axisCount; i++)
		{
			if (toCrop.pos0[i] < pos0[i])
			{
				toCrop.size[i] += (toCrop.pos0[i] - pos0[i]);
				toCrop.pos0[i] = pos0[i];
				// pos1 is not modified
			}

			if (toCropPos1[i] > borderPos1[i])
			{
				toCrop.size[i] = borderPos1[i] - toCrop.pos0[i];
			}
		}
		// safety measures are not in place. instead of returning a boolean, we return the result
		return toCrop;
	}

	constexpr static T getCollisionArea(rectangletn rect0, const rectangletn &rect1)
	{
		if (rect1.cropClientRect(rect0))
		{
			return rect0.size.volume();
		}
		else
		{
			return 0;
		}
	}

	constexpr rectangletn expanded(const T &border) const
	{
		rectangletn r = *this;
		r.expand(border);
		return r;
	}
	constexpr rectangletn touchingBorder(const rectangletn &borders, const directionID &direction) const
	{
		rectangletn r = *this;
		caxisID &axis = getAxis(direction);
		if (isPositive(direction))
		{
			r.pos0[axis] = borders.pos0[axis] + borders.size[axis];
		}
		else
		{
			r.pos0[axis] = borders.pos0[axis] - size[axis];
		}
		return r;
	}
	constexpr vec rectPos0Centered(const vec &innerRectSize) const
	{
		vec result = vec();

		for (fsize_t i = 0; i < axisCount; i++)
		{
			result[i] = pos0[i] + ((size[i] - innerRectSize[i]) / 2);
		}
		return result;
	}
	constexpr rectangletn<T, axisCount> rectCentered(const vec &innerRectSize) const
	{
		return rectangletn<T, axisCount>(rectPos0Centered(innerRectSize), innerRectSize);
	}
	constexpr rectangletn<T, axisCount> rectCenteredAround(const vec &newCenterPosition) const
	{
		return rectangletn<T, axisCount>(newCenterPosition + (size / (T)2), size);
	}
	constexpr void moveToCenter(const rectangletn &outerRect)
	{
		pos0 = outerRect.rectPos0Centered(size);
	}
	constexpr rectangletn switchAxes()
	{
		return rectangletn(pos0.switchAxes(), size.switchAxes());
	}
	constexpr vec getCenter() const
	{
		return pos0 + vec(size) / (T)2;
	}

	inline static constexpr rectangletn fromOppositeCorners(const vec &corner0, const vec &corner1)
	{
		vec pos0 = vec();
		vec pos1 = vec();

		for (size_t i = 0; i < axisCount; i++)
		{
			pos0[i] = math::minimum(corner0[i], corner1[i]);
			pos1[i] = math::maximum(corner0[i], corner1[i]);
		}
		return rectangletn(pos0, pos1 - pos0);
	}

	constexpr T getAlignedY(const T rectangleHeight, const verticalAlignment &alignment)
	{
		using base::h;
		using base::y;
		switch (alignment)
		{
		case verticalAlignment::bottom:
			return y;
		case verticalAlignment::middle:
			return y + (h + rectangleHeight) / 2;
		case verticalAlignment::top:
			return y + h - rectangleHeight;
		}
	}
	constexpr T getAlignedX(const T rectangleWidth, const horizontalAlignment &alignment)
	{
		using base::w;
		using base::x;

		switch (alignment)
		{
		case horizontalAlignment::left:
			return x;
		case horizontalAlignment::middle:
			return x + (w + rectangleWidth) / 2;
		case horizontalAlignment::right:
			return x + w - rectangleWidth;
		}
	}
	constexpr rectangletn rotatedInRectangle(const rectangletn &outerRect, const directionID &direction) const
	{
		rectangletn rotatedRectangle = *this;
		cbool positive = isPositive(direction);
		caxisID &axis = getAxis(direction);

		if (!positive)
		{
			rotatedRectangle.y = (outerRect.y + outerRect.h - rotatedRectangle.h) - (rotatedRectangle.y - outerRect.y);
		}

		if (axis == axisID::x)
		{
			return rotatedRectangle.switchAxes();
		}
		return rotatedRectangle;
	}

	inline static constexpr rectangletn unitSquare() { return rectangletn(0, 0, 1, 1); }

	constexpr bool operator==(const rectangletn &other) const
	{
		return (pos0 == other.pos0) && (size == other.size);
	}
	constexpr bool operator!=(const rectangletn &other) const
	{
		return !operator==(other);
	}

	template <typename t2>
	constexpr rectangletn multiplied(const t2 &multiplier) const
	{
		return rectangletn(pos0 * multiplier, size * multiplier);
	}
};

addTemplateTypes(rectangle)

	// An iterator over a vector of vectors.

	// https://stackoverflow.com/questions/1784573/iterator-for-2d-vector

	template <typename T, fsize_t axisCount>
	struct rectIteratortn
{
public:
	vectn<T, axisCount> pos = vectn<T, axisCount>();
	// rectangletn<T, axisCount> rect = rectangletn<T, axisCount>();
	vectn<T, axisCount> pos0 = vectn<T, axisCount>();
	vectn<T, axisCount> pos1 = vectn<T, axisCount>();

	constexpr rectIteratortn() = default;

	constexpr rectIteratortn(crectangletn<T, axisCount> &rect, cvectn<T, axisCount> &pos) : pos(pos), pos0(rect.pos0), pos1(rect.pos1()) {}

	// ++prefix operator
	constexpr rectIteratortn &operator++()
	{
		for (size_t i = 0; i < axisCount; i++)
		{
			const T& incrementedAxis = pos[i] + 1;
			if (incrementedAxis < pos1[i]) {
				pos[i] = incrementedAxis;
				return *this;
			}
			else {
				pos[i] = pos0[i];
			}
		}
		//reached the end
		pos[axisCount - 1] = pos1[axisCount - 1];
		return *this;
	}
	// --prefix operator
	constexpr rectIteratortn &operator--()
	{
		for (size_t i = 0; i < axisCount; i++)
		{
			if (pos[i] > pos0[i]) {
				pos[i]--;
				return *this;
			}
			else {
				pos[i] = pos1[i] - 1;
			}
		}
		//got before the start
		pos[axisCount - 1] = pos0[axisCount - 1] - 1;
	}

	// postfix++ operator
	constexpr rectIteratortn operator++(int)
	{
		const rectIteratortn copy = *this;
		++(*this);
		return copy;
	}
	// postfix-- operator
	constexpr rectIteratortn operator--(int)
	{
		const rectIteratortn copy = *this;
		--(*this);
		return copy;
	}
	constexpr bool operator==(const rectIteratortn &other) const
	{
		return other.pos == pos;
	}
	// this operator is used for checking if the end is reached, so it has to be fast
	// we shouldn'T detect if we're iterating over the same rectangle
	constexpr bool operator!=(const rectIteratortn &other) const
	{
		return other.pos != pos;
	}
	constexpr const vectn<T, axisCount> operator*() const
	{
		if constexpr (isDebugging)
		{
			if (pos[axisCount - 1] == pos1[axisCount - 1])
			{
				throw "out of rectangle";
			}
		}
		return pos;
	}
};

template <typename T, fsize_t axisCount>
constexpr rectIteratortn<T, axisCount> rectangletn<T, axisCount>::begin() const
{
	return rectIteratortn<T, axisCount>(*this, pos0);
}

template <typename T, fsize_t axisCount>
constexpr rectIteratortn<T, axisCount> rectangletn<T, axisCount>::end() const
{
	vectn<T, axisCount> endPos = pos0;

	endPos[axisCount - 1] += size[axisCount - 1];

	return rectIteratortn<T, axisCount>(*this, endPos);
}
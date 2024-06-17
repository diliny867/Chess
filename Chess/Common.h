#pragma once

#include <cstdint>
#include <ostream>

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef float f32;
typedef double f64;

template <typename T>
struct Vec2 {
	T x = 0;
	T y = 0;
	inline bool operator==(const Vec2& rhs) {
		return x == rhs.x && y == rhs.y;
	}
	inline bool operator!=(const Vec2& rhs) {
		return !(*this == rhs);
	}

};
template <typename>
std::ostream& operator<<(std::ostream& os, const Vec2<f32>& vec) {
	os<<f32(vec.x)<<", "<<f32(vec.y);
	return os;
}
template <typename>
std::ostream& operator<<(std::ostream& os, const Vec2<f64>& vec) {
	os<<f64(vec.x)<<", "<<f64(vec.y);
	return os;
}
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec) {
	os<<i32(vec.x)<<", "<<i32(vec.y);
	return os;
}

typedef Vec2<i8> i8Vec2;
typedef Vec2<u8> u8Vec2;
typedef Vec2<i16> i16Vec2;
typedef Vec2<u16> u16Vec2;
typedef Vec2<i32> i32Vec2;
typedef Vec2<u32> u32Vec2;
typedef Vec2<i64> i64Vec2;
typedef Vec2<u64> u64Vec2;
typedef Vec2<f32> f32Vec2;
typedef Vec2<f64> f64Vec2;

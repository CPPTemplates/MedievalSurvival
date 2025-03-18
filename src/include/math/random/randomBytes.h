#include <random>
#include <math/graphics/color/color.h>
using random_bytes_engine = std::independent_bits_engine<
	std::default_random_engine, CHAR_BIT, unsigned char>;

template<typename byteIterator>
void generateRandomBytes(byteIterator targetPtr, byteIterator endPtr, std::mt19937& generator) {
	while (true) {
		// Generate a 32-bit random number.
		uint32_t rnd = generator();
		// Extract the four bytes.
		for (int i = 0; i < 4; ++i) {
			if (targetPtr >= endPtr)return;
			unsigned char byte = static_cast<unsigned char>((rnd >> (8 * i)) & 0xFF);
			*targetPtr++ = byte;
		}
	}
}

colorb randomColor(std::mt19937& generator) {
	colorb result = colorb();
	generateRandomBytes(result.begin(), result.end() - 1, generator);
	return result;
}
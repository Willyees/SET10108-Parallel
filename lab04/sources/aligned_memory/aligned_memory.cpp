#include <immintrin.h>
#include <iostream>

// Union can flip between the types within.
union v4
{
	__m128 v;
	float a[4];
};

int main() {

	//declare single 128bit value aligned to 16 bytes (16*8 = 128 bits)
	alignas(16) v4 x;
	//we can treat x as a collection of 4 floats
	//or other combination of values for 128bits
	x.a[0] = 10.0f;
	x.a[1] = 20.0f;
	x.a[2] = 30.0f;
	x.a[3] = 40.0f;
	
	// We can print out individual values
	std::cout << "Original values" << std::endl;
	for (size_t i = 0; i < 4; ++i)
		std::cout << x.a[i] << std::endl;

	// Declared a second 128-bit value alignted to 16 bytes (size of 128-bits)
	alignas(16) v4 y;
	y.a[0] = 10.0f;
	y.a[1] = 20.0f;
	y.a[2] = 30.0f;
	y.a[3] = 40.0f;
	//add y to x
	x.v = _mm_add_ps(x.v, y.v);
	//print out individual values
	std::cout << "New values" << std::endl;
	for (size_t i = 0; i < 4; ++i)
		std::cout << x.a[i] << std::endl;
	
	//create array of 100 floats (32 bits), aligned to 4 bytes (32 bits)
	float* data = (float*)_aligned_malloc(100 * sizeof(float), 4);
	//access just link an array
	std::cout << data[0] << std::endl;

	//create array of 100 floats 128bits values aligned to 16 bytes (128bits).
	v4* big_data = (v4*)_aligned_malloc(100 * sizeof(v4), 16);
	//access just like an array of __m128
	std::cout << big_data[0].a[0] << std::endl;

	//free data. Dealing at C level!
	_aligned_free(data);
	_aligned_free(big_data);

	std::cin.get();
	return 0;
}
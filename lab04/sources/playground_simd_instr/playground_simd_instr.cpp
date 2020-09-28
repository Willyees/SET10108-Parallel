#include <immintrin.h>
#include <iostream>

union v4
{
	__m128 v;
	float a[4];

};

int main()
{
	alignas(16) v4 x;
	alignas(16) v4 y;
	for (int i = 0; i < 4; ++i)
	{
		x.a[i] = i;
		y.a[i] = 2;
	}
	//x.a[2] = 1;
	alignas(16) v4 result;
	result.v = _mm_add_ps(x.v, y.v);
	result.v = _mm_div_ps(x.v, y.v);
	for (int i = 0; i < 4; ++i)
	{
		std::cout << result.a[i] << std::endl;
	}
	std::cin.get();
	return 0;
}
#include <immintrin.h>
#include <random>
#include <iostream>

union v4
{
	__m128 v;
	float a[4];
};

void generate_data(v4* data, size_t n) {
	//create random engine
	std::random_device r;
	std::default_random_engine e(r());

	for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < 4; ++j)
			data[i].a[j] = e();
}

void nomalise_vectors(v4* data, v4* result, size_t n) {
	//normalise the vectors
	for (size_t i = 0; i < n; ++i) {
		//square each component - multiply the vectors by themselves
		result[i].v = _mm_mul_ps(data[i].v, data[i].v);
		//sum components
		result[i].v = _mm_hadd_ps(result[i].v, result[i].v);
		result[i].v = _mm_hadd_ps(result[i].v, result[i].v);
		//calculate reciprocal square root of the values
		// 1.0 / sqrt(value) -> length of vector
		result[i].v = _mm_rsqrt_ps(result[i].v);
		//multiply rest by original data which is the same as
		//divide each component by the vector length
		result[i].v = _mm_mul_ps(data[i].v, result[i].v);

	}
}

// Check the first 100 results
void check_results(v4 *data, v4 *result)
{
	// Check first 100 values
	for (size_t i = 0; i < 100; ++i)
	{
		// Calculate the length of the vector
		float l = 0.0f;
		// Square each component and add to l
		for (size_t j = 0; j < 4; ++j)
			l += powf(data[i].a[j], 2.0f);
		// Get sqrt of the length
		l = sqrtf(l);
		// Now check that the individual results
		for (size_t j = 0; j < 4; ++j)
			std::cout << data[i].a[j] / l << " : " << result[i].a[j] << std::endl;
	}
}

int main() {
	v4* data = (v4*)_aligned_malloc(sizeof(v4) * 100, 4);
	generate_data(data, 100);
	v4* result = (v4*)_aligned_malloc(sizeof(v4) * 100, 4);
	nomalise_vectors(data, result, 100);
	check_results(data, result);

	std::cin.get();
}
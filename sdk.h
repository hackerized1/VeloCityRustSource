#pragma once
#include "math.h"
#include "structures.h"
#include "driver/include.h"
#include <intrin.h>
#include <string>

#undef min
#undef max

inline float normalize(const float& f)
{
	return std::remainderf(f, 360.f);
}

template<typename T>
inline T min(const T& a, const T& b)
{
	return a < b ? a : b;
}
template<typename T>
inline T max(const T& a, const T& b)
{
	return a > b ? a : b;
}
template <typename T>
inline T clamp(const T& val, const T& lower, const T& upper)
{
	return max(lower, min(val, upper));
}

namespace sdk
{
	math::vector3 getTransformPos(uintptr_t transform)
	{
		__m128 result;

		const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
		const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
		const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

		structures::transformReadOnly pTransformAccessReadOnly = driver::rpm<structures::transformReadOnly>(transform + 0x38);

		structures::transform transformData = driver::rpm<structures::transform>(pTransformAccessReadOnly.pTransformData + 0x18);

		SIZE_T sizeMatriciesBuf = sizeof(math::matrix3x4) * pTransformAccessReadOnly.index + sizeof(math::matrix3x4);
		SIZE_T sizeIndicesBuf = sizeof(int) * pTransformAccessReadOnly.index + sizeof(int);

		PVOID pMatriciesBuf = malloc(sizeMatriciesBuf);
		PVOID pIndicesBuf = malloc(sizeIndicesBuf);

		if (pMatriciesBuf && pIndicesBuf)
		{
			SIZE_T single_chunk_matricies = sizeMatriciesBuf / 5;
			driver::rpm_raw(transformData.pTransformArray, (uint64_t)pMatriciesBuf, sizeMatriciesBuf);
			driver::rpm_raw(transformData.pTransformIndices, (uint64_t)pIndicesBuf, sizeIndicesBuf);
			result = *(__m128*)((uintptr_t)pMatriciesBuf + 0x30 * pTransformAccessReadOnly.index);
			int transformIndex = *(int*)((uintptr_t)pIndicesBuf + 0x4 * pTransformAccessReadOnly.index);

			while (transformIndex >= 0)
			{
				math::matrix3x4 matrix34 = *(math::matrix3x4*)((uintptr_t)pMatriciesBuf + 0x30 * transformIndex);

				__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));	// xxxx
				__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));	// yyyy
				__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));	// zwxy
				__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));	// wzyw
				__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));	// zzzz
				__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));	// yxwy
				__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

				result = _mm_add_ps(
					_mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
							tmp7)), *(__m128*)(&matrix34.vec0));

				transformIndex = *(int*)((uintptr_t)pIndicesBuf + 0x4 * transformIndex);
			}

			free(pMatriciesBuf);
			free(pIndicesBuf);
		}



		return math::vector3{ result.m128_f32[0], result.m128_f32[1], result.m128_f32[2] };
	}

	std::string getString(const structures::str& string)
	{
		std::string buffer;

		if (string.buffer && string.size > 1 && string.size < 32)
		{
			for (int x = 0; x < string.size * 2; x += 2)
			{

				if (!string.buffer[x]) return buffer;

				buffer += string.buffer[x];
			}
		}

		return buffer;
	}

	math::vector3 calculateAngle(const math::vector3& source, const math::vector3& destination)
	{
		math::vector3 difference = source - destination;


		math::vector3 angle = math::vector3{
			std::asin(difference.y / difference.length()) * 57.2957795131f,
			-std::atan2(difference.x, -difference.z) * 57.2957795131f,
			0.f };
		return angle;
	}

	math::vector4 calculateQuat(math::vector3 euler)
	{
		math::vector4 result{};

		auto yaw = (euler.z * 0.01745329251f) * 0.5f;
		auto sy = std::sin(yaw);
		auto cy = std::cos(yaw);

		auto roll = (euler.x * 0.01745329251f) * 0.5f;
		auto sr = std::sin(roll);
		auto cr = std::cos(roll);

		constexpr auto sp = 0.f;
		constexpr auto cp = 1.f;

		result.x = cy * sr * cp - sy * cr * sp;
		result.y = cy * cr * sp + sy * sr * cp;
		result.z = sy * cr * cp - cy * sr * sp;
		result.w = cy * cr * cp + sy * sr * sp;

		return result;
	}

	math::vector3 multiplyQuad(math::vector4 quat, math::vector3 vec)
	{
		float num = quat.x * 2.f;
		float num2 = quat.y * 2.f;
		float num3 = quat.z * 2.f;
		float num4 = quat.x * num;
		float num5 = quat.y * num2;
		float num6 = quat.z * num3;
		float num7 = quat.x * num2;
		float num8 = quat.x * num3;
		float num9 = quat.y * num3;
		float num10 = quat.w * num;
		float num11 = quat.w * num2;
		float num12 = quat.w * num3;
		math::vector3 result;
		result.x = (1.f - (num5 + num6)) * vec.x + (num7 - num12) * vec.y + (num8 + num11) * vec.z;
		result.y = (num7 + num12) * vec.x + (1.f - (num4 + num6)) * vec.y + (num9 - num10) * vec.z;
		result.z = (num8 - num11) * vec.x + (num9 + num10) * vec.y + (1.f - (num4 + num5)) * vec.z;
		return result;
	}

	float normalizeAngle(float flAng)
	{

		if (!std::isfinite(flAng)) return 0.0f;


		return std::remainder(flAng, 360.0f);
	}

	void clampAngles(math::vector3& vecAng)
	{
		vecAng.x = std::max(-89.0f, std::min(89.0f, normalizeAngle(vecAng.x)));
		vecAng.y = normalizeAngle(vecAng.y);
		vecAng.z = 0.0f;

	}


	uint64_t getKlass(const char* name, int index = 1)
	{
		MutateStart();

		auto base = globals::gameAssemblyMod;
		auto dos_header = driver::rpm<IMAGE_DOS_HEADER>(base);
		auto data_header = driver::rpm<IMAGE_SECTION_HEADER>(base + dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS64) + (3 * 40));
		auto next_section = driver::rpm<IMAGE_SECTION_HEADER>(base + dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS64) + (4 * 40));
		auto data_size = next_section.VirtualAddress - data_header.VirtualAddress;
		int occurances = 0;
		if (strcmp((char*)data_header.Name, xorstr(".data"))) return 0;

		for (uint64_t offset = data_size; offset > 0; offset -= 8) {
			char klass_name[0x100] = { 0 };
			auto klass = driver::rpm<uint64_t>(base + data_header.VirtualAddress + offset);
			if (klass == 0) continue;
			auto name_pointer = driver::rpm<uint64_t>(klass + 0x10);
			if (name_pointer == 0) continue;
			driver::rpm_raw(name_pointer, (uint64_t)klass_name, sizeof(klass_name));
			if (klass_name[0] != 0)
			{
				if (fnv::hash_runtime(klass_name) == fnv::hash(name))
				{
					occurances += 1;
					if (occurances == index)
						return data_header.VirtualAddress + offset;
					else
						continue;


				}
			}
			continue;
		}
		return 0;


		MutateEnd();
	}

}
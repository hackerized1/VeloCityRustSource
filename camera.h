#pragma once
#include "driver/include.h"
#include "globals.h"

namespace camera
{
	//bool fetch()
	//{
	//	uintptr_t last_object = driver::rpm<uintptr_t>(globals::classes::gameObjectManager);
	//	if (!last_object) return 0;
	//
	//	for (int i = 1; i < 1000; i++)
	//	{
	//		uintptr_t this_obj = driver::rpm<uintptr_t>(globals::classes::gameObjectManager + (i * 0x8));
	//		if (!this_obj) return 0;
	//
	//		if (this_obj == last_object) break;
	//
	//		uintptr_t buf = driver::rpm<uintptr_t>(this_obj + 0x10);
	//		if (!buf) return 0;
	//
	//		if (driver::rpm<short>(buf + 0x54) == 5)
	//		{
	//			buf = driver::rpm<UINT64>(buf + 0x30);
	//
	//			globals::objects::camera = driver::rpm<uintptr_t>(buf + 0x18);
	//			if (!globals::objects::camera) return 0;
	//
	//			return 1;
	//		}
	//	}
	//
	//	return 0;
	//}

	//bool fetch()
	//{
	//	uintptr_t new_camera = 0;
	//
	//	int camera_count = driver::rpm<int>(globals::classes::cameraManager + 0x10);
	//
	//	for (int i = 0; i < camera_count; i++)
	//	{
	//		uintptr_t camera = driver::rpm<uintptr_t>(globals::classes::cameraManager + (i * 0x8));
	//		if (!camera) continue;
	//
	//		uintptr_t camera_object = driver::rpm<uintptr_t>(camera + 0x30);
	//		uintptr_t name_pointer = driver::rpm<uintptr_t>(camera_object + 0x60);
	//
	//		char name_buffer[32] = {};
	//		driver::rpm_raw(name_pointer, (uintptr_t)&name_buffer, sizeof(name_buffer));
	//		name_buffer[31] = '\0';
	//
	//		if (!strcmp(name_buffer, xorstr("Main Camera")))
	//		{
	//			camera_object = driver::rpm<UINT64>(camera_object + 0x30);
	//			if (camera_object)
	//			{
	//				new_camera = driver::rpm<uintptr_t>(camera_object + 0x18);
	//				if (new_camera) break;
	//			}
	//		}
	//
	//	}
	//
	//	if (globals::objects::camera = new_camera)
	//		return true;
	//	else
	//		return false;
	//}

	bool fetch()
	{
		globals::objects::camera = driver::rpm<uintptr_t>(globals::classes::MainCamera);
		if (!globals::objects::camera) return false;

		globals::objects::camera = driver::rpm<uintptr_t>(globals::objects::camera + 0x10);
		if (!globals::objects::camera) return false;

		return 1;
	}

	math::matrix4x4 update()
	{
		return driver::rpm<math::matrix4x4>(globals::objects::camera + 0xDC);
	}

	math::vector3 get_pos()
	{
		return driver::rpm<math::vector3>(globals::objects::camera + 0x42c);
	}

	bool w2s(math::vector3& world, math::vector2* screen)
	{
		math::matrix4x4 matrix = update().transpose();

		const math::vector3 translation = { matrix[3][0], matrix[3][1], matrix[3][2] };

		const math::vector3 up = { matrix[1][0], matrix[1][1], matrix[1][2] };
		const math::vector3 right = { matrix[0][0], matrix[0][1], matrix[0][2] };


		const auto w = translation.dot_product(world) + matrix[3][3];

		if (w < 0.098f) return 0;

		const auto x = right.dot_product(world) + matrix[0][3];
		const auto y = up.dot_product(world) + matrix[1][3];


		screen->x = globals::gameSize.x / 2 * (1.f + x / w);
		screen->y = globals::gameSize.y / 2 * (1.f - y / w);

		return 1;
	}
}
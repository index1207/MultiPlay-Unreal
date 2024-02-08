#pragma once

#include "generated/Struct.gen.hpp"

class Utility
{
public:
	static gen::Vec3 MakeStatus(const FVector& Vector);

	static gen::Status MakeStatus(const gen::Vec3& Vec3, float Yaw, float Speed);

	static gen::Status MakeStatus(const FVector& Vector, const float Yaw, const float Speed);
	
	static FVector MakeFVector(const gen::Vec3& Vec3);
};

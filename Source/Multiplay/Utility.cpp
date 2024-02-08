#include "Utility.h"

gen::Vec3 Utility::MakeStatus(const FVector& Vector)
{
	gen::Vec3 Vec3;
	Vec3.x = Vector.X;
	Vec3.y = Vector.Y;
	Vec3.z = Vector.Z;
	return Vec3;
}

gen::Status Utility::MakeStatus(const gen::Vec3& Vec3, const float Yaw, const float Speed)
{
	gen::Status Status;
	Status.location = Vec3;
	Status.yaw = Yaw;
	Status.speed = Speed;
	return Status;
}

gen::Status Utility::MakeStatus(const FVector& Vector, const float Yaw, const float Speed)
{
	return MakeStatus(MakeStatus(Vector), Yaw, Speed);
}

FVector Utility::MakeFVector(const gen::Vec3& Vec3)
{
	FVector Vector;
	Vector.X = Vec3.x;
	Vector.Y = Vec3.y;
	Vector.Z = Vec3.z;
	return Vector;
}
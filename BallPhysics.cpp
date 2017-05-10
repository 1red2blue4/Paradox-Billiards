#pragma once
#include "BallPhysics.h"

BallPhysics::BallPhysics()
{
	netForce = vector3();
	velocity = vector3();
	speedCap = 1.5f;
	gravity = vector3();
	freedom = 0.035;
	name = "default";
}

BallPhysics::~BallPhysics()
{
	//NO MEMORY ALLOCATED
}

void BallPhysics::SetGravity(vector3 yfrc)
{
	gravity = yfrc;
}

vector3 BallPhysics::GetGravity()
{
	return gravity;
}

void BallPhysics::SetBounce(vector3 bounceFrc)
{
	//TODO
	bounceForce = bounceFrc;
}

vector3 BallPhysics::GetBounce()
{
	return bounceForce;
}

void BallPhysics::AddForce(vector3 frc)
{
	netForce += frc;
}

vector3 BallPhysics::ApplyForces(vector3 objectPosition)
{

	velocity += netForce;
	if (velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z > speedCap*speedCap)
	{
		float vectorDistance = sqrt(velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z);
		velocity = vector3(velocity.x*speedCap / vectorDistance, velocity.y*speedCap / vectorDistance, velocity.z*speedCap / vectorDistance);
	}
	objectPosition += velocity;

	return objectPosition;
}

void BallPhysics::SetVelocity(vector3 vel)
{
	velocity = vel;
}

vector3 BallPhysics::GetVelocity()
{
	return velocity;
}

void BallPhysics::ZeroVelocity()
{
	SetVelocity(vector3());
}

vector3 BallPhysics::GetForce(void)
{
	return netForce;
}

void BallPhysics::SetForce(vector3 frc)
{
	netForce = frc;
}

void BallPhysics::SetName(String nm) { name = nm; }
String BallPhysics::GetName() { return name; }
void BallPhysics::SetType(String typ) { type = typ; }
String BallPhysics::GetType() { return type; }
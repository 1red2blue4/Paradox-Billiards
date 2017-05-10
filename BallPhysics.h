#pragma once
#include "RE\ReEng.h"

class BallPhysics
{
private:
	vector3 netForce;
	vector3 velocity;
	float speedCap;
	vector3 gravity;
	vector3 bounceForce;
	//freedom is not currently implemented
	float freedom;
	std::vector<vector3> activeForces;
	//the type of the physicsObject
	String type;
	//the name of the physicsObject
	String name;

public:
	BallPhysics();
	~BallPhysics();
	void SetName(String nm);
	String GetName();
	void SetType(String typ);
	String GetType();
	void SetGravity(vector3 yfrc);
	vector3 GetGravity();
	void SetBounce(vector3 bounceFrc);
	vector3 GetBounce();
	void AddForce(vector3 frc);
	//void RemoveForce(vector3 frc);
	//void SetForce(vector3 frc);
	vector3 GetForce(void);
	void SetForce(vector3 frc);
	vector3 ApplyForces(vector3 object);
	void ZeroVelocity();
	void SetVelocity(vector3 vel);
	vector3 GetVelocity();
};
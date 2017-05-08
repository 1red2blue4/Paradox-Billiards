#pragma once
#include "BallPhysics.h"
#include "RE\ReEng.h"

class PhysicsManager
{
private:
	int numPhysicObjs;
	std::vector<BallPhysics*> physicsObjects;
	static PhysicsManager* pMngr;

	PhysicsManager();
	PhysicsManager(PhysicsManager const& other);
	PhysicsManager& operator=(PhysicsManager const& other);

public:
	static PhysicsManager* GetInstance(void);
	static void ReleaseInstance(void);

	/*
	//make a new physics object
	static void MakePhysicsObject(matrix4 obj);

	//set all objects' gravity
	static void SetGravityAll(float force);
	
	//set some objects' gravity
	static void SetGravityAt(int index, float force);
	
	//get a net force of a particular index
	static vector3 GetForceAt(int index);

	//set a force at a particular index
	static void SetForceAt(int index, vector3 force);

	//add a force to all
	static void AddForceAll(vector3 force);

	//add a force at an index
	static void AddForceAt(int index, vector3 force);
	
	//apply all forces
	//static void ApplyAllForces(std::vector<matrix4&> allObjectMatrices);

	static int GetNumObjs(void);
	static std::vector<BallPhysics*> GetAllPhysObj(void);
	*/
};

#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
	numPhysicObjs = 0;
	physicsObjects = std::vector<BallPhysics*>();
}

PhysicsManager::PhysicsManager(PhysicsManager const& other)
{
	numPhysicObjs = other.numPhysicObjs;
	physicsObjects = other.physicsObjects;
}

PhysicsManager& PhysicsManager::operator=(PhysicsManager const& other)
{
	if (this != &other)
	{
		numPhysicObjs = other.numPhysicObjs;
		physicsObjects = other.physicsObjects;
	}

	return *this;
}

PhysicsManager* PhysicsManager::GetInstance(void)
{
	if (pMngr == nullptr)
	{
		pMngr = new PhysicsManager();
	}
	return pMngr;
}

void PhysicsManager::ReleaseInstance(void)
{
	if (pMngr != nullptr)
	{
		delete pMngr;
		pMngr = nullptr;
	}
}

/*
//make a new physics object
void PhysicsManager::MakePhysicsObject(matrix4 obj)
{

}

//set all objects' gravity
void PhysicsManager::SetGravityAll(float force) 
{
	for (int i = 0; i < pMngr->GetNumObjs(); i++)
	{
		pMngr->GetAllPhysObj()[i]->SetGravity(force);
	}
}

//set some objects' gravity
void PhysicsManager::SetGravityAt(int index, float force)
{
	pMngr->GetAllPhysObj()[index]->SetGravity(force);
}

//get a net force of a particular index
vector3 PhysicsManager::GetForceAt(int index)
{
	return pMngr->GetAllPhysObj()[index]->GetForce();
}

//set a force at a particular index
void PhysicsManager::SetForceAt(int index, vector3 force)
{
	pMngr->GetAllPhysObj()[index]->SetForce(force);
}

//add a force to all
void PhysicsManager::AddForceAll(vector3 force)
{
	for (int i = 0; i < pMngr->GetNumObjs(); i++)
	{
		pMngr->GetAllPhysObj()[i]->AddForce(force);
	}
}

//add a force at an index
void PhysicsManager::AddForceAt(int index, vector3 force)
{
	pMngr->GetAllPhysObj()[index]->AddForce(force);
}

//apply all forces
void PhysicsManager::ApplyAllForces(std::vector<matrix4&> allObjectMatrices)
{
	for (int i = 0; i < pMngr->GetNumObjs(); i++)
	{
		//pMngr->GetAllPhysObj()[i]->ApplyForce(GetAllPhysObj()[i]->GetForce(), allObjectMatrices[i]);
	}
}

int PhysicsManager::GetNumObjs(void) { return pMngr->numPhysicObjs; }
std::vector<BallPhysics*> PhysicsManager::GetAllPhysObj(void) { return pMngr->physicsObjects; }
*/
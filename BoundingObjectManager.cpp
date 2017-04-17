#include "BoundingObjectManager.h"

BoundingObjectManager* BoundingObjectManager::boMngr = nullptr;

BoundingObjectManager::BoundingObjectManager()
{
	numBoundingObjects = 0;
	allBoundingObjects = std::vector<MyBoundingObjectClass*>();
	shouldRender = std::vector<bool>();
}

BoundingObjectManager::BoundingObjectManager(BoundingObjectManager const& other)
{
	numBoundingObjects = other.numBoundingObjects;
	allBoundingObjects = other.allBoundingObjects;
	shouldRender = other.shouldRender;
}

BoundingObjectManager& BoundingObjectManager::operator=(BoundingObjectManager const& other)
{
	if (this != &other)
	{
		numBoundingObjects = other.numBoundingObjects;
		allBoundingObjects = other.allBoundingObjects;
		shouldRender = other.shouldRender;
	}

	return *this;
}

BoundingObjectManager* BoundingObjectManager::GetInstance()
{
	if (boMngr == nullptr)
	{
		boMngr = new BoundingObjectManager();
	}
	return boMngr;
}

void BoundingObjectManager::ReleaseInstance()
{
	if (boMngr != nullptr)
	{
		delete boMngr;
		boMngr = nullptr;
	}
}

void BoundingObjectManager::MakeNewBoundingObject(std::vector<vector3> vertexList)
{
	MyBoundingObjectClass* myBO = new MyBoundingObjectClass(vertexList);
	boMngr->shouldRender.push_back(true);
	boMngr->numBoundingObjects++;
	boMngr->allBoundingObjects.push_back(myBO);
	//delete myBO;
}

void BoundingObjectManager::ToggleBoxVisibility(int index)
{
	boMngr->GetBO(index)->ToggleBoxVisibility();
}

void BoundingObjectManager::SetVisibility(int index, bool set)
{
	boMngr->GetBO(index)->SetVisibility(set);
}

void BoundingObjectManager::SetColor(int index, vector3 color)
{
	boMngr->GetBO(index)->SetBoxColor(color);
}

void BoundingObjectManager::SetAllMatrices(std::vector<matrix4> matrix)
{
	for (int i = 0; i < boMngr->GetNumBO(); i++)
	{
		boMngr->GetAllBO()[i]->SetModelMatrix(matrix[i]);
	}
}

void BoundingObjectManager::RenderAll()
{
	for (int i = 0; i < boMngr->GetNumBO(); i++)
	{
		boMngr->GetBO(i)->Render();
	}
}

void BoundingObjectManager::Render(MyBoundingObjectClass* toRender)
{
	toRender->Render();
}

void BoundingObjectManager::CheckRotation(quaternion rotationArc, int index)
{
	boMngr->GetBO(index)->GetBoundingBox()->FindOrthoBox(rotationArc);
}

void BoundingObjectManager::CheckAllCollisions()
{
	for (int i = 0; i < boMngr->GetNumBO(); i++)
	{
		boMngr->GetAllBO()[i]->SetBoxColor(REGREEN);
		boMngr->GetAllBO()[i]->SetSphereColor(REGREEN);
	}

	for (int i = 0; i < boMngr->GetNumBO(); i++)
	{
		for (int j = 0; j < boMngr->GetNumBO(); j++)
		{
			if (i != j && boMngr->GetAllBO()[i]->IsBoxColliding(boMngr->GetAllBO()[j]->GetBoundingBox()))
			{
				boMngr->GetAllBO()[i]->SetBoxColor(RERED);
				boMngr->GetAllBO()[j]->SetBoxColor(RERED);
			}
			if (i != j && boMngr->GetAllBO()[i]->IsSphereColliding(boMngr->GetAllBO()[j]->GetBoundingSphere()))
			{
				boMngr->GetAllBO()[i]->SetSphereColor(RERED);
				boMngr->GetAllBO()[j]->SetSphereColor(RERED);
			}
		}
	}
}

int BoundingObjectManager::GetNumBO() { return boMngr->numBoundingObjects; }
std::vector<MyBoundingObjectClass*> BoundingObjectManager::GetAllBO() { return boMngr->allBoundingObjects; }
MyBoundingObjectClass* BoundingObjectManager::GetBO(int index) { return boMngr->allBoundingObjects[index]; }
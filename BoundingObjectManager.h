#pragma once
#include "MyBoundingObjectClass.h"
#include "RE\ReEng.h"

class BoundingObjectManager
{
private:

	int numBoundingObjects;
	std::vector<MyBoundingObjectClass*> allBoundingObjects;
	std::vector<bool> shouldRender;

	static BoundingObjectManager* boMngr;
	BoundingObjectManager();
	BoundingObjectManager(BoundingObjectManager const& other);
	BoundingObjectManager& operator=(BoundingObjectManager const& other);

	
	

public:
	static BoundingObjectManager* GetInstance(void);
	static void ReleaseInstance(void);
	static void MakeNewBoundingObject(std::vector<vector3> vertexList);
	static void ToggleBoxVisibility(int index);
	static int GetNumBO();
	static std::vector<MyBoundingObjectClass*> GetAllBO();
	static MyBoundingObjectClass* GetBO(int index);
	static void SetColor(int index, vector3 color);
	static void SetVisibility(int index, bool set);
	static void SetAllMatrices(std::vector<matrix4> matrices);
	static void CheckRotation(quaternion rotationArc, int index);
	static void RenderAll();
	static void Render(MyBoundingObjectClass* toRender);
	static void CheckAllCollisions();

};
#ifndef __OCTREE_H_
#define __OCTREE_H_

#include "RE\ReEng.h"
#include "MyBOClass.h"

class Octree
{
private:

	std::vector<MyBOClass*> allBoundingObjects;
	Octree* parent = nullptr;
	Octree* allChildren[8];
	MyBOClass* octreeBO;
	int depthLevel;
	vector3 m_v3Max;
	vector3 m_v3Min;
	vector3 m_centerLocal;

	MeshManagerSingleton* m_pMeshMngr = nullptr;

	vector3 m_v3MaxGlobal;
	vector3 m_v3MinGlobal;
	vector3 m_centerGlobal;

	vector3 m_v3Color;

	matrix4 m_m4ToWorld;

public:

	//set allBoundingObjs
	//find m_v3Max and m_v3Min
	//find center of all objects
	//set depthLevel to numIterations

	//subdivide it into 8 more Octree's with (numIterations - 1)
	//set this as children's parent
	Octree(std::vector<matrix4> encasedObjs, std::vector<MyBOClass*> objsBOs, int numIterations);
	Octree(std::vector<vector3> vertexList, std::vector<MyBOClass*> objsBOs, int numIterations);
	~Octree();

	//start with a fresh octree
	void ZeroOut();

	void Render();
	void Update();
	void SetModelMatrix(matrix4 m_4ToWorld);
	void PushAllBOToOctree();
	void MakeChildren(std::vector<MyBOClass*> objsBOs);
	void CheckCollisions();

	void SetParent(Octree* myParent);
	Octree* GetParent();
	Octree* GetChild(int index);


};

#endif //__OCTREE_H__
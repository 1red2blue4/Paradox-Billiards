#include "Octree.h"

void Octree::ZeroOut()
{
	allBoundingObjects = std::vector<MyBOClass*>();
	parent = nullptr;
	
	for (int i = 0; i < 8; i++)
	{
		allChildren[i] = nullptr;
	}
	
	depthLevel = 0;
	m_v3Max = vector3();
	m_v3Min = vector3();
	m_centerLocal = vector3();

	m_v3MaxGlobal = vector3();
	m_v3MinGlobal = vector3();
	m_centerGlobal = vector3();

	m_m4ToWorld = IDENTITY_M4;

	m_pMeshMngr = MeshManagerSingleton::GetInstance();
}

//construtor for the eldest octree
Octree::Octree(std::vector<matrix4> encasedObjs, std::vector<MyBOClass*> objsBOs, int numIterations)
{
	//initialize
	if (this != nullptr)
	{
		ZeroOut();

		//set depth level
		depthLevel = numIterations;


		//start max and min algorithm
		m_v3Max = vector3(encasedObjs[0][3][0], encasedObjs[0][3][1], encasedObjs[0][3][2]); //first matrix4's x, y, and z
		m_v3Min = vector3(encasedObjs[0][3][0], encasedObjs[0][3][1], encasedObjs[0][3][2]); //first matrix4's x, y, and z

		for (int i = 0; i < encasedObjs.size(); i++)
		{
			//if the youngest generation, add the objects to allBoundingObjects
			if (depthLevel == 0 /*&& objsBOs[i]->IsColliding()*/)
			{
				allBoundingObjects.push_back(objsBOs[i]);
			}
			matrix4 temp = encasedObjs[i];
			//max and min algorithm
			if (temp[3][0] > m_v3Max.x)
			{
				m_v3Max.x = temp[3][0] + objsBOs[i]->GetMax().x;
			}
			if (temp[3][1] > m_v3Max.y)
			{
				m_v3Max.y = temp[3][1] + objsBOs[i]->GetMax().y;
			}
			if (temp[3][2] > m_v3Max.z)
			{
				m_v3Max.z = temp[3][2] + objsBOs[i]->GetMax().z;
			}

			if (temp[3][0] < m_v3Min.x)
			{
				m_v3Min.x = temp[3][0] - objsBOs[i]->GetMin().x;
			}
			if (temp[3][1] < m_v3Min.y)
			{
				m_v3Min.y = temp[3][1] - objsBOs[i]->GetMin().y;
			}
			if (temp[3][2] < m_v3Min.z)
			{
				m_v3Min.z = temp[3][2] - objsBOs[i]->GetMin().z;
			}
		}

		m_centerLocal = (m_v3Max + m_v3Min) / 2.0f;

		m_v3MaxGlobal = m_v3Max;
		m_v3MinGlobal = m_v3Min;
		m_centerGlobal = m_centerLocal;

		octreeBO = new MyBOClass({
			vector3(m_v3Max.x, m_v3Max.y, m_v3Max.z),
			vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z),
			vector3(m_v3Max.x, m_v3Min.y, m_v3Max.z),
			vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z),
			vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z),
			vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z),
			vector3(m_v3Min.x, m_v3Min.y, m_v3Max.z),
			vector3(m_v3Min.x, m_v3Min.y, m_v3Min.z) });

		if (depthLevel != 0)
		{
			MakeChildren(objsBOs);
		}
	}
}

Octree::Octree(std::vector<vector3> vertexList, std::vector<MyBOClass*> objsBOs, int numIterations)
{
	if (this != nullptr)
	{
		ZeroOut();

		depthLevel = numIterations;

		m_v3Max = vertexList[0];
		m_v3Min = vertexList[0];

		for (int i = 0; i < vertexList.size(); i++)
		{
			if (vertexList[i].x > m_v3Max.x)
			{
				m_v3Max.x = vertexList[i].x;
			}
			if (vertexList[i].y > m_v3Max.y)
			{
				m_v3Max.y = vertexList[i].y;
			}
			if (vertexList[i].z > m_v3Max.z)
			{
				m_v3Max.z = vertexList[i].z;
			}
			if (vertexList[i].x < m_v3Min.x)
			{
				m_v3Min.x = vertexList[i].x;
			}
			if (vertexList[i].y < m_v3Min.y)
			{
				m_v3Min.y = vertexList[i].y;
			}
			if (vertexList[i].z < m_v3Min.z)
			{
				m_v3Min.z = vertexList[i].z;
			}
		}

		m_centerLocal = (m_v3Max + m_v3Min) / 2.0f;

		m_v3MaxGlobal = m_v3Max;
		m_v3MinGlobal = m_v3Min;
		m_centerGlobal = m_centerLocal;

		octreeBO = new MyBOClass({
			vector3(m_v3Max.x, m_v3Max.y, m_v3Max.z),
			vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z),
			vector3(m_v3Max.x, m_v3Min.y, m_v3Max.z),
			vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z),
			vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z),
			vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z),
			vector3(m_v3Min.x, m_v3Min.y, m_v3Max.z),
			vector3(m_v3Min.x, m_v3Min.y, m_v3Min.z) });

		if (depthLevel != 0)
		{
			MakeChildren(objsBOs);
		}

		if (depthLevel == 0)
		{
			for (int i = 0; i < objsBOs.size(); i++)
			{
				if (octreeBO->IsColliding(objsBOs[i]))
				{
					allBoundingObjects.push_back(objsBOs[i]);
				}
			}
			
		}
	}
}

void Octree::CheckCollisions()
{
	if (depthLevel == 0)
	{
		for (int i = 0; i < allBoundingObjects.size(); i++)
		{
			allBoundingObjects[i]->SetColor(REWHITE);
		}

		for (int i = 0; i < allBoundingObjects.size(); i++)
		{
			for (int j = i; j < allBoundingObjects.size(); j++)
			{
				if (i != j && allBoundingObjects[i]->IsColliding(allBoundingObjects[j]))
				{
					allBoundingObjects[i]->SetColor(RERED);
					allBoundingObjects[j]->SetColor(RERED);
				}
			}
		}
	}
	
	
}

void Octree::MakeChildren(std::vector<MyBOClass*> objsBOs)
{
	allChildren[0] = new Octree({
		vector3(m_v3Max.x, m_v3Max.y, m_v3Max.z),
		vector3(m_v3Max.x, m_v3Max.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Max.y, m_v3Max.z),
		vector3(m_v3Max.x, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Max.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Max.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Max.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3(m_v3Max.x, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
	}, objsBOs, depthLevel - 1);

	allChildren[1] = new Octree({
		vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z),
		vector3(m_v3Max.x, m_v3Max.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Max.y, m_v3Min.z),
		vector3(m_v3Max.x, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Min.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Min.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Max.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3(m_v3Max.x, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
	}, objsBOs, depthLevel - 1);

	allChildren[2] = new Octree({
		vector3(m_v3Max.x, m_v3Min.y, m_v3Max.z),
		vector3(m_v3Max.x, m_v3Min.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Min.y, m_v3Max.z),
		vector3(m_v3Max.x, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Max.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Max.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Min.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3(m_v3Max.x, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
	}, objsBOs, depthLevel - 1);

	allChildren[3] = new Octree({
		vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z),
		vector3(m_v3Max.x, m_v3Min.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Min.y, m_v3Min.z),
		vector3(m_v3Max.x, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Min.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Min.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Min.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3(m_v3Max.x, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
	}, objsBOs, depthLevel - 1);

	allChildren[4] = new Octree({
		vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z),
		vector3(m_v3Min.x, m_v3Max.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Max.y, m_v3Max.z),
		vector3(m_v3Min.x, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Max.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Max.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Max.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3(m_v3Min.x, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
	}, objsBOs, depthLevel - 1);

	allChildren[5] = new Octree({
		vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z),
		vector3(m_v3Min.x, m_v3Max.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Max.y, m_v3Min.z),
		vector3(m_v3Min.x, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Min.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Min.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Max.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3(m_v3Min.x, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
	}, objsBOs, depthLevel - 1);

	allChildren[6] = new Octree({
		vector3(m_v3Min.x, m_v3Min.y, m_v3Max.z),
		vector3(m_v3Min.x, m_v3Min.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Min.y, m_v3Max.z),
		vector3(m_v3Min.x, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Max.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Max.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Min.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3(m_v3Min.x, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
	}, objsBOs, depthLevel - 1);

	allChildren[7] = new Octree({
		vector3(m_v3Min.x, m_v3Min.y, m_v3Min.z),
		vector3(m_v3Min.x, m_v3Min.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Min.y, m_v3Min.z),
		vector3(m_v3Min.x, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Min.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, m_v3Min.z),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, m_v3Min.y, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3(m_v3Min.x, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
		vector3((m_v3Max.x + m_v3Min.x) / 2.0f, (m_v3Max.y + m_v3Min.y) / 2.0f, (m_v3Max.z + m_v3Min.z) / 2.0f),
	}, objsBOs, depthLevel - 1);

	allChildren[0]->SetParent(this);
	allChildren[1]->SetParent(this);
	allChildren[2]->SetParent(this);
	allChildren[3]->SetParent(this);
	allChildren[4]->SetParent(this);
	allChildren[5]->SetParent(this);
	allChildren[6]->SetParent(this);
	allChildren[7]->SetParent(this);
}

void Octree::SetModelMatrix(matrix4 a_m4ToWorld)
{
	//If there are no changes in the Model Matrix there is no need
	//of doing further calculations
	if (m_m4ToWorld == a_m4ToWorld)
		return;

	m_m4ToWorld = a_m4ToWorld;
	//Calculate the vertex that makes the Object
	vector3 v3Corner[8];
	v3Corner[0] = vector3(m_v3Min.x, m_v3Min.y, m_v3Min.z);
	v3Corner[1] = vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z);
	v3Corner[2] = vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z);
	v3Corner[3] = vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z);

	v3Corner[4] = vector3(m_v3Min.x, m_v3Min.y, m_v3Max.z);
	v3Corner[5] = vector3(m_v3Max.x, m_v3Min.y, m_v3Max.z);
	v3Corner[6] = vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z);
	v3Corner[7] = vector3(m_v3Max.x, m_v3Max.y, m_v3Max.z);
	//Get vectors in global space
	for (uint nVertex = 0; nVertex < 8; nVertex++)
	{
		v3Corner[nVertex] = vector3(m_m4ToWorld * vector4(v3Corner[nVertex], 1.0f));
	}

	//Get the max and min out of the list
	m_v3MaxGlobal = m_v3MinGlobal = v3Corner[0];
	for (uint nVertex = 1; nVertex < 8; nVertex++)
	{
		if (m_v3MinGlobal.x > v3Corner[nVertex].x) //If min is larger than current
			m_v3MinGlobal.x = v3Corner[nVertex].x;
		else if (m_v3MaxGlobal.x < v3Corner[nVertex].x)//if max is smaller than current
			m_v3MaxGlobal.x = v3Corner[nVertex].x;

		if (m_v3MinGlobal.y > v3Corner[nVertex].y) //If min is larger than current
			m_v3MinGlobal.y = v3Corner[nVertex].y;
		else if (m_v3MaxGlobal.y < v3Corner[nVertex].y)//if max is smaller than current
			m_v3MaxGlobal.y = v3Corner[nVertex].y;

		if (m_v3MinGlobal.z > v3Corner[nVertex].z) //If min is larger than current
			m_v3MinGlobal.z = v3Corner[nVertex].z;
		else if (m_v3MaxGlobal.z < v3Corner[nVertex].z)//if max is smaller than current
			m_v3MaxGlobal.z = v3Corner[nVertex].z;
	}
	m_centerGlobal = (m_v3MinGlobal + m_v3MaxGlobal) / 2.0f;

}

void Octree::Render()
{
	m_pMeshMngr->AddCubeToRenderList(glm::translate(IDENTITY_M4, m_centerGlobal) *
		glm::scale(vector3(m_v3Max - m_v3Min)), REGREEN, WIRE);

	if (depthLevel == 0)
	{
		for (int i = 0; i < allBoundingObjects.size(); i++)
		{
			m_pMeshMngr->AddCubeToRenderList(allBoundingObjects[i]->GetModelMatrix() *
				glm::scale(vector3(allBoundingObjects[i]->GetMax() - allBoundingObjects[i]->GetMin())), allBoundingObjects[i]->GetColor(), WIRE);
		}
	}
	

	for (int i = 0; i < 8; i++)
	{
		if (allChildren[i] != nullptr)
		{
			allChildren[i]->Render();
		}
	}
}

void Octree::Update()
{
	CheckCollisions();

	for (int i = 0; i < 8; i++)
	{
		if (allChildren[i] != nullptr)
		{
			allChildren[i]->Update();
		}
	}
}

void Octree::PushAllBOToOctree()
{

}

Octree::~Octree()
{
	for (int i = 0; i < 8; i++)
	{
		if (allChildren[i] != nullptr && depthLevel != 0)
		{
			allChildren[i]->~Octree();
			allChildren[i] = nullptr;
		}
		else if (allChildren[i] != nullptr)
		{
			allChildren[i] = nullptr;
		}
	}
	MeshManagerSingleton::ReleaseInstance();
	for (int i = 0; i < allBoundingObjects.size(); i++)
	{
		if (allBoundingObjects[i] != nullptr)
		{
			allBoundingObjects[i] = nullptr;
		}
		//delete allBoundingObjects[i];
	}

	if (octreeBO != nullptr)
	{
		octreeBO = nullptr;
	}
	delete octreeBO;

	if (parent != nullptr)
	{
		parent = nullptr;
	}
	delete parent;
	
	//delete allChildren;
	

}

void Octree::SetParent(Octree* myParent) { parent = myParent; }
Octree* Octree::GetParent() { return parent; }
Octree* Octree::GetChild(int index) { return allChildren[index]; }
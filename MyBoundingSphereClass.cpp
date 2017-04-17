#include "MyBoundingSphereClass.h"

MyBoundingSphereClass::MyBoundingSphereClass(std::vector<vector3> vertexList)
{
	m_bColliding = false;
	m_fRadius = 0.0f;
	m_v3CenterGlobal = vector3(0.0f);
	bool m_bColliding = false;

	if (vertexList.size() < 1)
		return;

	vector3 v3Min = vertexList[0];
	vector3 v3Max = vertexList[0];

	for (int i = 1; i < vertexList.size(); i++)
	{
		if (v3Min.x > vertexList[i].x)
		{
			v3Min.x = vertexList[i].x;
		}
		else if (v3Max.x < vertexList[i].x)
		{
			v3Max.x = vertexList[i].x;
		}

		if (v3Min.y > vertexList[i].y)
		{
			v3Min.y = vertexList[i].y;
		}
		else if (v3Max.y < vertexList[i].y)
		{
			v3Max.y = vertexList[i].y;
		}

		if (v3Min.z > vertexList[i].z)
		{
			v3Min.z = vertexList[i].z;
		}
		else if (v3Max.z < vertexList[i].z)
		{
			v3Max.z = vertexList[i].z;
		}
	}

	m_v3CenterLocal = m_v3CenterGlobal = (v3Max + v3Min) / 2.0f;

	m_fRadius = glm::distance(m_v3CenterGlobal, v3Max);

	m_pMeshMngr = MeshManagerSingleton::GetInstance();
}

void MyBoundingSphereClass::RenderSphere()
{
	/*
	m_v3Color = REGREEN;
	if (m_bColliding)
	{
		m_v3Color = RERED;
	}
	if (true == m_bColliding)
		m_v3Color = RERED;
	*/

	if (GetVisibility())
	{
		m_pMeshMngr->AddSphereToRenderList(
			glm::translate(m_v3CenterGlobal) *
			glm::scale(vector3(m_fRadius) * 2.0f), m_v3Color, WIRE);
	}
}
void MyBoundingSphereClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
	if (m_m4ToWorld == a_m4ToWorld)
		return;

	m_m4ToWorld = a_m4ToWorld;
	m_v3CenterGlobal = vector3(m_m4ToWorld * vector4(m_v3CenterLocal, 1.0f));
}

bool MyBoundingSphereClass::IsColliding(MyBoundingSphereClass* other)
{
	float fDistance = glm::distance(this->m_v3CenterGlobal, other->m_v3CenterGlobal);
	float fRadiiSum = this->m_fRadius + other->m_fRadius;
	if (fDistance < fRadiiSum)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MyBoundingSphereClass::SetColliding(bool input) { m_bColliding = input; }
void MyBoundingSphereClass::SetCenterLocal(vector3 input) { m_v3CenterLocal = input; }
void MyBoundingSphereClass::SetCenterGlobal(vector3 input) { m_v3CenterGlobal = input; }
void MyBoundingSphereClass::SetRadius(float input) { m_fRadius = input; }
bool MyBoundingSphereClass::GetColliding(void) { return m_bColliding; }
vector3 MyBoundingSphereClass::GetCenterLocal(void) { return m_v3CenterLocal; }
vector3 MyBoundingSphereClass::GetCenterGlobal(void) { return m_v3CenterGlobal; }
float MyBoundingSphereClass::GetRadius(void) { return m_fRadius; }
matrix4 MyBoundingSphereClass::GetModelMatrix(void) { return m_m4ToWorld; }
vector3 MyBoundingSphereClass::GetColor(void) { return m_v3Color; }
void MyBoundingSphereClass::SetColor(vector3 input) { m_v3Color = input; }
bool MyBoundingSphereClass::GetVisibility(void) { return visible; }
void MyBoundingSphereClass::SetVisibility(bool input) { visible = input; }
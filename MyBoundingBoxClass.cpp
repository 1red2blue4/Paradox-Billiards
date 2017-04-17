#include "MyBoundingBoxClass.h"

MyBoundingBoxClass::MyBoundingBoxClass(std::vector<vector3> vertexList)
{
	m_v3CenterLocal = vector3(0.0f); //center of the sphere in local 
	m_v3CenterNewLocal = vector3(0.0f);
	m_v3CenterGlobal = vector3(0.0f); //center of the sphere in global space
	m_m4ToWorld = IDENTITY_M4; //matrix that takes you from local to global space
	m_m4ToNewWorld = IDENTITY_M4;
	m_pMeshMngr = nullptr; //for drawing the sphere

	//initialize values
	m_v3Size = vector3(0.0f);
	m_v3NewSize = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);
	m_v3NewMin = vector3(0.0f);
	m_v3NewMax = vector3(0.0f);
	m_v3MinGlobal = vector3(0.0f);
	m_v3MaxGlobal = vector3(0.0f);
	m_bColliding = false;

	m_v3Color = vector3(0.0f);

	m_vOrthoMax = vector3(0.0f);
	m_vOrthoMin = vector3(0.0f);

	tempRot = quaternion();

	m_v3CenterNewGlobal = vector3(0.0f);
	m_v3MinNewGlobal = vector3(0.0f);
	m_v3MaxNewGlobal = vector3(0.0f);

	visible = true;

	if (vertexList.size() < 1)
		return;

	//start the min and max values
	m_v3Min = vertexList[0];
	m_v3Max = vertexList[0];

	for (int i = 1; i < vertexList.size(); i++)
	{
		if (m_v3Min.x > vertexList[i].x)
		{
			m_v3Min.x = vertexList[i].x;
		}
		else if (m_v3Max.x < vertexList[i].x)
		{
			m_v3Max.x = vertexList[i].x;
		}

		if (m_v3Min.y > vertexList[i].y)
		{
			m_v3Min.y = vertexList[i].y;
		}
		else if (m_v3Max.y < vertexList[i].y)
		{
			m_v3Max.y = vertexList[i].y;
		}

		if (m_v3Min.z > vertexList[i].z)
		{
			m_v3Min.z = vertexList[i].z;
		}
		else if (m_v3Max.z < vertexList[i].z)
		{
			m_v3Max.z = vertexList[i].z;
		}
	}

	//set other start values
	m_v3MinGlobal = m_v3Min;
	m_v3MaxGlobal = m_v3Max;

	m_v3CenterLocal = m_v3CenterGlobal = (m_v3Max + m_v3Min) / 2.0f;
	m_v3CenterNewLocal = m_v3CenterGlobal = (m_v3Max + m_v3Min) / 2.0f;

	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_v3Size.x = glm::distance(vector3(m_v3Min.x, 0.0f, 0.0f), vector3(m_v3Max.x, 0.0f, 0.0f));
	m_v3Size.y = glm::distance(vector3(0.0f, m_v3Min.y, 0.0f), vector3(0.0f, m_v3Max.y, 0.0f));
	m_v3Size.z = glm::distance(vector3(0.0f, 0.0f, m_v3Min.z), vector3(0.0f, 0.0f, m_v3Max.z));

	m_v3NewSize.x = glm::distance(vector3(m_v3Min.x, 0.0f, 0.0f), vector3(m_v3Max.x, 0.0f, 0.0f));
	m_v3NewSize.y = glm::distance(vector3(0.0f, m_v3Min.y, 0.0f), vector3(0.0f, m_v3Max.y, 0.0f));
	m_v3NewSize.z = glm::distance(vector3(0.0f, 0.0f, m_v3Min.z), vector3(0.0f, 0.0f, m_v3Max.z));
}

//check for the axis aligned bounding box every frame
void MyBoundingBoxClass::FindOrthoBox(quaternion rotationArc)
{
	m_vOrthoMax = m_v3Max;
	m_vOrthoMin = m_v3Min;

	vector3 maxXmaxYmaxZ = vector3(m_vOrthoMax.x, m_vOrthoMax.y, m_vOrthoMax.z);
	vector3 maxXmaxYminZ = vector3(m_vOrthoMax.x, m_vOrthoMax.y, m_vOrthoMin.z);
	vector3 maxXminYmaxZ = vector3(m_vOrthoMax.x, m_vOrthoMin.y, m_vOrthoMax.z);
	vector3 maxXminYminZ = vector3(m_vOrthoMax.x, m_vOrthoMin.y, m_vOrthoMin.z);
	vector3 minXmaxYmaxZ = vector3(m_vOrthoMin.x, m_vOrthoMax.y, m_vOrthoMax.z);
	vector3 minXmaxYminZ = vector3(m_vOrthoMin.x, m_vOrthoMax.y, m_vOrthoMin.z);
	vector3 minXminYmaxZ = vector3(m_vOrthoMin.x, m_vOrthoMin.y, m_vOrthoMax.z);
	vector3 minXminYminZ = vector3(m_vOrthoMin.x, m_vOrthoMin.y, m_vOrthoMin.z);
	std::vector<vector3> newMinMaxes = { maxXmaxYmaxZ, maxXmaxYminZ, maxXminYmaxZ, maxXminYminZ, minXmaxYmaxZ, minXmaxYminZ, minXminYmaxZ, minXminYminZ };

	m_v3NewMin = vector3(ToMatrix4(rotationArc) * vector4(newMinMaxes[0], 1.0f));
	m_v3NewMax = vector3(ToMatrix4(rotationArc) * vector4(newMinMaxes[0], 1.0f));

	for (int i = 1; i < newMinMaxes.size(); i++)
	{
		vector3 myVector = vector3(ToMatrix4(rotationArc) * vector4(newMinMaxes[i], 1.0f));
		if (m_v3NewMin.x > myVector.x)
		{
			m_v3NewMin.x = myVector.x;
		}
		else if (m_v3NewMax.x < myVector.x)
		{
			m_v3NewMax.x = myVector.x;
		}

		if (m_v3NewMin.y > myVector.y)
		{
			m_v3NewMin.y = myVector.y;
		}
		else if (m_v3NewMax.y < myVector.y)
		{
			m_v3NewMax.y = myVector.y;
		}

		if (m_v3NewMin.z > myVector.z)
		{
			m_v3NewMin.z = myVector.z;
		}
		else if (m_v3NewMax.z < myVector.z)
		{
			m_v3NewMax.z = myVector.z;
		}
	}

	m_v3CenterNewLocal = (m_v3NewMax + m_v3NewMin) / 2.0f;

	m_v3NewSize.x = glm::distance(vector3(m_v3NewMin.x, 0.0f, 0.0f), vector3(m_v3NewMax.x, 0.0f, 0.0f));
	m_v3NewSize.y = glm::distance(vector3(0.0f, m_v3NewMin.y, 0.0f), vector3(0.0f, m_v3NewMax.y, 0.0f));
	m_v3NewSize.z = glm::distance(vector3(0.0f, 0.0f, m_v3NewMin.z), vector3(0.0f, 0.0f, m_v3NewMax.z));

	tempRot = rotationArc;
}

void MyBoundingBoxClass::Render()
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

		//add local bounding box
		m_pMeshMngr->AddCubeToRenderList(
			m_m4ToWorld *
			glm::translate(m_v3CenterLocal) *
			glm::scale(m_v3Size), m_v3Color, WIRE);



		//add changed bounding box
		m_pMeshMngr->AddCubeToRenderList(
			m_m4ToNewWorld *
			glm::inverse(ToMatrix4(tempRot)) *
			glm::translate(m_v3CenterNewLocal) *
			glm::scale(m_v3NewSize),
			m_v3Color, WIRE);
	}

	/*
	m_v3CenterLocal = m_v3CenterGlobal = (m_v3Max + m_v3Min) / 2.0f;
	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_v3Size = m_v3Max - m_v3Min;

	m_v3MinGlobal = m_v3Min;
	m_v3MaxGlobal = m_v3Max;
	*/
}

void MyBoundingBoxClass::RenderSphere()
{
	vector3 v3Color = REGREEN;
	if (true == m_bColliding)
		v3Color = RERED;

	m_pMeshMngr->AddCubeToRenderList(
		m_m4ToWorld *
		glm::translate(m_v3CenterLocal) *
		glm::scale(m_v3Size),
		v3Color, WIRE);
}
void MyBoundingBoxClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
	if (m_m4ToWorld == a_m4ToWorld)
		return;

	m_m4ToWorld = a_m4ToWorld;
	m_v3CenterGlobal = vector3(m_m4ToWorld * vector4(m_v3CenterLocal, 1.0f));
	m_v3MinGlobal = vector3(m_m4ToWorld * vector4(m_v3Min, 1.0f));
	m_v3MaxGlobal = vector3(m_m4ToWorld * vector4(m_v3Max, 1.0f));



	m_m4ToNewWorld = a_m4ToWorld;
	m_v3CenterNewGlobal = vector3(m_m4ToNewWorld * glm::inverse(ToMatrix4(tempRot)) * vector4(m_v3CenterNewLocal, 1.0f));
	m_v3MinNewGlobal = vector3(m_m4ToNewWorld * glm::inverse(ToMatrix4(tempRot)) * vector4(m_v3NewMin, 1.0f));
	m_v3MaxNewGlobal = vector3(m_m4ToNewWorld * glm::inverse(ToMatrix4(tempRot)) * vector4(m_v3NewMax, 1.0f));
}



bool MyBoundingBoxClass::IsColliding(MyBoundingBoxClass* other)
{

	if (this->m_v3MaxNewGlobal.x < other->m_v3MinNewGlobal.x)
	{
		return false;
	}
	if (this->m_v3MinNewGlobal.x > other->m_v3MaxNewGlobal.x)
	{
		return false;
	}

	if (this->m_v3MaxNewGlobal.y < other->m_v3MinNewGlobal.y)
	{
		return false;
	}
	if (this->m_v3MinNewGlobal.y > other->m_v3MaxNewGlobal.y)
	{
		return false;
	}

	if (this->m_v3MaxNewGlobal.z < other->m_v3MinNewGlobal.z)
	{
		return false;
	}
	if (this->m_v3MinNewGlobal.z > other->m_v3MaxNewGlobal.z)
	{
		return false;
	}
	m_v3MinGlobal = vector3(m_m4ToWorld * vector4(m_v3Min, 1.0f));
	m_v3MaxGlobal = vector3(m_m4ToWorld * vector4(m_v3Max, 1.0f));
}

/*
bool MyBoundingBoxClass::IsColliding(MyBoundingBoxClass* a_other)
{
	if (this->m_v3MaxGlobal.x < a_other->m_v3MinGlobal.x)
		return false;
	if (this->m_v3MinGlobal.x > a_other->m_v3MaxGlobal.x)
		return false;

	if (this->m_v3MaxGlobal.y < a_other->m_v3MinGlobal.y)
		return false;
	if (this->m_v3MinGlobal.y > a_other->m_v3MaxGlobal.y)
		return false;

	if (this->m_v3MaxGlobal.z < a_other->m_v3MinGlobal.z)
		return false;
	if (this->m_v3MinGlobal.z > a_other->m_v3MaxGlobal.z)
		return false;

	return true;
}
*/

void MyBoundingBoxClass::SetColliding(bool input) { m_bColliding = input; }
void MyBoundingBoxClass::SetCenterLocal(vector3 input) { m_v3CenterLocal = input; }
void MyBoundingBoxClass::SetCenterGlobal(vector3 input) { m_v3CenterGlobal = input; }
bool MyBoundingBoxClass::GetColliding(void) { return m_bColliding; }
vector3 MyBoundingBoxClass::GetCenterLocal(void) { return m_v3CenterLocal; }
vector3 MyBoundingBoxClass::GetCenterGlobal(void) { return m_v3CenterGlobal; }
matrix4 MyBoundingBoxClass::GetModelMatrix(void) { return m_m4ToWorld; }
vector3 MyBoundingBoxClass::GetMinGlobal(void) { return m_v3MinGlobal; }
vector3 MyBoundingBoxClass::GetMaxGlobal(void) { return m_v3MaxGlobal; }
vector3 MyBoundingBoxClass::GetColor(void) { return m_v3Color; }
void MyBoundingBoxClass::SetColor(vector3 input) { m_v3Color = input; }
bool MyBoundingBoxClass::GetVisibility(void) { return visible; }
void MyBoundingBoxClass::SetVisibility(bool input) { visible = input; }
//void MyBoundingBoxClass::SetRadius(float input) { m_fRadius = input; }
//bool MyBoundingBoxClass::GetColliding(void) { return m_bColliding; }
//vector3 MyBoundingBoxClass::GetCenterLocal(void) { return m_v3CenterLocal; }
//vector3 MyBoundingBoxClass::GetCenterGlobal(void) { return m_v3CenterGlobal; }
//float MyBoundingBoxClass::GetRadius(void) { return m_fRadius; }
//matrix4 MyBoundingBoxClass::GetModelMatrix(void) { return m_m4ToWorld; }

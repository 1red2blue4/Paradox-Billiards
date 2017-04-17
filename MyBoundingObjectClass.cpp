#include "MyBoundingObjectClass.h"

void MyBoundingObjectClass::SetCollidingBox(bool input) 
{ 
	m_myBox->SetColliding(input);
}
void MyBoundingObjectClass::SetCollidingSphere(bool input)
{
	m_mySphere->SetColliding(input);
}
void MyBoundingObjectClass::SetCenterLocal(vector3 input) 
{ 
	m_myBox->SetCenterLocal(input);
	m_mySphere->SetCenterLocal(input);
}
void MyBoundingObjectClass::SetCenterGlobal(vector3 input) 
{ 
	m_myBox->SetCenterGlobal(input); 
	m_mySphere->SetCenterGlobal(input);
}
void MyBoundingObjectClass::SetModelMatrix(matrix4 m_matrix)
{
	m_myBox->SetModelMatrix(m_matrix);
	m_mySphere->SetModelMatrix(m_matrix);
}
bool MyBoundingObjectClass::GetCollidingBox(void) 
{ 
	return m_myBox->GetColliding(); 
}
bool MyBoundingObjectClass::GetCollidingSphere(void)
{
	return m_mySphere->GetColliding();
}
vector3 MyBoundingObjectClass::GetCenterLocal(void) 
{ 
	return m_myBox->GetCenterLocal(); 
}
vector3 MyBoundingObjectClass::GetCenterGlobal(void) 
{ 
	return m_myBox->GetCenterGlobal(); 
}
matrix4 MyBoundingObjectClass::GetModelMatrix(void) 
{ 
	return m_myBox->GetModelMatrix(); 
}
MyBoundingBoxClass* MyBoundingObjectClass::GetBoundingBox(void)
{
	return m_myBox;
}
MyBoundingSphereClass* MyBoundingObjectClass::GetBoundingSphere(void)
{
	return m_mySphere;
}


void MyBoundingObjectClass::SetSphereColor(vector3 color) 
{ 
	m_mySphere->SetColor(color);
}

void MyBoundingObjectClass::SetBoxColor(vector3 color)
{
	m_myBox->SetColor(color);
}

void MyBoundingObjectClass::ToggleBoxVisibility()
{
	m_myBox->SetVisibility(!m_myBox->GetVisibility());
}

void MyBoundingObjectClass::ToggleSphereVisibility()
{
	m_mySphere->SetVisibility(!m_mySphere->GetVisibility());
}

void MyBoundingObjectClass::SetVisibility(bool input)
{
	m_mySphere->SetVisibility(input);
	m_myBox->SetVisibility(input);
}


MyBoundingObjectClass::MyBoundingObjectClass(std::vector<vector3> vertexList)
{
	m_myBox = new MyBoundingBoxClass(vertexList);
	m_mySphere = new MyBoundingSphereClass(vertexList);
}



void MyBoundingObjectClass::Render() 
{
	m_myBox->Render();
	m_mySphere->RenderSphere();
}

bool MyBoundingObjectClass::IsBoxColliding(MyBoundingBoxClass* other)
{

	return m_myBox->IsColliding(other);
	/*
	if (m_myBox->GetMaxGlobal().x < other->GetMinGlobal().x)
	{
		return false;
	}
	if (m_myBox->GetMinGlobal().x > other->GetMaxGlobal().x)
	{
		return false;
	}

	if (m_myBox->GetMaxGlobal().y < other->GetMinGlobal().y)
	{
		return false;
	}
	if (m_myBox->GetMinGlobal().y > other->GetMaxGlobal().y)
	{
		return false;
	}

	if (m_myBox->GetMaxGlobal().z < other->GetMinGlobal().z)
	{
		return false;
	}
	if (m_myBox->GetMinGlobal().z > other->GetMaxGlobal().z)
	{
		return false;
	}

	return true;
	*/
}

bool MyBoundingObjectClass::IsSphereColliding(MyBoundingSphereClass* other)
{
	return m_mySphere->IsColliding(other);
	/*
	float fDistance = glm::distance(m_mySphere->GetCenterGlobal(), other->GetCenterGlobal());
	float fRadiiSum = m_mySphere->GetRadius() + other->GetRadius();
	if (fDistance < fRadiiSum)
	{
		return true;
	}
	else
	{
		return false;
	}
	*/
}

MyBoundingObjectClass::~MyBoundingObjectClass()
{
	delete m_myBox;
	delete m_mySphere;
}
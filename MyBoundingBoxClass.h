#pragma once
#include "RE\ReEng.h"

class MyBoundingBoxClass
{

private:
	//float m_fRadius = 0.0f; //radius of the sphere
	vector3 m_v3CenterLocal = vector3(0.0f); //center of the sphere in local 
	vector3 m_v3CenterNewLocal = vector3(0.0f);
	vector3 m_v3CenterGlobal = vector3(0.0f); //center of the sphere in global space
	matrix4 m_m4ToWorld = IDENTITY_M4; //matrix that takes you from local to global space
	matrix4 m_m4ToNewWorld = IDENTITY_M4;
	MeshManagerSingleton* m_pMeshMngr = nullptr; //for drawing the sphere

	vector3 m_v3Size = vector3(0.0f);
	vector3 m_v3NewSize = vector3(0.0f);
	vector3 m_v3Min = vector3(0.0f);
	vector3 m_v3Max = vector3(0.0f);
	vector3 m_v3NewMin = vector3(0.0f);
	vector3 m_v3NewMax = vector3(0.0f);
	vector3 m_v3MinGlobal = vector3(0.0f);
	vector3 m_v3MaxGlobal = vector3(0.0f);
	bool m_bColliding = false;

	vector3 m_v3Color = vector3(0.0f);

	vector3 m_vOrthoMax = vector3(0.0f);
	vector3 m_vOrthoMin = vector3(0.0f);

	quaternion tempRot = quaternion();

	vector3 m_v3CenterNewGlobal = vector3(0.0f);
	vector3 m_v3MinNewGlobal = vector3(0.0f);
	vector3 m_v3MaxNewGlobal = vector3(0.0f);

	bool visible = true;


public:


	
/*
private:
	float m_fRadius = 0.0f; //radius of the sphere
	vector3 m_v3CenterLocal = vector3(0.0f); //center of the sphere in local space
	vector3 m_v3CenterGlobal = vector3(0.0f); //center of the sphere in global space
	matrix4 m_m4ToWorld = IDENTITY_M4; //matrix that takes you from local to global space
	MeshManagerSingleton* m_pMeshMngr = nullptr; //for drawing the sphere
	bool m_bColliding = false;

	vector3 m_v3Max;
	vector3 m_v3Min;
	vector3 m_v3Size;

	vector3 m_v3MaxG;
	vector3 m_v3MinG;
	vector3 m_v3SizeG;
	*/
public:
	/*
	Finds the axis realligned bounding box
	*/
	void FindOrthoBox(quaternion rotationArc);

	/*
	Sets Colliding
	*/
	void SetColliding(bool input);
	/*
	Sets Center of the sphere in local space
	*/
	void SetCenterLocal(vector3 input);
	/*
	Sets Center of the sphere in global space
	*/
	void SetCenterGlobal(vector3 input);
	/*
	Sets the radius of the sphere
	*/
	//void SetRadius(float input);

	/*
	Gets Colliding
	*/
	bool GetColliding(void);
	/*
	Gets center of the sphere in local space
	*/
	vector3 GetCenterLocal(void);
	/*
	Gets center of the sphere in global space
	*/
	vector3 GetCenterGlobal(void);
	/*
	Gets model to world matrix of the sphere
	*/
	matrix4 GetModelMatrix(void);

	/*
	Get the min and max global values
	*/
	vector3 GetMinGlobal(void);
	vector3 GetMaxGlobal(void);
	/*
	Get and set the colors
	*/
	vector3 GetColor(void);
	void SetColor(vector3 input);

	/*
	Get and set visibility
	*/
	bool GetVisibility(void);
	void SetVisibility(bool input);

	/*
	Gets radius of the sphere
	*/
	//float GetRadius(void);
	
	/*
	Constructor, needs a vertex list
	*/
	MyBoundingBoxClass(std::vector<vector3> vertexList);
	/*
	Renders the sphere based on the radius and the center in global space
	*/
	void Render();
	void RenderSphere();
	/*
	Sets the transform from the local to world matrix
	*/
	void SetModelMatrix(matrix4 a_m4ToWorld);

	/*
	Will check the collision with another object
	*/
	bool IsColliding(MyBoundingBoxClass* a_other);
};
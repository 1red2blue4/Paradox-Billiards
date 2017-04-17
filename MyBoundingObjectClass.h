#pragma once
#include "MyBoundingBoxClass.h"
#include "MyBoundingSphereClass.h"
#include "RE\ReEng.h"

class MyBoundingObjectClass
{
private:
	MyBoundingBoxClass* m_myBox;
	MyBoundingSphereClass* m_mySphere;

	//bool m_bColliding;
	//vector3 m_v3CenterLocal;
	//vector3 m_v3CenterGlobal;
	//matrix4 m_m4ToWorld;


	//vector3 m_v3Min = vector3(0.0f);
	//vector3 m_v3Max = vector3(0.0f);
	//vector3 v3Color = REGREEN;

public:
	MyBoundingObjectClass(std::vector<vector3> vertexList);
	~MyBoundingObjectClass();

	//set visibility
	void ToggleBoxVisibility();
	void ToggleSphereVisibility();
	void SetVisibility(bool input);

	/*
	Sets Colliding
	*/
	void SetCollidingBox(bool input);
	void SetCollidingSphere(bool input);
	/*
	Sets Center of the sphere in local space
	*/
	void SetCenterLocal(vector3 input);
	/*
	Sets Center of the sphere in global space
	*/
	void SetCenterGlobal(vector3 input);

	/*
	Get the box or the sphere
	*/
	MyBoundingBoxClass* GetBoundingBox(void);
	MyBoundingSphereClass* GetBoundingSphere(void);


	/*
	Gets Colliding
	*/
	bool GetCollidingBox(void);
	bool GetCollidingSphere(void);
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
	Sets model to world matrix of the sphere
	*/
	void SetModelMatrix(matrix4 m_matrix);
	/*
	Sets the color of the Bounding Object
	*/
	void SetSphereColor(vector3 color);
	void SetBoxColor(vector3 color);
	/*
	Renders the sphere based on the radius and the center in global space
	*/
	void Render();
	/*
	Will check the collision with another object using boxes
	*/
	bool IsBoxColliding(MyBoundingBoxClass* other);

	/*
	Will check the collision with another object using spheres
	*/
	bool IsSphereColliding(MyBoundingSphereClass* other);

};
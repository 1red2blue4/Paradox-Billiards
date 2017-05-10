/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/11
----------------------------------------------*/
#ifndef __MYBOCLASS_H_
#define __MYBOCLASS_H_

#include "RE\ReEng.h"

#define SHOWPLANES

//System Class
class MyBOClass
{
	float m_fRadius = 0.0f; //Radius of the Bounding Object

	bool isTable = false;
	bool isBall = false;

	vector3 m_v3Color = vector3();

	MeshManagerSingleton* m_pMeshMngr = nullptr;//Mesh Manager Singleton

	vector3 m_v3Center = vector3(0.0f); //Will store the center point of the Object Class
	vector3 m_v3Min = vector3(0.0f); //Will store the minimum vector of the Object Class
	vector3 m_v3Max = vector3(0.0f); //Will store the maximum vector of the Object Class

	vector3 m_v3CenterG = vector3(0.0f); //Will store the center point of the Object Class
	vector3 m_v3MinG = vector3(0.0f); //Will store the minimum vector of the Object Class
	vector3 m_v3MaxG = vector3(0.0f); //Will store the maximum vector of the Object Class

	vector3 m_v3HalfWidth = vector3(0.0f);//Will store half the size of all sides
	vector3 m_v3HalfWidthG = vector3(0.0f);//Will store half the size of all sides

	matrix4 m_m4ToWorld = IDENTITY_M4; //Matrix that will take us from local to world coordinate
public:

	vector3 GetNormalToPlane(vector3 p, vector3 q, vector3 r);

	//set whether or not an object is a ball or table
	void SetBall(bool is);
	void SetTable(bool is);
	//returns true is the object is a ball//table
	bool IsBall();
	bool IsTable();

	void SetColor(vector3 color);
	vector3 GetColor();

	/*
	USAGE: Constructor
	ARGUMENTS: ---
	OUTPUT: class object
	*/
	MyBOClass(std::vector<vector3> a_lVectorList);
	/*
	USAGE: Copy Constructor
	ARGUMENTS: class object to copy
	OUTPUT: class object instance
	*/
	MyBOClass(MyBOClass const& other);
	/*
	USAGE: Copy Assignment Operator
	ARGUMENTS: class object to copy
	OUTPUT: ---
	*/
	MyBOClass& operator=(MyBOClass const& other);
	/*
	USAGE: Destructor
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~MyBOClass(void);
	/*
	USAGE: Changes object contents for other object's
	ARGUMENTS:
	- MyBOClass& other -> object to swap content from
	OUTPUT: ---
	*/
	void Swap(MyBOClass& other);
	/*
	USAGE: Sets the Bounding Object into world coordinates
	ARGUMENTS:
	- matrix4 a_m4ToWorld -> Model to World matrix
	OUTPUT: ---
	*/
	void SetModelMatrix(matrix4 a_m4ToWorld);
	/*
	USAGE: Gets the Bounding Object into world coordinates
	ARGUMENTS:---
	OUTPUT: Model to World matrix
	*/
	matrix4 GetModelMatrix(void);
	/*
	USAGE: Gets the Bounding Object's radius
	ARGUMENTS: ---
	OUTPUT: Radius of the BO
	*/
	float GetRadius(void);
	/*
	USAGE: Gets the Bounding Object's center in local coordinates
	ARGUMENTS: ---
	OUTPUT: Center's of the Object in local coordinates
	*/
	vector3 GetCenterLocal(void);
	/*
	USAGE: Gets the Bounding Object's center in global coordinates
	ARGUMENTS: ---
	OUTPUT: Center's of the Object in global coordinates
	*/
	vector3 GetCenterGlobal(void);
	/*
	USAGE: Gets the Bounding Object sizes for all sides (divided in half)
	ARGUMENTS: ---
	OUTPUT: HalfWidth Vector
	*/
	vector3 GetHalfWidth(void);
	/*
	USAGE: Gets the Bounding Object sizes for all sides (divided in half) reoriented
	ARGUMENTS: ---
	OUTPUT: HalfWidth Vector
	*/
	vector3 GetHalfWidthG(void);
	/*
	USAGE: Gets the min vector from the Oriented Bounding Box in local space
	ARGUMENTS: ---
	OUTPUT: Minimum in global space
	*/
	vector3 GetMin(void);
	/*
	USAGE: Gets the max vector from the Oriented Bounding Box in local space
	ARGUMENTS: ---
	OUTPUT: Maximum in global space
	*/
	vector3 GetMax(void);
	/*
	USAGE: Gets the min vector from the ReAlligned Bounding Box in global space
	ARGUMENTS: ---
	OUTPUT: Minimum in global space
	*/
	vector3 GetMinG(void);
	/*
	USAGE: Gets the max vector from the ReAlligned Bounding Box in global space
	ARGUMENTS: ---
	OUTPUT: Maximum in global space
	*/
	vector3 GetMaxG(void);
	/*
	USAGE: Asks if there is a collision with another Bounding Object object
	ARGUMENTS:
	- MyBOClass* const a_pOther -> Other object to check collision with
	OUTPUT: check of the collision
	*/
	bool IsColliding(MyBOClass* const a_pOther);
	/*
	USAGE: Determines the collision with an incoming object using the SAT
	ARGUMENTS:
	- MyBOClass* const a_pOther -> Other object to check collision with
	OUTPUT: result of the collision
	*/
	bool SAT(MyBOClass* const a_pOther);

	bool SATCircle(MyBOClass* const a_pOther);
	/*
	USAGE: Displays the Bounding Sphere of the BO in the specified color
	ARGUMENTS:
	- vector3 a_v3Color = REDEFAULT -> Color of the Object to display if the value is REDEFAULT it
	-- will display Objects in white and colliding ones in red.
	OUTPUT: ---
	*/
	void DisplaySphere(vector3 a_v3Color = REDEFAULT);
	/*
	USAGE: Displays the Oriented Bounding Box of the BO in the specified color
	ARGUMENTS:
	- vector3 a_v3Color = REDEFAULT -> Color of the Object to display if the value is REDEFAULT it
	-- will display Objects in white and colliding ones in red.
	OUTPUT: ---
	*/
	void DisplayOriented(vector3 a_v3Color = REDEFAULT);
	/*
	USAGE: Displays the ReAlligned Bounding Box of the BO in the specified color
	ARGUMENTS:
	- vector3 a_v3Color = REDEFAULT -> Color of the Object to display if the value is REDEFAULT it
	-- will display Objects in white and colliding ones in red.
	OUTPUT: ---
	*/
	void DisplayReAlligned(vector3 a_v3Color = REDEFAULT);

private:
	/*
	USAGE: Deallocates member fields
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Release(void);
	/*
	USAGE: Allocates member fields
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Init(void);
};

#endif //__MYBOCLASS_H__
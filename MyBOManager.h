/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/11
----------------------------------------------*/
#ifndef __MYBOMANAGER_H_
#define __MYBOMANAGER_H_

#include "MyBOClass.h"

//System Class
class MyBOManager
{
	uint m_nObjectCount = 0; //number of AABB in the list
	static MyBOManager* m_pInstance; // Singleton pointer
	std::vector<MyBOClass*> m_lObject; //list of AABB
	MeshManagerSingleton* m_pMeshMngr = nullptr;//Mesh Manager Singleton
	std::vector<std::vector<int>> m_llCollidingIndices; //List of list of colliding indices.
	std::map<String, uint> m_mapIndex;//Map relating the mesh and the index
public:
	/*
	USAGE: Gets the static instance of the class
	ARGUMENTS: ---
	OUTPUT: static pointer to class
	*/
	static MyBOManager* GetInstance();
	/*
	USAGE: releases the static pointer to class
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	static void ReleaseInstance(void);
	/*
	USAGE: Creates a Object from a list of vertices and adds it to the object list.
	ARGUMENTS:
	- vector3 a_lVertex -> list of vertex to create a Object from
	- String a_sName -> name of the object
	OUTPUT: ---
	*/
	void AddObject(std::vector<vector3> a_lVertex, String a_sName);
	/*
	USAGE: Creates a Object from a list of vertices and adds it to the object list from a previously
	loaded object to the mesh manager.
	ARGUMENTS:
	- String a_sName -> name of the object
	OUTPUT: ---
	*/
	void AddObject(String a_sName);
	/*
	USAGE: Sets the model matrix to the specified index
	ARGUMENTS:
	- matrix4 a_mModelMatrix -> ModelToWorld matrix to set
	- String a_sIndex -> Index to set into, will find the related uint through a dictionary
	OUTPUT: ---
	*/
	void SetModelMatrix(matrix4 a_mModelMatrix, String a_sIndex);
	/*
	USAGE: Displays the Bounding Sphere of the model in the specified color
	ARGUMENTS:
	- int a_nIndex -> Index of Object to be displayed, a negative index indicate that all
	-- Objects will be drawn.
	- vector3 a_v3Color = REDEFAULT -> Color of the Object to display if the value is REDEFAULT it
	-- will display Objects in white and colliding ones in red.
	OUTPUT: ---
	*/
	void DisplaySphere(int a_nIndex, vector3 a_v3Color = REDEFAULT);
	/*
	USAGE: Displays the Bounding Sphere of the model in the specified color
	ARGUMENTS:
	- String a_sName = "ALL" -> Name of index of Object to be displayed, ALL will display all
	-- objects in the manager.
	- vector3 a_v3Color = REDEFAULT -> Color of the Object to display if the value is REDEFAULT it
	-- will display Objects in white and colliding ones in red.
	OUTPUT: ---
	*/
	void DisplaySphere(String a_sName = "ALL", vector3 a_v3Color = REDEFAULT);
	/*
	USAGE: Displays the Oriented Bounding Object of the model in the specified color
	ARGUMENTS:
	- int a_nIndex -> Index of Object to be displayed, a negative index indicate that all
	-- Objects will be drawn.
	- vector3 a_v3Color = REDEFAULT -> Color of the Object to display if the value is REDEFAULT it
	-- will display Objects in white and colliding ones in red.
	OUTPUT: ---
	*/
	void DisplayOriented(int a_nIndex, vector3 a_v3Color = REDEFAULT);
	/*
	USAGE: Displays the Oriented Bounding Object of the model in the specified color
	ARGUMENTS:
	- int a_nIndex -> Index of Object to be displayed, a negative index indicate that all
	-- Objects will be drawn.
	- vector3 a_v3Color = REDEFAULT -> Color of the Object to display if the value is REDEFAULT it
	-- will display Objects in white and colliding ones in red.
	OUTPUT: ---
	*/
	void DisplayOriented(String a_sName = "ALL", vector3 a_v3Color = REDEFAULT);
	/*
	USAGE: Displays the ReAlligned Bounding Object of the model in the specified color
	ARGUMENTS:
	- int a_nIndex -> Index of Object to be displayed, a negative index indicate that all
	-- Objects will be drawn.
	- vector3 a_v3Color = REDEFAULT -> Color of the Object to display if the value is REDEFAULT it
	-- will display Objects in white and colliding ones in red.
	OUTPUT: ---
	*/
	void DisplayReAlligned(int a_nIndex, vector3 a_v3Color = REDEFAULT);
	/*
	USAGE: Displays the ReAlligned Bounding Object of the model in the specified color
	ARGUMENTS:
	- int a_nIndex -> Index of Object to be displayed, a negative index indicate that all
	-- Objects will be drawn.
	- vector3 a_v3Color = REDEFAULT -> Color of the Object to display if the value is REDEFAULT it
	-- will display Objects in white and colliding ones in red.
	OUTPUT: ---
	*/
	void DisplayReAlligned(String a_sName = "ALL", vector3 a_v3Color = REDEFAULT);
	/*
	USAGE: Returns the list of indices of object for which the BO (specified by name) is colliding with
	ARGUMENTS:
	- String a_sIndex -> Name of the bounding object (index)
	OUTPUT: list of colliding objects index
	*/
	std::vector<int> GetCollidingVector(String a_sIndex);
	/*
	USAGE: Returns the list of indices of object for which the BO (specified by index) is colliding with
	ARGUMENTS:
	- uint a_nIndex -> index of the bounding object
	OUTPUT: list of colliding objects index
	*/
	std::vector<int> GetCollidingVector(uint a_nIndex);
	/*
	USAGE: Returns the index of the BO specified by name from the dictionary
	ARGUMENTS:
	- String a_sIndex -> name of the index
	OUTPUT: index of the BO specified by name, -1 if not found
	*/
	int GetIndex(String a_sIndex);
	/*
	USAGE: returns the element specified by the index
	ARGUMENTS:
	- uint a_nIndex -> Index of the object in the manager
	OUTPUT: MyBOClass* from the list
	*/
	MyBOClass* GetBoundingObject(uint a_nIndex);
	/*
	USAGE: returns the element specified by the index
	ARGUMENTS:
	- String a_sIndex -> Name of the object in the manager
	OUTPUT: MyBOClass* from the list
	*/
	MyBOClass* GetBoundingObject(String a_sIndex);
	/*
	USAGE: Calculates the intermediary states of all the members in the variables
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Update(void);

private:
	/*
	USAGE: instantiates the object
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	MyBOManager(void);
	/*
	USAGE: does nothing (singleton behavior)
	ARGUMENTS: other instance to copy
	OUTPUT: ---
	*/
	MyBOManager(MyBOManager const& other);
	/*
	USAGE: does nothing (singleton behavior)
	ARGUMENTS: other instance to copy
	OUTPUT: ---
	*/
	MyBOManager& operator=(MyBOManager const& other);
	/*
	USAGE: releases the instance
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~MyBOManager(void);
	/*
	USAGE: deallocates the memory of the method
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Release(void);
	/*
	USAGE: allocates the members of the method
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Init(void);
	/*
	USAGE: Calculates the collision between all OBs
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void CheckCollisions(void);
};

#endif //__MYBOMANAGER_H_
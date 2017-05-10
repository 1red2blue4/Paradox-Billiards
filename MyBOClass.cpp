#include "MyBOClass.h"
//  MyBOClass
void MyBOClass::Init(void)
{
	m_m4ToWorld = IDENTITY_M4;

	isTable = false;
	isBall = false;

	m_v3Color = vector3();

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_v3CenterG = vector3(0.0f);
	m_v3MinG = vector3(0.0f);
	m_v3MaxG = vector3(0.0f);

	m_v3HalfWidth = vector3(0.0f);
	m_v3HalfWidthG = vector3(0.0f);

	m_fRadius = 0.0f;

	m_pMeshMngr = MeshManagerSingleton::GetInstance();
}
vector3 MyBOClass::GetNormalToPlane(vector3 p, vector3 q, vector3 r)
{
	vector3 v1 = p - q;
	vector3 v2 = r - q;
	return glm::cross(v1, v2);
}
void MyBOClass::SetBall(bool is)
{
	isBall = is;
}
bool MyBOClass::IsBall()
{
	return isBall;
}
void MyBOClass::SetTable(bool is)
{
	isTable = is;
}
bool MyBOClass::IsTable()
{
	return isTable;
}
void MyBOClass::Swap(MyBOClass& other)
{
	std::swap(m_m4ToWorld, other.m_m4ToWorld);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	std::swap(m_v3CenterG, other.m_v3CenterG);
	std::swap(m_v3MinG, other.m_v3MinG);
	std::swap(m_v3MaxG, other.m_v3MaxG);

	std::swap(m_v3HalfWidth, other.m_v3HalfWidth);
	std::swap(m_v3HalfWidthG, other.m_v3HalfWidthG);

	std::swap(m_fRadius, other.m_fRadius);

	std::swap(m_pMeshMngr, other.m_pMeshMngr);
}
void MyBOClass::Release(void) {/* No pointers allocated yet */ }
//The big 3
MyBOClass::MyBOClass(std::vector<vector3> a_lVectorList)
{
	//Init the default values
	Init();

	//Count the points of the incoming list
	uint nVertexCount = a_lVectorList.size();

	//If there are none just return, we have no information to create the BS from
	if (nVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3Max = m_v3Min = a_lVectorList[0];

	//Get the max and min out of the list
	for (uint nVertex = 1; nVertex < nVertexCount; nVertex++)
	{
		if (m_v3Min.x > a_lVectorList[nVertex].x) //If min is larger than current
			m_v3Min.x = a_lVectorList[nVertex].x;
		else if (m_v3Max.x < a_lVectorList[nVertex].x)//if max is smaller than current
			m_v3Max.x = a_lVectorList[nVertex].x;

		if (m_v3Min.y > a_lVectorList[nVertex].y) //If min is larger than current
			m_v3Min.y = a_lVectorList[nVertex].y;
		else if (m_v3Max.y < a_lVectorList[nVertex].y)//if max is smaller than current
			m_v3Max.y = a_lVectorList[nVertex].y;

		if (m_v3Min.z > a_lVectorList[nVertex].z) //If min is larger than current
			m_v3Min.z = a_lVectorList[nVertex].z;
		else if (m_v3Max.z < a_lVectorList[nVertex].z)//if max is smaller than current
			m_v3Max.z = a_lVectorList[nVertex].z;
	}

	//with the max and the min we calculate the center
	m_v3Center = (m_v3Min + m_v3Max) / 2.0f;

	m_fRadius = glm::distance(m_v3Center, m_v3Max);
	//another way of doing this, except that with it the sphere just surrounds the object making the
	//pretest kind of useless
	//m_fRadius = 0;
	//for (uint nVertex = 0; nVertex < nVertexCount; nVertex++)
	//{
	//	float fDistance = glm::distance(m_v3Center, a_lVectorList[nVertex]);
	//	if (m_fRadius < fDistance)
	//		m_fRadius = fDistance;
	//}

	//we calculate the distance between all the values of min and max vectors
	m_v3HalfWidth = (m_v3Max - m_v3Min) / 2.0f;

	m_v3MaxG = m_v3Max;
	m_v3MinG = m_v3Min;
	m_v3CenterG = m_v3Center;
	m_v3HalfWidthG = m_v3HalfWidth;
}
MyBOClass::MyBOClass(MyBOClass const& other)
{
	m_m4ToWorld = other.m_m4ToWorld;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_v3CenterG = other.m_v3CenterG;
	m_v3MinG = other.m_v3MinG;
	m_v3MaxG = other.m_v3MaxG;

	m_v3HalfWidth = other.m_v3HalfWidth;
	m_v3HalfWidthG = other.m_v3HalfWidthG;

	m_fRadius = other.m_fRadius;

	m_pMeshMngr = other.m_pMeshMngr;
}
MyBOClass& MyBOClass::operator=(MyBOClass const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyBOClass temp(other);
		Swap(temp);
	}
	return *this;
}
MyBOClass::~MyBOClass() { Release(); };
//Accessors
void MyBOClass::SetModelMatrix(matrix4 a_m4ToWorld)
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
	m_v3MaxG = m_v3MinG = v3Corner[0];
	for (uint nVertex = 1; nVertex < 8; nVertex++)
	{
		if (m_v3MinG.x > v3Corner[nVertex].x) //If min is larger than current
			m_v3MinG.x = v3Corner[nVertex].x;
		else if (m_v3MaxG.x < v3Corner[nVertex].x)//if max is smaller than current
			m_v3MaxG.x = v3Corner[nVertex].x;

		if (m_v3MinG.y > v3Corner[nVertex].y) //If min is larger than current
			m_v3MinG.y = v3Corner[nVertex].y;
		else if (m_v3MaxG.y < v3Corner[nVertex].y)//if max is smaller than current
			m_v3MaxG.y = v3Corner[nVertex].y;

		if (m_v3MinG.z > v3Corner[nVertex].z) //If min is larger than current
			m_v3MinG.z = v3Corner[nVertex].z;
		else if (m_v3MaxG.z < v3Corner[nVertex].z)//if max is smaller than current
			m_v3MaxG.z = v3Corner[nVertex].z;
	}
	m_v3CenterG = (m_v3MinG + m_v3MaxG) / 2.0f;

	//we calculate the distance between all the values of min and max vectors
	m_v3HalfWidthG = (m_v3MaxG - m_v3MinG) / 2.0f;

	m_fRadius = glm::distance(m_v3CenterG, m_v3MaxG);
}
float MyBOClass::GetRadius(void) { return m_fRadius; }
matrix4 MyBOClass::GetModelMatrix(void) { return m_m4ToWorld; }
vector3 MyBOClass::GetCenterLocal(void) { return m_v3Center; }
vector3 MyBOClass::GetCenterGlobal(void) { return m_v3CenterG; }
vector3 MyBOClass::GetHalfWidth(void) { return m_v3HalfWidth; }
vector3 MyBOClass::GetHalfWidthG(void) { return m_v3HalfWidthG; }
vector3 MyBOClass::GetMin(void) { return m_v3Min; }
vector3 MyBOClass::GetMax(void) { return m_v3Max; }
vector3 MyBOClass::GetMinG(void) { return m_v3MinG; }
vector3 MyBOClass::GetMaxG(void) { return m_v3MaxG; }
//--- Non Standard Singleton Methods
void MyBOClass::DisplaySphere(vector3 a_v3Color)
{
	m_pMeshMngr->AddSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) *
		glm::scale(vector3(m_fRadius * 2.0f)), a_v3Color, WIRE);
}
void MyBOClass::DisplayOriented(vector3 a_v3Color)
{
	m_pMeshMngr->AddCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) *
		glm::scale(m_v3HalfWidth * 2.0f), a_v3Color, WIRE);
}
void MyBOClass::DisplayReAlligned(vector3 a_v3Color)
{
	m_pMeshMngr->AddCubeToRenderList(glm::translate(IDENTITY_M4, m_v3CenterG) *
		glm::scale(m_v3HalfWidthG * 2.0f), a_v3Color, WIRE);
}

//works the same as SAT, but if this object is a circle
bool MyBOClass::SATCircle(MyBOClass* const a_pOther)
{
	// Get the information of this object
	vector3 v3CenterGlobalA = GetCenterGlobal();
	matrix4 mToWorldA = GetModelMatrix();
	vector3 v3RotationA[3];
	v3RotationA[0] = vector3(mToWorldA[0][0], mToWorldA[0][1], mToWorldA[0][2]);
	v3RotationA[1] = vector3(mToWorldA[1][0], mToWorldA[1][1], mToWorldA[1][2]);
	v3RotationA[2] = vector3(mToWorldA[2][0], mToWorldA[2][1], mToWorldA[2][2]);

	//Get the information of the other object
	vector3 v3CenterGlobalB = a_pOther->GetCenterGlobal();
	matrix4 mToWorldB = a_pOther->GetModelMatrix();
	vector3 v3RotationB[3];
	v3RotationB[0] = vector3(mToWorldB[0][0], mToWorldB[0][1], mToWorldB[0][2]);
	v3RotationB[1] = vector3(mToWorldB[1][0], mToWorldB[1][1], mToWorldB[1][2]);
	v3RotationB[2] = vector3(mToWorldB[2][0], mToWorldB[2][1], mToWorldB[2][2]);

	float fCenterAToMiddle, fCenterBToMiddle;
	glm::mat3 m3Rotation, m3RotationAbs;

	// Compute rotation matrix expressing b in a's coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m3Rotation[i][j] = glm::dot(v3RotationA[i], v3RotationB[j]);

	// Compute translation vector v3Distance (this is the distance between both centers)
	vector3 v3Distance = v3CenterGlobalB - v3CenterGlobalA; //distance in global space
															// Bring translation into a's coordinate frame
	v3Distance = vector3(glm::dot(v3Distance, v3RotationA[0]), glm::dot(v3Distance, v3RotationA[1]), glm::dot(v3Distance, v3RotationA[2])); //distance in A's local
																																			// their cross product is (near) null (see the orange book for details)
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m3RotationAbs[i][j] = std::abs(m3Rotation[i][j]) + 0.0001f;

	// Test axes L = AX <- 0
	fCenterAToMiddle = m_v3HalfWidth.x;
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[0][0] + a_pOther->m_v3HalfWidth.y * m3RotationAbs[0][1] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[0][2];
	if (std::abs(v3Distance.x) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center;
		matrix4 m4WorldToLocal = glm::inverse(m_m4ToWorld);
		if (m_v3CenterG.x < a_pOther->m_v3CenterG.x)
		{
			vector3 v3OtherMinInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MinG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MaxG, 1.0f)) + v3OtherMinInA;
		}
		else
		{
			vector3 v3OtherMaxInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MaxG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MinG, 1.0f)) + v3OtherMaxInA;
		}
		v3Center /= 2.0f;

		matrix4 m4Space = glm::translate(m_m4ToWorld, v3Center) * glm::rotate(IDENTITY_M4, 90.0f, REAXISY);
		//m_pMeshMngr->AddPlaneToRenderList(m4Space * glm::scale(vector3(5.0f)), RERED);
		return false;
	}

	// Test axes L = AY <- 1
	fCenterAToMiddle = m_v3HalfWidth.y;
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[1][0] + a_pOther->m_v3HalfWidth.y * m3RotationAbs[1][1] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[1][2];
	if (std::abs(v3Distance.y) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center;
		matrix4 m4WorldToLocal = glm::inverse(m_m4ToWorld);
		if (m_v3CenterG.y < a_pOther->m_v3CenterG.y)
		{
			vector3 v3OtherMinInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MinG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MaxG, 1.0f)) + v3OtherMinInA;
		}
		else
		{
			vector3 v3OtherMaxInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MaxG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MinG, 1.0f)) + v3OtherMaxInA;
		}
		v3Center /= 2.0f;
		matrix4 m4Space = glm::translate(m_m4ToWorld, v3Center) * glm::rotate(IDENTITY_M4, 90.0f, REAXISX);
		//m_pMeshMngr->AddPlaneToRenderList(m4Space * glm::scale(vector3(5.0f)), REGREEN);
		return false;
	}

	// Test axes L = AZ <- 2
	fCenterAToMiddle = m_v3HalfWidth.z;
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[2][0] + a_pOther->m_v3HalfWidth.y * m3RotationAbs[2][1] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[2][2];
	if (std::abs(v3Distance.z) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center;
		matrix4 m4WorldToLocal = glm::inverse(m_m4ToWorld);
		if (m_v3CenterG.z < a_pOther->m_v3CenterG.z)
		{
			vector3 v3OtherMinInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MinG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MaxG, 1.0f)) + v3OtherMinInA;
		}
		else
		{
			vector3 v3OtherMaxInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MaxG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MinG, 1.0f)) + v3OtherMaxInA;
		}
		v3Center /= 2.0f;
		matrix4 m4Space = glm::translate(m_m4ToWorld, v3Center);
		//m_pMeshMngr->AddPlaneToRenderList(m4Space * glm::scale(vector3(5.0f)), REBLUE);
		return false;
	}
}

bool MyBOClass::SAT(MyBOClass* const a_pOther)
{
	// Get the information of this object
	vector3 v3CenterGlobalA = GetCenterGlobal();
	matrix4 mToWorldA = GetModelMatrix();
	vector3 v3RotationA[3];
	v3RotationA[0] = vector3(mToWorldA[0][0], mToWorldA[0][1], mToWorldA[0][2]);
	v3RotationA[1] = vector3(mToWorldA[1][0], mToWorldA[1][1], mToWorldA[1][2]);
	v3RotationA[2] = vector3(mToWorldA[2][0], mToWorldA[2][1], mToWorldA[2][2]);

	//Get the information of the other object
	vector3 v3CenterGlobalB = a_pOther->GetCenterGlobal();
	matrix4 mToWorldB = a_pOther->GetModelMatrix();
	vector3 v3RotationB[3];
	v3RotationB[0] = vector3(mToWorldB[0][0], mToWorldB[0][1], mToWorldB[0][2]);
	v3RotationB[1] = vector3(mToWorldB[1][0], mToWorldB[1][1], mToWorldB[1][2]);
	v3RotationB[2] = vector3(mToWorldB[2][0], mToWorldB[2][1], mToWorldB[2][2]);

	float fCenterAToMiddle, fCenterBToMiddle;
	glm::mat3 m3Rotation, m3RotationAbs;

	// Compute rotation matrix expressing b in a's coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m3Rotation[i][j] = glm::dot(v3RotationA[i], v3RotationB[j]);

	// Compute translation vector v3Distance (this is the distance between both centers)
	vector3 v3Distance = v3CenterGlobalB - v3CenterGlobalA; //distance in global space
															// Bring translation into a's coordinate frame
	v3Distance = vector3(glm::dot(v3Distance, v3RotationA[0]), glm::dot(v3Distance, v3RotationA[1]), glm::dot(v3Distance, v3RotationA[2])); //distance in A's local
																																// their cross product is (near) null (see the orange book for details)
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m3RotationAbs[i][j] = std::abs(m3Rotation[i][j]) + 0.0001f;

	// Test axes L = AX <- 0
	fCenterAToMiddle = m_v3HalfWidth.x;
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[0][0] + a_pOther->m_v3HalfWidth.y * m3RotationAbs[0][1] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[0][2];
	if (std::abs(v3Distance.x) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center;
		matrix4 m4WorldToLocal = glm::inverse(m_m4ToWorld);
		if (m_v3CenterG.x < a_pOther->m_v3CenterG.x)
		{
			vector3 v3OtherMinInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MinG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MaxG, 1.0f)) + v3OtherMinInA;
		}
		else
		{
			vector3 v3OtherMaxInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MaxG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MinG, 1.0f)) + v3OtherMaxInA;
		}
		v3Center /= 2.0f;

		matrix4 m4Space = glm::translate(m_m4ToWorld, v3Center) * glm::rotate(IDENTITY_M4, 90.0f, REAXISY);
		//m_pMeshMngr->AddPlaneToRenderList(m4Space * glm::scale(vector3(5.0f)), RERED);
		return false;
	}

	// Test axes L = AY <- 1
	fCenterAToMiddle = m_v3HalfWidth.y;
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[1][0] + a_pOther->m_v3HalfWidth.y * m3RotationAbs[1][1] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[1][2];
	if (std::abs(v3Distance.y) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center;
		matrix4 m4WorldToLocal = glm::inverse(m_m4ToWorld);
		if (m_v3CenterG.y < a_pOther->m_v3CenterG.y)
		{
			vector3 v3OtherMinInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MinG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MaxG, 1.0f)) + v3OtherMinInA;
		}
		else
		{
			vector3 v3OtherMaxInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MaxG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MinG, 1.0f)) + v3OtherMaxInA;
		}
		v3Center /= 2.0f;
		matrix4 m4Space = glm::translate(m_m4ToWorld, v3Center) * glm::rotate(IDENTITY_M4, 90.0f, REAXISX);
		//m_pMeshMngr->AddPlaneToRenderList(m4Space * glm::scale(vector3(5.0f)), REGREEN);
		return false;
	}

	// Test axes L = AZ <- 2
	fCenterAToMiddle = m_v3HalfWidth.z;
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[2][0] + a_pOther->m_v3HalfWidth.y * m3RotationAbs[2][1] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[2][2];
	if (std::abs(v3Distance.z) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center;
		matrix4 m4WorldToLocal = glm::inverse(m_m4ToWorld);
		if (m_v3CenterG.z < a_pOther->m_v3CenterG.z)
		{
			vector3 v3OtherMinInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MinG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MaxG, 1.0f)) + v3OtherMinInA;
		}
		else
		{
			vector3 v3OtherMaxInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MaxG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MinG, 1.0f)) + v3OtherMaxInA;
		}
		v3Center /= 2.0f;
		matrix4 m4Space = glm::translate(m_m4ToWorld, v3Center);
		//m_pMeshMngr->AddPlaneToRenderList(m4Space * glm::scale(vector3(5.0f)), REBLUE);
		return false;
	}

	// Test axes L = BX <- 3
	fCenterAToMiddle = m_v3HalfWidth.x * m3RotationAbs[0][0] + m_v3HalfWidth.y * m3RotationAbs[1][0] + m_v3HalfWidth.z * m3RotationAbs[2][0];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x;
	if (std::abs(v3Distance.x * m3Rotation[0][0] + v3Distance.y * m3Rotation[1][0] + v3Distance.z * m3Rotation[2][0]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		matrix4 m4Space = glm::translate(IDENTITY_M4, a_pOther->m_v3CenterG) * glm::rotate(IDENTITY_M4, 90.0f, REAXISY);
		//m_pMeshMngr->AddPlaneToRenderList(m4Space * glm::scale(vector3(5.0f)), RERED * 0.33f);
		return false;
	}

	// Test axes L = BY <- 4
	fCenterAToMiddle = m_v3HalfWidth.x * m3RotationAbs[0][1] + m_v3HalfWidth.y * m3RotationAbs[1][1] + m_v3HalfWidth.z * m3RotationAbs[2][1];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.y;
	if (std::abs(v3Distance.x * m3Rotation[0][1] + v3Distance.y * m3Rotation[1][1] + v3Distance.z * m3Rotation[2][1]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		matrix4 m4Space = glm::translate(IDENTITY_M4, a_pOther->m_v3CenterG) * glm::rotate(IDENTITY_M4, 90.0f, REAXISX);
		//m_pMeshMngr->AddPlaneToRenderList(m4Space * glm::scale(vector3(5.0f)), REGREEN * 0.33f);
		return false;
	}

	// Test axes L = BZ <- 5
	fCenterAToMiddle = m_v3HalfWidth.x * m3RotationAbs[0][2] + m_v3HalfWidth.y * m3RotationAbs[1][2] + m_v3HalfWidth.z * m3RotationAbs[2][2];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.z;
	if (std::abs(v3Distance.x * m3Rotation[0][2] + v3Distance.y * m3Rotation[1][2] + v3Distance.z * m3Rotation[2][2]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		matrix4 m4Space = glm::translate(IDENTITY_M4, a_pOther->m_v3CenterG);
		//m_pMeshMngr->AddPlaneToRenderList(m4Space * glm::scale(vector3(5.0f)), REBLUE * 0.33f);
		return false;
	}

	// Test axis L = AX x BX <- 6
	fCenterAToMiddle = m_v3HalfWidth.y * m3RotationAbs[2][0] + m_v3HalfWidth.z * m3RotationAbs[1][0];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.y * m3RotationAbs[0][2] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[0][1];
	if (std::abs(v3Distance.z * m3Rotation[1][0] - v3Distance.y * m3Rotation[2][0]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center;
		matrix4 m4WorldToLocal = glm::inverse(m_m4ToWorld);
		if (m_v3CenterG.z < a_pOther->m_v3CenterG.z)
		{
			vector3 v3OtherMinInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MinG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MaxG, 1.0f)) + v3OtherMinInA;
		}
		else
		{
			vector3 v3OtherMaxInA = vector3(m4WorldToLocal * vector4(a_pOther->m_v3MaxG, 1.0f));
			v3Center = vector3(m4WorldToLocal * vector4(m_v3MinG, 1.0f)) + v3OtherMaxInA;
		}
		v3Center /= 2.0f;
		matrix4 m4Space = glm::translate(m_m4ToWorld, v3Center);
		m_pMeshMngr->AddSphereToRenderList(m4Space * glm::scale(vector3(0.10f)), REYELLOW, SOLID);
		return false;
	}

	// Test axis L = AX x BY <- 7
	fCenterAToMiddle = m_v3HalfWidth.y * m3RotationAbs[2][1] + m_v3HalfWidth.z * m3RotationAbs[1][1];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[0][2] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[0][0];
	if (std::abs(v3Distance.z * m3Rotation[1][1] - v3Distance.y * m3Rotation[2][1]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center = (m_v3CenterG + a_pOther->m_v3CenterG) / 2.0f;
		matrix4 m4Space = glm::translate(IDENTITY_M4, v3Center);
		m_pMeshMngr->AddSphereToRenderList(m4Space * glm::scale(vector3(0.10f)), REYELLOW, SOLID);
		return false;
	}

	// Test axis L = AX x BZ <- 8
	fCenterAToMiddle = m_v3HalfWidth.y * m3RotationAbs[2][2] + m_v3HalfWidth.z * m3RotationAbs[1][2];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[0][1] + a_pOther->m_v3HalfWidth.y * m3RotationAbs[0][0];
	if (std::abs(v3Distance.z * m3Rotation[1][2] - v3Distance.y * m3Rotation[2][2]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center = (m_v3CenterG + a_pOther->m_v3CenterG) / 2.0f;
		matrix4 m4Space = glm::translate(IDENTITY_M4, v3Center);
		m_pMeshMngr->AddSphereToRenderList(m4Space * glm::scale(vector3(0.10f)), REYELLOW, SOLID);
		return false;
	}

	// Test axis L = AY x BX <- 9
	fCenterAToMiddle = m_v3HalfWidth.x * m3RotationAbs[2][0] + m_v3HalfWidth.z * m3RotationAbs[0][0];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.y * m3RotationAbs[1][2] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[1][1];
	if (std::abs(v3Distance.x * m3Rotation[2][0] - v3Distance.z * m3Rotation[0][0]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center = (m_v3CenterG + a_pOther->m_v3CenterG) / 2.0f;
		matrix4 m4Space = glm::translate(IDENTITY_M4, v3Center);
		m_pMeshMngr->AddSphereToRenderList(m4Space * glm::scale(vector3(0.10f)), REYELLOW, SOLID);
		return false;
	}

	// Test axis L = AY x BY <- 10
	fCenterAToMiddle = m_v3HalfWidth.x * m3RotationAbs[2][1] + m_v3HalfWidth.z * m3RotationAbs[0][1];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[1][2] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[1][0];
	if (std::abs(v3Distance.x * m3Rotation[2][1] - v3Distance.z * m3Rotation[0][1]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center = (m_v3CenterG + a_pOther->m_v3CenterG) / 2.0f;
		matrix4 m4Space = glm::translate(IDENTITY_M4, v3Center);
		m_pMeshMngr->AddSphereToRenderList(m4Space * glm::scale(vector3(0.10f)), REYELLOW, SOLID);
		return false;
	}

	// Test axis L = AY x BZ <- 11
	fCenterAToMiddle = m_v3HalfWidth.x * m3RotationAbs[2][2] + m_v3HalfWidth.z * m3RotationAbs[0][2];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[1][1] + a_pOther->m_v3HalfWidth.y * m3RotationAbs[1][0];
	if (std::abs(v3Distance.x * m3Rotation[2][2] - v3Distance.z * m3Rotation[0][2]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center = (m_v3CenterG + a_pOther->m_v3CenterG) / 2.0f;
		matrix4 m4Space = glm::translate(IDENTITY_M4, v3Center);
		m_pMeshMngr->AddSphereToRenderList(m4Space * glm::scale(vector3(0.10f)), REYELLOW, SOLID);
		return false;
	}

	// Test axis L = AZ x BX <- 12
	fCenterAToMiddle = m_v3HalfWidth.x * m3RotationAbs[1][0] + m_v3HalfWidth.y * m3RotationAbs[0][0];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.y * m3RotationAbs[2][2] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[2][1];
	if (std::abs(v3Distance.y * m3Rotation[0][0] - v3Distance.x * m3Rotation[1][0]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center = (m_v3CenterG + a_pOther->m_v3CenterG) / 2.0f;
		matrix4 m4Space = glm::translate(IDENTITY_M4, v3Center);
		m_pMeshMngr->AddSphereToRenderList(m4Space * glm::scale(vector3(0.10f)), REYELLOW, SOLID);
		return false;
	}

	// Test axis L = AZ x BY <- 13
	fCenterAToMiddle = m_v3HalfWidth.x * m3RotationAbs[1][1] + m_v3HalfWidth.y * m3RotationAbs[0][1];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[2][2] + a_pOther->m_v3HalfWidth.z * m3RotationAbs[2][0];
	if (std::abs(v3Distance.y * m3Rotation[0][1] - v3Distance.x * m3Rotation[1][1]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center = (m_v3CenterG + a_pOther->m_v3CenterG) / 2.0f;
		matrix4 m4Space = glm::translate(IDENTITY_M4, v3Center);
		m_pMeshMngr->AddSphereToRenderList(m4Space * glm::scale(vector3(0.10f)), REYELLOW, SOLID);
		return false;
	}

	// Test axis L = AZ x BZ <- 14
	fCenterAToMiddle = m_v3HalfWidth.x * m3RotationAbs[1][2] + m_v3HalfWidth.y * m3RotationAbs[0][2];
	fCenterBToMiddle = a_pOther->m_v3HalfWidth.x * m3RotationAbs[2][1] + a_pOther->m_v3HalfWidth.y * m3RotationAbs[2][0];
	if (std::abs(v3Distance.y * m3Rotation[0][2] - v3Distance.x * m3Rotation[1][2]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		vector3 v3Center = (m_v3CenterG + a_pOther->m_v3CenterG) / 2.0f;
		matrix4 m4Space = glm::translate(IDENTITY_M4, v3Center);
		m_pMeshMngr->AddSphereToRenderList(m4Space * glm::scale(vector3(0.10f)), REYELLOW, SOLID);
		return false;
	}

	// Since no separating axis found, the OBBs must a_pOther->m_v3HalfWidth intersecting
	return true;
}
bool MyBOClass::IsColliding(MyBOClass* const a_pOther)
{
	//Get all vectors in global space
	vector3 v3Min = vector3(m_m4ToWorld * vector4(m_v3Min, 1.0f));
	vector3 v3Max = vector3(m_m4ToWorld * vector4(m_v3Max, 1.0f));

	vector3 v3MinO = vector3(a_pOther->m_m4ToWorld * vector4(a_pOther->m_v3Min, 1.0f));
	vector3 v3MaxO = vector3(a_pOther->m_m4ToWorld * vector4(a_pOther->m_v3Max, 1.0f));

	//Are they colliding?
	//For Objects we will assume they are colliding, unless at least one of the following conditions is not met
	//first check the bounding sphere, if that is not colliding we can guarantee that there are no collision
	if ((m_fRadius + a_pOther->m_fRadius) < glm::distance(m_v3CenterG, a_pOther->m_v3CenterG))
		return false;

	//If the distance was smaller it might be colliding
	bool bColliding = true;

	//Check for X
	if (m_v3MaxG.x < a_pOther->m_v3MinG.x)
		bColliding = false;
	if (m_v3MinG.x > a_pOther->m_v3MaxG.x)
		bColliding = false;

	//Check for Y
	if (m_v3MaxG.y < a_pOther->m_v3MinG.y)
		bColliding = false;
	if (m_v3MinG.y > a_pOther->m_v3MaxG.y)
		bColliding = false;

	//Check for Z
	if (m_v3MaxG.z < a_pOther->m_v3MinG.z)
		bColliding = false;
	if (m_v3MinG.z > a_pOther->m_v3MaxG.z)
		bColliding = false;

	if (bColliding == false)
		return false;

	return SAT(a_pOther);
}

void MyBOClass::SetColor(vector3 color) { m_v3Color = color; }
vector3 MyBOClass::GetColor() { return m_v3Color; }

#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Paradox Billiards");
}

void AppClass::InitVariables(void)
{
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 10.0f, 56.0f),//Camera position
		vector3(0.0f, 0.0f, 0.0f),//What Im looking at
		REAXISY);//What is up

	//Load models onto the Mesh manager
	m_pMeshMngr->LoadModel("media\\models\\table_with_colors.obj", "table1");
	m_pMeshMngr->LoadModel("media\\models\\table_with_colors.obj", "table2");
	m_pMeshMngr->LoadModel("media\\models\\table_with_colors.obj", "table3");
	m_pMeshMngr->LoadModel("media\\models\\table_with_colors.obj", "table4");
	m_pMeshMngr->LoadModel("media\\models\\ball1.obj", "ball1");
	/*m_pMeshMngr->LoadModel("media\\models\\ball2.obj", "ball2");
	m_pMeshMngr->LoadModel("media\\models\\ball3.obj", "ball3");
	m_pMeshMngr->LoadModel("media\\models\\ball4.obj", "ball4");
	m_pMeshMngr->LoadModel("media\\models\\ball5.obj", "ball5");
	m_pMeshMngr->LoadModel("media\\models\\ball6.obj", "ball6");
	m_pMeshMngr->LoadModel("media\\models\\ball7.obj", "ball7");
	m_pMeshMngr->LoadModel("media\\models\\ball8.obj", "ball8");
	m_pMeshMngr->LoadModel("media\\models\\ball9.obj", "ball9");
	m_pMeshMngr->LoadModel("media\\models\\ball10.obj", "ball10");
	m_pMeshMngr->LoadModel("media\\models\\ball11.obj", "ball11");
	m_pMeshMngr->LoadModel("media\\models\\ball12.obj", "ball12");
	m_pMeshMngr->LoadModel("media\\models\\ball13.obj", "ball13");
	m_pMeshMngr->LoadModel("media\\models\\ball14.obj", "ball14");
	m_pMeshMngr->LoadModel("media\\models\\ball15.obj", "ball15");*/


	m_ball1Phys = new BallPhysics();
	m_ball1Phys->SetGravity(vector3(0.0f, -0.05f, 0.0f));

	//used for SAT test
	//Load Models
	//m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
	//m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper");

	//Initialize positions
	//m_v3O1 = vector3(-2.5f, 0.0f, 0.0f);
	//m_v3O2 = vector3(2.5f, 0.0f, 0.0f);

	m_pBOMngr = MyBOManager::GetInstance();
	m_pBOMngr->AddObject(m_pMeshMngr->GetVertexList("ball1"), "ball1");
	m_pBOMngr->AddObject(m_pMeshMngr->GetVertexList("table1"), "table1");
	m_pBOMngr->AddObject(m_pMeshMngr->GetVertexList("table2"), "table2");
	m_pBOMngr->AddObject(m_pMeshMngr->GetVertexList("table3"), "table3");
	m_pBOMngr->AddObject(m_pMeshMngr->GetVertexList("table4"), "table4");

	m_backgroundOn = true;
	m_seeControls = true;

	m_ball1Pos = vector3();
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Call the arcball method
	ArcBall();

	//Object Movement
	float stretch = 9.0f;
	static float fTimer = 0.0f;
	static int nClock = m_pSystem->GenClock();
	float fDeltaTime = static_cast<float>(m_pSystem->LapClock(nClock));
	fTimer += fDeltaTime;

	static quaternion table1Rot = quaternion(vector3(0.0f, -PI / 2.0f, 0.0f));
	static quaternion table2Rot = quaternion(vector3(PI, -PI / 2.0f, 0.0f));
	static quaternion table3Rot = quaternion(vector3(0.0f, PI / 2.0f, PI / 2.0f));
	static quaternion table4Rot = quaternion(vector3(0.0f, PI / 2.0f, -PI / 2.0f));
	static quaternion ball1Rot = quaternion(vector3());

	static vector3 table3Pos = vector3(stretch, 0.0, 0.0); //right
	static vector3 table4Pos = vector3(-stretch, 0.0, 0.0); //left
	static vector3 table2Pos = vector3(0.0, stretch, 0.0); //up
	static vector3 table1Pos = vector3(0.0, -stretch, 0.0); //down

	//static vector3 ball1Pos = vector3(); //ball position
	static vector3 ball1Force = m_ball1Phys->GetForce(); //ball force

	m_ball1Phys->SetBounce(vector3());

	m_ball1Phys->AddForce(m_ball1Phys->GetGravity());

	if (m_pBOMngr->GetBoundingObject("ball1")->IsColliding(m_pBOMngr->GetBoundingObject("table1")))
	{
		//m_ball1Phys->ZeroVelocity();
		//m_ball1Phys->SetForce(vector3());
		float speed = m_ball1Phys->GetVelocity().y;
		m_ball1Phys->SetForce(vector3());
		m_ball1Phys->SetVelocity(vector3());

		matrix4 table1Mat = m_pBOMngr->GetBoundingObject("table1")->GetModelMatrix();
		vector3 table1Max = m_pBOMngr->GetBoundingObject("table1")->GetMaxG();
		vector3 table1Min = m_pBOMngr->GetBoundingObject("table1")->GetMinG();

		vector3 localUpperRightForward = vector3(table1Max.x, table1Max.y, table1Min.z);
		vector3 localUpperRightBack = vector3(table1Max.x, table1Max.y, table1Max.z);
		vector3 localUpperLeftForward = vector3(table1Min.x, table1Max.y, table1Min.z);

		vector3 globalUpperRightForward = vector3(table1Mat * vector4(localUpperRightForward, 1.0f));
		vector3 globalUpperRightBack = vector3(table1Mat * vector4(localUpperRightBack, 1.0f));
		vector3 globalUpperLeftForward = vector3(table1Mat * vector4(localUpperLeftForward, 1.0f));

		vector3 normal = m_pBOMngr->GetBoundingObject("table1")->GetNormalToPlane(globalUpperRightForward, globalUpperRightBack, globalUpperLeftForward);

		float sizeNormal = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
		

		vector3 normalizedNormal = normal / sizeNormal;

		std::cout << "X: " << normal.x << "\n" << "Y: " << normal.y << "\n" << "Z: " << normal.z << "\n";

		m_ball1Phys->AddForce(normalizedNormal*0.05f);
		
	}
	if (m_pBOMngr->GetBoundingObject("ball1")->IsColliding(m_pBOMngr->GetBoundingObject("table2")))
	{
		//m_ball1Phys->ZeroVelocity();
		//m_ball1Phys->SetForce(vector3());
		float speed = m_ball1Phys->GetVelocity().y;
		m_ball1Phys->SetForce(vector3());
		m_ball1Phys->SetVelocity(vector3());

		vector3 table1Max = m_pBOMngr->GetBoundingObject("table2")->GetMaxG();
		vector3 table1Min = m_pBOMngr->GetBoundingObject("table2")->GetMinG();
		matrix4 table1Mat = m_pBOMngr->GetBoundingObject("table2")->GetModelMatrix();

		vector3 localUpperRightForward = vector3(table1Max.x, table1Max.y, table1Min.z);
		vector3 localUpperRightBack = vector3(table1Max.x, table1Max.y, table1Max.z);
		vector3 localUpperLeftForward = vector3(table1Min.x, table1Max.y, table1Min.z);

		vector3 globalUpperRightForward = vector3(table1Mat * vector4(localUpperRightForward, 1.0f));
		vector3 globalUpperRightBack = vector3(table1Mat * vector4(localUpperRightBack, 1.0f));
		vector3 globalUpperLeftForward = vector3(table1Mat * vector4(localUpperLeftForward, 1.0f));

		vector3 normal = m_pBOMngr->GetBoundingObject("table2")->GetNormalToPlane(globalUpperRightForward, globalUpperRightBack, globalUpperLeftForward);

		float sizeNormal = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);


		vector3 normalizedNormal = normal / sizeNormal;

		//std::cout << "X: " << normal.x << "\n" << "Y: " << normal.y << "\n" << "Z: " << normal.z << "\n";

		m_ball1Phys->AddForce(normalizedNormal*0.05f);

	}
	if (m_pBOMngr->GetBoundingObject("ball1")->IsColliding(m_pBOMngr->GetBoundingObject("table3")))
	{
		//m_ball1Phys->ZeroVelocity();
		//m_ball1Phys->SetForce(vector3());
		float speed = m_ball1Phys->GetVelocity().y;
		m_ball1Phys->SetForce(vector3());
		m_ball1Phys->SetVelocity(vector3());

		matrix4 table1Mat = m_pBOMngr->GetBoundingObject("table3")->GetModelMatrix();
		vector3 table1Max = m_pBOMngr->GetBoundingObject("table3")->GetMaxG();
		vector3 table1Min = m_pBOMngr->GetBoundingObject("table3")->GetMinG();

		vector3 localUpperRightForward = vector3(table1Max.x, table1Max.y, table1Min.z);
		vector3 localUpperRightBack = vector3(table1Max.x, table1Max.y, table1Max.z);
		vector3 localUpperLeftForward = vector3(table1Min.x, table1Max.y, table1Min.z);

		vector3 globalUpperRightForward = vector3(table1Mat * vector4(localUpperRightForward, 1.0f));
		vector3 globalUpperRightBack = vector3(table1Mat * vector4(localUpperRightBack, 1.0f));
		vector3 globalUpperLeftForward = vector3(table1Mat * vector4(localUpperLeftForward, 1.0f));

		vector3 normal = m_pBOMngr->GetBoundingObject("table3")->GetNormalToPlane(globalUpperRightForward, globalUpperRightBack, globalUpperLeftForward);

		float sizeNormal = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);


		vector3 normalizedNormal = normal / sizeNormal;

		//std::cout << "X: " << normal.x << "\n" << "Y: " << normal.y << "\n" << "Z: " << normal.z << "\n";

		m_ball1Phys->AddForce(normalizedNormal*0.05f);

	}
	if (m_pBOMngr->GetBoundingObject("ball1")->IsColliding(m_pBOMngr->GetBoundingObject("table4")))
	{
		//m_ball1Phys->ZeroVelocity();
		//m_ball1Phys->SetForce(vector3());
		float speed = m_ball1Phys->GetVelocity().y;
		m_ball1Phys->SetForce(vector3());
		m_ball1Phys->SetVelocity(vector3());

		matrix4 table1Mat = m_pBOMngr->GetBoundingObject("table4")->GetModelMatrix();
		vector3 table1Max = m_pBOMngr->GetBoundingObject("table4")->GetMaxG();
		vector3 table1Min = m_pBOMngr->GetBoundingObject("table4")->GetMinG();

		vector3 localUpperRightForward = vector3(table1Max.x, table1Max.y, table1Min.z);
		vector3 localUpperRightBack = vector3(table1Max.x, table1Max.y, table1Max.z);
		vector3 localUpperLeftForward = vector3(table1Min.x, table1Max.y, table1Min.z);

		vector3 globalUpperRightForward = vector3(table1Mat * vector4(localUpperRightForward, 1.0f));
		vector3 globalUpperRightBack = vector3(table1Mat * vector4(localUpperRightBack, 1.0f));
		vector3 globalUpperLeftForward = vector3(table1Mat * vector4(localUpperLeftForward, 1.0f));

		vector3 normal = m_pBOMngr->GetBoundingObject("table4")->GetNormalToPlane(globalUpperRightForward, globalUpperRightBack, globalUpperLeftForward);

		float sizeNormal = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);


		vector3 normalizedNormal = normal / sizeNormal;

		//std::cout << "X: " << normal.x << "\n" << "Y: " << normal.y << "\n" << "Z: " << normal.z << "\n";

		m_ball1Phys->AddForce(normalizedNormal*0.05f);

	}


	

	m_ball1Pos = m_ball1Phys->ApplyForces(m_ball1Pos);

	//move the tables to their proper location
	//numbered tablenRot is for that table's unique rotation
	//m_tableRotation is for chaos rotation
	//m_sceneRotation is for moving all the tables as one 
	m_pMeshMngr->SetModelMatrix(glm::translate(ToMatrix4(m_sceneRotation), table1Pos) * ToMatrix4(table1Rot) * ToMatrix4(m_tableRotation), "table1");
	m_pMeshMngr->SetModelMatrix(glm::translate(ToMatrix4(m_sceneRotation), table2Pos) * ToMatrix4(table2Rot) * ToMatrix4(m_tableRotation), "table2");
	m_pMeshMngr->SetModelMatrix(glm::translate(ToMatrix4(m_sceneRotation), table3Pos) * ToMatrix4(table3Rot) * ToMatrix4(m_tableRotation), "table3");
	m_pMeshMngr->SetModelMatrix(glm::translate(ToMatrix4(m_sceneRotation), table4Pos) * ToMatrix4(table4Rot) * ToMatrix4(m_tableRotation), "table4");
	m_pMeshMngr->SetModelMatrix(glm::translate(IDENTITY_M4, m_ball1Pos) * ToMatrix4(ball1Rot), "ball1");
		
	//Set the model matrix to the Bounding Object
	m_pBOMngr->SetModelMatrix(m_pMeshMngr->GetModelMatrix("table1"), "table1");
	m_pBOMngr->SetModelMatrix(m_pMeshMngr->GetModelMatrix("table2"), "table2");
	m_pBOMngr->SetModelMatrix(m_pMeshMngr->GetModelMatrix("table3"), "table3");
	m_pBOMngr->SetModelMatrix(m_pMeshMngr->GetModelMatrix("table4"), "table4");
	m_pBOMngr->SetModelMatrix(m_pMeshMngr->GetModelMatrix("ball1"), "ball1");


	m_pBOMngr->Update();//Update collision detection

	m_pBOMngr->DisplaySphere("ball1");

	/*
	matrix4 temp = m_pMeshMngr->GetModelMatrix("table1");

	vector3 max = m_pBOMngr->GetBoundingObject("table1")->GetMax();
	vector3 min = m_pBOMngr->GetBoundingObject("table1")->GetMin();

	vector3 topRightForward = vector3(temp[3][0] + max.x, temp[3][1] + max.y, temp[3][2] + min.z);
	vector3 topRightBack = vector3(temp[3][0] + max.x, temp[3][1] + max.y, temp[3][2] + max.z);
	vector3 topLeftForward = vector3(temp[3][0] + min.x, temp[3][1] + max.y, temp[3][2] + min.z);

	vector3 normal = m_pBOMngr->GetBoundingObject("table1")->GetNormalToPlane(topRightForward, topRightBack, topLeftForward);

	std::cout << "X: " << normal.x << "\n" << "Y: " << normal.y << "\n" << "Z: " << normal.z << "\n";
	*/

	//Print info on the screen

	//Adds all loaded instance to the render list
	if (!m_backgroundOn)
	{
		m_pMeshMngr->AddSkyboxToRenderList();
	}
	else
	{
		m_pMeshMngr->AddSkyboxToRenderList("barPh.png");
	}

	//render everything
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	m_pMeshMngr->PrintLine("");//Add a line on top
	if (m_seeControls)
	{
		//m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
		m_pMeshMngr->Print("I,O,P: Rotate tables");
		m_pMeshMngr->Print("J,K,L: Rotation Chaos");
		m_pMeshMngr->Print("\nW, A, S, D, Q, E: Move camera");
		m_pMeshMngr->Print("\nRight click: Aim camera");
		m_pMeshMngr->Print("\nR: Reset tables");
		m_pMeshMngr->Print("\nB: Toggle background");
		m_pMeshMngr->Print("\nT: Toggle controls");
	}
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_ball1Phys);
	super::Release(); //release the memory of the inherited fields
	MyBOManager::ReleaseInstance();
}
#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Bounding Spheres DEMO");
}

void AppClass::InitVariables(void)
{
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 0.0f, 15.0f),//Camera position
		vector3(0.0f, 0.0f, 0.0f),//What Im looking at
		REAXISY);//What is up
	//Load a model onto the Mesh manager
	m_pMeshMngr->LoadModel("Minecraft\\Zombie.obj", "Zombie1");
	m_pMeshMngr->LoadModel("Minecraft\\Zombie.obj", "Zombie2");
	m_pMeshMngr->LoadModel("Minecraft\\Zombie.obj", "Zombie3");
	m_pMeshMngr->LoadModel("Minecraft\\Zombie.obj", "Zombie4");

	m_boMngr = BoundingObjectManager::GetInstance();
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
	static float fTimer = 0.0f;
	static int nClock = m_pSystem->GenClock();
	float fDeltaTime = static_cast<float>(m_pSystem->LapClock(nClock));
	fTimer += fDeltaTime;

	static quaternion zombie1Rot = quaternion(vector3(0.0f, -PI / 2.0f, 0.0f));
	static quaternion zombie2Rot = quaternion(vector3(PI, -PI / 2.0f, 0.0f));
	static quaternion zombie3Rot = quaternion(vector3(0.0f, -PI / 2.0f, PI / 2.0f));
	static quaternion zombie4Rot = quaternion(vector3(0.0f, PI / 2.0f, PI / 2.0f));

	static vector3 zombie1Pos = vector3(3.0, -1.0, 0.0);
	static vector3 zombie2Pos = vector3(-3.0, 1.0, 0.0);
	static vector3 zombie3Pos = vector3(1.0, 2.0, 0.0);
	static vector3 zombie4Pos = vector3(1.0, -3.0, 0.0);

	//set the translate to create the transform matrix
	matrix4 m4Transform = glm::translate(m_v3Position) * ToMatrix4(m_qArcBall);
	m_pMeshMngr->SetModelMatrix(glm::translate(ToMatrix4(m_sceneRotation), zombie1Pos) * ToMatrix4(zombie1Rot), "Zombie1");
	m_pMeshMngr->SetModelMatrix(glm::translate(ToMatrix4(m_sceneRotation), zombie2Pos) * ToMatrix4(zombie2Rot), "Zombie2");
	m_pMeshMngr->SetModelMatrix(glm::translate(ToMatrix4(m_sceneRotation), zombie3Pos) * ToMatrix4(zombie3Rot), "Zombie3");
	m_pMeshMngr->SetModelMatrix(glm::translate(ToMatrix4(m_sceneRotation), zombie4Pos) * ToMatrix4(zombie4Rot), "Zombie4");
	

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	//SafeDelete(m_pBS0);
	//SafeDelete(m_pBS1);
	//SafeDelete(m_pBS2);
	super::Release(); //release the memory of the inherited fields
	m_boMngr->ReleaseInstance();
}
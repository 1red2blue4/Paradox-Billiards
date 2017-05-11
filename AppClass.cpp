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

	myTriMag = new TriangleMagic();

	m_tableForce = 0.01f;
	m_ballForce = 0.0075f;

	m_numBalls = 15;
	m_numTables = 4;

	for (int i = 0; i < m_numTables; i++)
	{
		String name = "table" + std::to_string(i);
		m_tableNames.push_back(name);
	}

	for (int i = 0; i < m_numBalls; i++)
	{
		String name = "ball" + std::to_string(i);
		m_ballNames.push_back(name);
		m_shouldFall.push_back(true);
	}

	//Load models onto the Mesh manager
	for (int i = 0; i < m_numTables; i++)
	{
		m_pMeshMngr->LoadModel("media\\models\\table_with_colors.obj", m_tableNames[i]);
	}

	for (int i = 1; i < m_numBalls + 1; i++)
	{
		String name;
		if (i == m_numBalls)
		{
			name = "media\\models\\ball" + std::to_string(i) + ".obj";
		}
		else
		{
			name = "media\\models\\" + m_ballNames[i] + ".obj";
		}
		m_pMeshMngr->LoadModel(name, m_ballNames[i-1]);
	}

	//make physics for balls
	m_pPhysicsMngr = PhysicsManager::GetInstance();

	for (int i = 0; i < m_numBalls; i++)
	{
		m_pPhysicsMngr->MakePhysicsObject(m_ballNames[i]);
	}

	m_pBOMngr = MyBOManager::GetInstance();

	for (int i = 0; i < m_numTables; i++)
	{
		m_pBOMngr->AddObject(m_pMeshMngr->GetVertexList(m_tableNames[i]), m_tableNames[i]);
		m_pBOMngr->AddToAllTables(m_pMeshMngr->GetVertexList(m_tableNames[i]), m_tableNames[i]);
		m_pBOMngr->GetBoundingObject(m_tableNames[i])->SetTable(true);
		m_allTablePos.push_back(vector3());
		m_allTableRot.push_back(quaternion());	
	}

	float stretch = 9.0f;

	m_allTableRot[0] = quaternion(vector3(0.0f, -PI / 2.0f, 0.0f));
	m_allTableRot[1] = quaternion(vector3(PI, -PI / 2.0f, 0.0f));
	m_allTableRot[2] = quaternion(vector3(0.0f, PI / 2.0f, PI / 2.0f));
	m_allTableRot[3] = quaternion(vector3(0.0f, PI / 2.0f, -PI / 2.0f));

	m_allTablePos[0] = vector3(0.0, -stretch, 0.0); //down
	m_allTablePos[1] = vector3(0.0, stretch, 0.0); //up
	m_allTablePos[2] = vector3(stretch, 0.0, 0.0); //right
	m_allTablePos[3] = vector3(-stretch, 0.0, 0.0); //left

	for (int i = 0; i < m_numTables; i++)
	{
		allMatrices.push_back(m_pMeshMngr->GetModelMatrix(m_tableNames[i]));
		allBO.push_back(m_pBOMngr->GetBoundingObject(m_tableNames[i]));
	}

	for (int i = 0; i < m_numBalls; i++)
	{
		m_pBOMngr->AddObject(m_pMeshMngr->GetVertexList(m_ballNames[i]), m_ballNames[i]);
		m_pBOMngr->AddToAllBalls(m_pMeshMngr->GetVertexList(m_ballNames[i]), m_ballNames[i]);
		m_pBOMngr->GetBoundingObject(m_ballNames[i])->SetBall(true);
		m_allBallRot.push_back(quaternion());
		float xPos = 0.875f*(float)myTriMag->RowOnPascals(i + 1) - 1.0f - 1.75f*(float)myTriMag->PascalsAmountAboveRow(i + 1);
		float yPos = 0.0f;
		float zPos = -(float)myTriMag->RowOnPascals(i + 1);
		m_allBallPos.push_back(vector3(xPos, yPos, zPos));
		std::cout << "X: " << xPos << std::endl << "Y: " << yPos << std::endl << "Z: " << zPos << std::endl;
		m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->SetGravity(vector3(0.0f, -0.05f, 0.0f));
		allMatrices.push_back(m_pMeshMngr->GetModelMatrix(m_ballNames[i]));
		allBO.push_back(m_pBOMngr->GetBoundingObject(m_ballNames[i]));
		//allBallMatrices.push_back(m_pMeshMngr->GetVertexList(m_ballNames[i]));
	}

	m_backgroundOn = true;
	m_seeControls = true;
	m_GravitySux = false;
	whyWouldYouEverTurnThisOn = false;

	myOctree = new Octree(allMatrices, allBO, 2);

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

	m_pPhysicsMngr->GetPhysObject(m_ballNames[0])->SetBounce(vector3());

	//check collisions between balls and tables
	for (int i = 0; i < m_pBOMngr->GetAllBalls().size(); i++)
	{
		bool hitBall = false;
		bool hitTable = false;

		for (int j = i; j < m_numBalls; j++)
		{
			if (i != j && m_pBOMngr->GetAllBalls()[i]->IsColliding(m_pBOMngr->GetAllBalls()[j]))
			{
				//stop if you're colliding with a table
				if (m_shouldFall[i])
				{
					m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->SetVelocity(vector3());
					m_shouldFall[i] = false;
				}

				vector3 goDirection = m_allBallPos[i] - m_allBallPos[j];

				float sizeNormal = sqrt(goDirection.x*goDirection.x + goDirection.y*goDirection.y + goDirection.z*goDirection.z);

				vector3 normalizedGoDirection = goDirection / sizeNormal;

				m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->AddForce(normalizedGoDirection*m_ballForce);

				hitBall = true;
			}
		}

		for (int j = 0; j < m_pBOMngr->GetAllTables().size(); j++)
		{
			//if any collide with tables, get pushed from the table
			if (m_pBOMngr->GetAllBalls()[i]->IsColliding(m_pBOMngr->GetAllTables()[j]))
			{
				float speed = m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->GetVelocity().y;
				m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->SetForce(vector3());

				//stop if you're colliding with a table
				if (m_shouldFall[i])
				{
					m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->SetVelocity(vector3());
					m_shouldFall[i] = false;
				}

				//get info
				matrix4 table1Mat = m_pBOMngr->GetAllTables()[j]->GetModelMatrix();
				vector3 table1Max = m_pBOMngr->GetAllTables()[j]->GetMaxG();
				vector3 table1Min = m_pBOMngr->GetAllTables()[j]->GetMinG();

				vector3 localUpperRightForward = vector3(table1Max.x, table1Max.y, table1Min.z);
				vector3 localUpperRightBack = vector3(table1Max.x, table1Max.y, table1Max.z);
				vector3 localUpperLeftForward = vector3(table1Min.x, table1Max.y, table1Min.z);

				vector3 globalUpperRightForward = vector3(table1Mat * vector4(localUpperRightForward, 1.0f));
				vector3 globalUpperRightBack = vector3(table1Mat * vector4(localUpperRightBack, 1.0f));
				vector3 globalUpperLeftForward = vector3(table1Mat * vector4(localUpperLeftForward, 1.0f));

				//get the normal to the table
				vector3 normal = m_pBOMngr->GetAllTables()[j]->GetNormalToPlane(globalUpperRightForward, globalUpperRightBack, globalUpperLeftForward);

				float sizeNormal = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);

				//normalize the normal
				vector3 normalizedNormal = normal / sizeNormal;

				//std::cout << "X: " << normal.x << "\n" << "Y: " << normal.y << "\n" << "Z: " << normal.z << "\n";

				//add the normal force
				m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->AddForce(normalizedNormal*m_tableForce);

				hitTable = true;
			}
		}

		//otherwise, fall with gravity
		if (m_GravitySux)
		{
			if (!hitTable && !hitBall)
			{
				m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->AddForce(m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->GetGravity());
				m_shouldFall[i] = true;
			}
		}
		else
		{
			if (!hitTable /*&& !hitBall*/)
			{
				m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->AddForce(m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->GetGravity());
				m_shouldFall[i] = true;
			}
		}
	}

	for (int i = 0; i < m_numBalls; i++)
	{
		bool hitBall = false;
		
	}
		
	for (int i = 0; i < m_numBalls; i++)
	{
		m_allBallPos[i] = m_pPhysicsMngr->GetPhysObject(m_ballNames[i])->ApplyForces(m_allBallPos[i]);
	}

	

	//move the tables to their proper location
	//m_tableRotation is for chaos rotation
	//m_sceneRotation is for moving all the tables as one
	for (int i = 0; i < m_numTables; i++)
	{
		m_pMeshMngr->SetModelMatrix(glm::translate(ToMatrix4(m_sceneRotation), m_allTablePos[i]) * ToMatrix4(m_allTableRot[i]) * ToMatrix4(m_tableRotation), m_tableNames[i]);
		//m_pBOMngr->SetModelMatrix(m_pMeshMngr->GetModelMatrix(m_tableNames[i]), m_tableNames[i]);
	}

	//set the balls to their proper location
	for (int i = 0; i < m_numBalls; i++)
	{
		m_pMeshMngr->SetModelMatrix(glm::translate(IDENTITY_M4, m_allBallPos[i]) * ToMatrix4(m_allBallRot[i]), m_ballNames[i]);
		m_pBOMngr->SetModelMatrix(m_pMeshMngr->GetModelMatrix(m_ballNames[i]), m_ballNames[i]);
		m_pBOMngr->GetAllBalls()[i]->SetModelMatrix(m_pMeshMngr->GetModelMatrix(m_ballNames[i]));
	}

	

	for (int i = 0; i < m_numTables; i++)
	{
		m_pBOMngr->GetAllTables()[i]->SetModelMatrix(m_pMeshMngr->GetModelMatrix(m_tableNames[i]));
	}

	m_pBOMngr->Update();//Update collision detection

	if (whyWouldYouEverTurnThisOn)
	{
		myOctree->Render();
		myOctree->CheckCollisions();
	}

	//Print info on the screen

	//Adds all loaded instance to the render list
	if (!m_backgroundOn)
	{
		m_pMeshMngr->AddSkyboxToRenderList();
	}
	else
	{
		m_pMeshMngr->AddSkyboxToRenderList("barPh5.png");
	}

	//render everything
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	m_pMeshMngr->PrintLine("");//Add a line on top
	if (m_seeControls)
	{	
		m_pMeshMngr->Print("I,O,P: Rotate tables");
		m_pMeshMngr->Print("J,K,L: Rotation Chaos");
		m_pMeshMngr->Print("\nW, A, S, D, Q, E: Move camera");
		m_pMeshMngr->Print("\nRight click: Aim camera");
		m_pMeshMngr->Print("\nR: Reset tables");
		m_pMeshMngr->Print("\nB: Toggle background");
		m_pMeshMngr->Print("\nT: Toggle controls");
		m_pMeshMngr->Print("\nToggle GravitySux: G ("); if (m_GravitySux) { m_pMeshMngr->Print("On"); } else { m_pMeshMngr->Print("Off"); }; m_pMeshMngr->Print(")");
		if (!whyWouldYouEverTurnThisOn) 
		{ 
			m_pMeshMngr->Print("\nToggle Pointless Octree: M (Off)"); 
		}
		else 
		{
			m_pMeshMngr->Print("\nYou were warned. \nPress M to turn it back off now.");
		} 
		m_pMeshMngr->Print("\nAny number: Reset some balls.");
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
	//SafeDelete(m_ball1Phys);
	super::Release(); //release the memory of the inherited fields
	MyBOManager::ReleaseInstance();
	PhysicsManager::ReleaseInstance();

	delete myOctree;
	delete myTriMag;

	/*
	delete myOctree;
	for (int i = 0; i < allBO.size(); i++)
	{
		if (allBO[i] != nullptr)
		{
			allBO[i] = nullptr;
		}
		delete allBO[i];
	}
	*/
}
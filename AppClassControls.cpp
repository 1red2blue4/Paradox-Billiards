#include "AppClass.h"
void AppClass::ProcessKeyboard(void)
{
	bool bModifier = false;
	float fSpeed = 0.75f;
	float rotSpeed = 0.03f;
	

#pragma region ON_KEY_PRESS_RELEASE
	static bool	bLastF1 = false, bLastF2 = false, bLastF3 = false, bLastF4 = false, bLastF5 = false,
				bLastF6 = false, bLastF7 = false, bLastF8 = false, bLastF9 = false, bLastF10 = false,
				bLastEscape = false, bLastF = false;
#define ON_KEY_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion

#pragma region Modifiers
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		bModifier = true;
#pragma endregion

#pragma region Camera Positioning
	if(bModifier)
		fSpeed *= 10.0f;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_pCameraMngr->MoveForward(fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_pCameraMngr->MoveForward(-fSpeed);
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_pCameraMngr->MoveSideways(-fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_pCameraMngr->MoveSideways(fSpeed);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		m_pCameraMngr->MoveVertical(-fSpeed);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		m_pCameraMngr->MoveVertical(fSpeed);

	//rotate the scene
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
	{
		m_sceneRotation = m_sceneRotation * quaternion(vector3(rotSpeed, 0.0f, 0.0f));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			m_sceneRotation = m_sceneRotation * quaternion(vector3(-2.0f*rotSpeed, 0.0f, 0.0f));
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
	{
		m_sceneRotation = m_sceneRotation * quaternion(vector3(0.0f, rotSpeed, 0.0f));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			m_sceneRotation = m_sceneRotation * quaternion(vector3(0.0f, -2.0f*rotSpeed, 0.0f));
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		m_sceneRotation = m_sceneRotation * quaternion(vector3(0.0f, 0.0f, rotSpeed));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			m_sceneRotation = m_sceneRotation * quaternion(vector3(0.0f, 0.0f, -2.0f*rotSpeed));
		}
	}

	//rotate individual tables
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
	{
		m_tableRotation = m_tableRotation * quaternion(vector3(rotSpeed, 0.0f, 0.0f));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			m_tableRotation = m_tableRotation * quaternion(vector3(-2.0f*rotSpeed, 0.0f, 0.0f));
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
	{
		m_tableRotation = m_tableRotation * quaternion(vector3(0.0f, rotSpeed, 0.0f));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			m_tableRotation = m_tableRotation * quaternion(vector3(0.0f, -2.0f*rotSpeed, 0.0f));
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		m_tableRotation = m_tableRotation * quaternion(vector3(0.0f, 0.0f, rotSpeed));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			m_tableRotation = m_tableRotation * quaternion(vector3(0.0f, 0.0f, -2.0f*rotSpeed));
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) && pressTimedEvent == false)
	{
		m_backgroundOn = !m_backgroundOn;
		pressTimedEvent = true;
		timerTracker = controlTimer;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T) && pressTimedEvent == false)
	{
		m_seeControls = !m_seeControls;
		pressTimedEvent = true;
		timerTracker = controlTimer;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		m_sceneRotation = quaternion();
		m_tableRotation = quaternion();
		for (int i = 0; i < m_numBalls; i++)
		{
			float xPos = 0.875f*(float)myTriMag->RowOnPascals(i + 1) - 1.0f - 1.75f*(float)myTriMag->PascalsAmountAboveRow(i + 1);
			float yPos = 0.0f;
			float zPos = -(float)myTriMag->RowOnPascals(i + 1);
			m_allBallPos[i] = vector3(xPos, yPos, zPos);
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		m_sceneRotation = quaternion();
		m_tableRotation = quaternion();
		for (int i = 0; i < 1; i++)
		{
			float xPos = 0.875f*(float)myTriMag->RowOnPascals(i + 1) - 1.0f - 1.75f*(float)myTriMag->PascalsAmountAboveRow(i + 1);
			float yPos = 0.0f;
			float zPos = -(float)myTriMag->RowOnPascals(i + 1);
			m_allBallPos[i] = vector3(xPos, yPos, zPos);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		m_sceneRotation = quaternion();
		m_tableRotation = quaternion();
		for (int i = 0; i < 2; i++)
		{
			float xPos = 0.875f*(float)myTriMag->RowOnPascals(i + 1) - 1.0f - 1.75f*(float)myTriMag->PascalsAmountAboveRow(i + 1);
			float yPos = 0.0f;
			float zPos = -(float)myTriMag->RowOnPascals(i + 1);
			m_allBallPos[i] = vector3(xPos, yPos, zPos);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		m_sceneRotation = quaternion();
		m_tableRotation = quaternion();
		for (int i = 0; i < 3; i++)
		{
			float xPos = 0.875f*(float)myTriMag->RowOnPascals(i + 1) - 1.0f - 1.75f*(float)myTriMag->PascalsAmountAboveRow(i + 1);
			float yPos = 0.0f;
			float zPos = -(float)myTriMag->RowOnPascals(i + 1);
			m_allBallPos[i] = vector3(xPos, yPos, zPos);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		m_sceneRotation = quaternion();
		m_tableRotation = quaternion();
		for (int i = 0; i < 4; i++)
		{
			float xPos = 0.875f*(float)myTriMag->RowOnPascals(i + 1) - 1.0f - 1.75f*(float)myTriMag->PascalsAmountAboveRow(i + 1);
			float yPos = 0.0f;
			float zPos = -(float)myTriMag->RowOnPascals(i + 1);
			m_allBallPos[i] = vector3(xPos, yPos, zPos);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
	{
		m_sceneRotation = quaternion();
		m_tableRotation = quaternion();
		for (int i = 0; i < 5; i++)
		{
			float xPos = 0.875f*(float)myTriMag->RowOnPascals(i + 1) - 1.0f - 1.75f*(float)myTriMag->PascalsAmountAboveRow(i + 1);
			float yPos = 0.0f;
			float zPos = -(float)myTriMag->RowOnPascals(i + 1);
			m_allBallPos[i] = vector3(xPos, yPos, zPos);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
	{
		m_sceneRotation = quaternion();
		m_tableRotation = quaternion();
		for (int i = 0; i < 6; i++)
		{
			float xPos = 0.875f*(float)myTriMag->RowOnPascals(i + 1) - 1.0f - 1.75f*(float)myTriMag->PascalsAmountAboveRow(i + 1);
			float yPos = 0.0f;
			float zPos = -(float)myTriMag->RowOnPascals(i + 1);
			m_allBallPos[i] = vector3(xPos, yPos, zPos);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
	{
		m_sceneRotation = quaternion();
		m_tableRotation = quaternion();
		for (int i = 0; i < 7; i++)
		{
			float xPos = 0.875f*(float)myTriMag->RowOnPascals(i + 1) - 1.0f - 1.75f*(float)myTriMag->PascalsAmountAboveRow(i + 1);
			float yPos = 0.0f;
			float zPos = -(float)myTriMag->RowOnPascals(i + 1);
			m_allBallPos[i] = vector3(xPos, yPos, zPos);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
	{
		m_sceneRotation = quaternion();
		m_tableRotation = quaternion();
		for (int i = 0; i < 8; i++)
		{
			float xPos = 0.875f*(float)myTriMag->RowOnPascals(i + 1) - 1.0f - 1.75f*(float)myTriMag->PascalsAmountAboveRow(i + 1);
			float yPos = 0.0f;
			float zPos = -(float)myTriMag->RowOnPascals(i + 1);
			m_allBallPos[i] = vector3(xPos, yPos, zPos);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
	{
		m_sceneRotation = quaternion();
		m_tableRotation = quaternion();
		for (int i = 0; i < 9; i++)
		{
			float xPos = 0.875f*(float)myTriMag->RowOnPascals(i + 1) - 1.0f - 1.75f*(float)myTriMag->PascalsAmountAboveRow(i + 1);
			float yPos = 0.0f;
			float zPos = -(float)myTriMag->RowOnPascals(i + 1);
			m_allBallPos[i] = vector3(xPos, yPos, zPos);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) && !pressTimedEvent)
	{
		m_GravitySux = !m_GravitySux;
		pressTimedEvent = true;
		timerTracker = controlTimer;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M) && !pressTimedEvent)
	{
		whyWouldYouEverTurnThisOn = !whyWouldYouEverTurnThisOn;
		pressTimedEvent = true;
		timerTracker = controlTimer;
	}

	controlTimer++;

	//if the time has surpassed two seconds since the timed event, you can do the timed event again
	if (controlTimer > timerTracker + 15)
	{
		pressTimedEvent = false;
	}
	

#pragma endregion

#pragma region Other Actions
	ON_KEY_PRESS_RELEASE(Escape, NULL, PostMessage(m_pWindow->GetHandler(), WM_QUIT, NULL, NULL));
	ON_KEY_PRESS_RELEASE(F1, NULL, m_pCameraMngr->SetCameraMode(CAMPERSP));
	ON_KEY_PRESS_RELEASE(F2, NULL, m_pCameraMngr->SetCameraMode(CAMROTHOZ));
	ON_KEY_PRESS_RELEASE(F3, NULL, m_pCameraMngr->SetCameraMode(CAMROTHOY));
	ON_KEY_PRESS_RELEASE(F4, NULL, m_pCameraMngr->SetCameraMode(CAMROTHOX));
	static bool bFPSControll = false;
	ON_KEY_PRESS_RELEASE(F, bFPSControll = !bFPSControll, m_pCameraMngr->SetFPS(bFPSControll));
#pragma endregion
}
void AppClass::ProcessMouse(void)
{
	m_bArcBall = false;
	m_bFPC = false;
#pragma region ON_MOUSE_PRESS_RELEASE
	static bool	bLastLeft = false, bLastMiddle = false, bLastRight = false;
#define ON_MOUSE_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion
	bool bLeft = false;
	ON_MOUSE_PRESS_RELEASE(Left, NULL, bLeft = true)
	if (bLeft)
	{
		//Turn off the visibility of all BOs for all instances
		m_pMeshMngr->SetVisibleBO(BD_NONE, "ALL", -1);
		//Get the Position and direction of the click on the screen
		std::pair<vector3, vector3> pair =
			m_pCameraMngr->GetClickAndDirectionOnWorldSpace(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
		float fDistance = 0;//Stores the distance to the first colliding object
		m_selection = m_pMeshMngr->IsColliding(pair.first, pair.second, fDistance);

		//If there is a collision
		if (m_selection.first >= 0)
		{
			//Turn on the BO of the group
			m_pMeshMngr->SetVisibleBO(BD_OB, m_selection.first, m_selection.second);

			//Turn of the BO of the instance but not the group
			m_pMeshMngr->SetVisibleBO(BD_NONE, m_selection.first, -2);
		}
	}
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		m_bArcBall = true;
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		m_bFPC = true;
}

/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/08
----------------------------------------------*/

#include "AppClass.h"

//Initializing the main program using winappi
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//Creating a ReEngAppClass object providing the arguments and the window handler
	AppClass Application(hInstance, lpCmdLine, nCmdShow, false);
	//RE_RUNNING the Application's Main Loop
	Application.Run();
	//Finalizing the Application
	return 0;
	/*
	Sources for images:
	Bar image found at: https://media.timeout.com/images/100519023/image.jpg
	Ceiling image found at: http://files.gamebanana.com/img/ss/textures/517ab155e844c.jpg
	Floor image found at: http://files.gamebanana.com/img/ss/textures/517ab1434ab7a.jpg
	Table image found at: http://www.giltbarchicago.com/wp-content/uploads/IMG_0283-2.jpg
	*/
}
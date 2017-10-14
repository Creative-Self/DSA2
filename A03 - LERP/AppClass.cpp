#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/

	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		std::vector<vector3> Points;

		float dAngle = 360 / i;
		for (int j = 0; j < i; j++)
		{
			Points.push_back(vector3(glm::cos(glm::radians(dAngle * j)) * fSize, 0.0f, glm::sin(glm::radians(dAngle * j)) * fSize)); //calc the points to orbit
		}
		
		orbitsPos.push_back(Points); // pushing the points to orbit on each orbit 

		//each orbit has its own route
		int routez = 0;
		routes.push_back(routez);

		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength

	}

}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix

	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer
	float fTimeBetweenStops = 1.0;//in seconds
	float fPercentage = MapValue(fTimer, 0.0f, fTimeBetweenStops, 0.0f, 1.0f);

	// draw a shapes
	for (uint i = 0; i < m_uOrbits; i++)
	{	
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], mat);

		//calculate the current position 
		vector3 v3CurrentPos;
		vector3 v3Start; //start point
		vector3 v3End; //end point

		v3Start = orbitsPos[i][routes[i]];//start at the current route
		v3End = orbitsPos[i][(routes[i] + 1) % orbitsPos[i].size()]; //end at route +1 (if overboard will restart from 0)

		//calculate the current position
		v3CurrentPos = glm::lerp(v3Start, v3End, fPercentage);

		matrix4 m4Model = glm::translate(mat, v3CurrentPos);

		//if we are done with this route
		if (fPercentage >= 1.0f)
		{
			routes[i]++; //go to the next route
			fTimer = m_pSystem->GetDeltaTime(uClock);//restart the clock
			routes[i] %= orbitsPos[i].size(); //make sure we are within boundries
		}

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);	
	}

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}






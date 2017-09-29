#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
	
	//init the mesh
	m_pMesh = new MyMesh();

	for (int i = 0; i < 48; i++)
	{
		cubes[i] = new MyMesh();
		cubes[i]->GenerateCube(1.0f, C_WHITE);
	}
	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(3.0f,3.0f,3.0f));
	static float value = 0.0f;
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	 value += 0.01f;

	//matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model = m4Scale * m4Translate;

	m_pMesh->Render(m4Projection, m4View, m4Model);	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();
	//clear the render list
	m_pMeshMngr->ClearRenderList();

	static float v = 0.0f;

	cubePos[0] = glm::translate(IDENTITY_M4, vector3(6.0f + v, 10.0f, 3.0));
	cubePos[1] = glm::translate(IDENTITY_M4, vector3(0.0f + v, 10.0f, 3.0));

	cubePos[2] = glm::translate(IDENTITY_M4, vector3(5.0f + v, 9.0f, 3.0));
	cubePos[3] = glm::translate(IDENTITY_M4, vector3(1.0f + v, 9.0f, 3.0));

	cubePos[4] = glm::translate(IDENTITY_M4, vector3(6.0f + v, 8.0f, 3.0));
	cubePos[6] = glm::translate(IDENTITY_M4, vector3(5.0f + v, 8.0f, 3.0));
	cubePos[9] = glm::translate(IDENTITY_M4, vector3(4.0f + v, 8.0f, 3.0));
	cubePos[10] = glm::translate(IDENTITY_M4, vector3(3.0f + v, 8.0f, 3.0));
	cubePos[8] = glm::translate(IDENTITY_M4, vector3(2.0f + v, 8.0f, 3.0));
	cubePos[7] = glm::translate(IDENTITY_M4, vector3(1.0f + v, 8.0f, 3.0));
	cubePos[5] = glm::translate(IDENTITY_M4, vector3(0.0f + v, 8.0f, 3.0));
		
	cubePos[11] = glm::translate(IDENTITY_M4, vector3(7.0f + v, 7.0f, 3.0));
	cubePos[12] = glm::translate(IDENTITY_M4, vector3(6.0f + v, 7.0f, 3.0));
	cubePos[13] = glm::translate(IDENTITY_M4, vector3(0.0f + v, 7.0f, 3.0));
	cubePos[14] = glm::translate(IDENTITY_M4, vector3(4.0f + v, 7.0f, 3.0));
	cubePos[15] = glm::translate(IDENTITY_M4, vector3(3.0f + v, 7.0f, 3.0));
	cubePos[16] = glm::translate(IDENTITY_M4, vector3(2.0f + v, 7.0f, 3.0));
	cubePos[17] = glm::translate(IDENTITY_M4, vector3(-1.0f + v, 7.0f, 3.0));

	cubePos[18] = glm::translate(IDENTITY_M4, vector3(8.0f + v, 6.0f, 3.0));
	cubePos[19] = glm::translate(IDENTITY_M4, vector3(7.0f + v, 6.0f, 3.0));
	cubePos[20] = glm::translate(IDENTITY_M4, vector3(6.0f + v, 6.0f, 3.0));
	cubePos[21] = glm::translate(IDENTITY_M4, vector3(5.0f + v, 6.0f, 3.0));
	cubePos[22] = glm::translate(IDENTITY_M4, vector3(4.0f + v, 6.0f, 3.0));
	cubePos[23] = glm::translate(IDENTITY_M4, vector3(3.0f + v, 6.0f, 3.0));
	cubePos[24] = glm::translate(IDENTITY_M4, vector3(2.0f + v, 6.0f, 3.0));
	cubePos[25] = glm::translate(IDENTITY_M4, vector3(1.0f + v, 6.0f, 3.0));
	cubePos[26] = glm::translate(IDENTITY_M4, vector3(0.0f + v, 6.0f, 3.0));
	cubePos[27] = glm::translate(IDENTITY_M4, vector3(-1.0f + v, 6.0f, 3.0));
	cubePos[28] = glm::translate(IDENTITY_M4, vector3(-2.0f + v, 6.0f, 3.0));

	cubePos[29] = glm::translate(IDENTITY_M4, vector3(8.0f + v, 5.0f, 3.0));
	cubePos[30] = glm::translate(IDENTITY_M4, vector3(6.0f + v, 5.0f, 3.0));
	cubePos[31] = glm::translate(IDENTITY_M4, vector3(5.0f + v, 5.0f, 3.0));
	cubePos[32] = glm::translate(IDENTITY_M4, vector3(4.0f + v, 5.0f, 3.0));
	cubePos[33] = glm::translate(IDENTITY_M4, vector3(3.0f + v, 5.0f, 3.0));
	cubePos[34] = glm::translate(IDENTITY_M4, vector3(2.0f + v, 5.0f, 3.0));
	cubePos[35] = glm::translate(IDENTITY_M4, vector3(1.0f + v, 5.0f, 3.0));
	cubePos[36] = glm::translate(IDENTITY_M4, vector3(0.0f + v, 5.0f, 3.0));
	cubePos[37] = glm::translate(IDENTITY_M4, vector3(-2.0f + v, 5.0f, 3.0));

	cubePos[38] = glm::translate(IDENTITY_M4, vector3(8.0f + v, 4.0f, 3.0));
	cubePos[39] = glm::translate(IDENTITY_M4, vector3(6.0f + v, 4.0f, 3.0));
	cubePos[40] = glm::translate(IDENTITY_M4, vector3(-2.0f + v, 4.0f, 3.0));
	cubePos[41] = glm::translate(IDENTITY_M4, vector3(0.0f + v, 4.0f, 3.0));

	cubePos[42] = glm::translate(IDENTITY_M4, vector3(5.0f + v, 3.0f, 3.0));
	cubePos[43] = glm::translate(IDENTITY_M4, vector3(1.0f + v, 3.0f, 3.0));
	cubePos[44] = glm::translate(IDENTITY_M4, vector3(4.0f + v, 3.0f, 3.0));
	cubePos[45] = glm::translate(IDENTITY_M4, vector3(2.0f + v, 3.0f, 3.0));

	
	for (int i = 0; i < 46; i++)
	{
		matrix4 end = m4Scale * cubePos[i];

		cubes[i]->Render(m4Projection, m4View, cubePos[i]);	
	
	}

	v += 0.01f;

	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}
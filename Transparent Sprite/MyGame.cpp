#include "MyDirectX.h"

const string APPTITLE = "Render Mesh Demo";
const string ERRSTR = "Fuck!!!";
const int SCREENW = 1024;
const int SCREENH = 768;
#define STR_ERR ERRSTR.c_str()

DWORD screentimer = timeGetTime();
MODEL* mesh = NULL;

bool Game_Init(HWND window)
{
	srand(time(NULL));
	//initialize Direct3D
	if (!Direct3D_Init(window, SCREENW, SCREENH, false))
	{
		MessageBox(0, "Error initializing Direct3D", "Error", NULL);
		return false;
	}
	//initialize DirectInput
	if (!DirectInput_Init(window))
	{
		MessageBox(0, "Error initializing DirectInput", "Error", NULL);
		return false;
	}
	//initialize DirectSound
	if (!DirectSound_Init(window))
	{
		MessageBox(window, "Error initializing DirectSound", ERRSTR.c_str(), NULL);
		return false;
	}
	//create pointer to the back buffer
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	//set the camera position
	SetCamera(0.0f, -800.0f, -200.0f);
	//use ambient lighting and z-buffering
	d3ddev->SetRenderState(D3DRS_ZENABLE, true);
	d3ddev->SetRenderState(D3DRS_LIGHTING, false);
	//load the mesh file
	mesh = LoadModel("Fokker.X");
	if (mesh == NULL)
	{
		MessageBox(window, "Error loading mesh", STR_ERR, NULL);
		return 0;
	}

	return true;
}

void Game_Run(HWND window)
{
	//make sure the Direct3D device is vaild
	if (!d3ddev)	return;
	//update input devices
	DirectInput_Update();
	//clear the backbuffer
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	//slow rendering to approximately 60 fps
	if (timeGetTime() > screentimer + 14)
	{
		screentimer = GetTickCount();
		//start rendering
		if (d3ddev->BeginScene())
		{
			//rotate the view
			D3DXMATRIX matWorld;
			D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
			d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
			//draw the model
			DrawModel(mesh);
			//stop rendering
			d3ddev->EndScene();
			d3ddev->Present(NULL, NULL, NULL, NULL);
		}
	}

	//start rendering
	//if (d3ddev->BeginScene())
	//{
	//	d3ddev->EndScene();
	//	d3ddev->Present(NULL, NULL, NULL, NULL);
	//}

	//escape key exits
	if (Key_Down(DIK_SPACE) || Key_Down(DIK_ESCAPE))
		gameover = true;
	//controller back button also exits
	if (controllers[0].wButtons & XINPUT_GAMEPAD_BACK)
		gameover = true;
}

void Game_End()
{
	//free momory and shut down
	DeleteModel(mesh);
	//Direct component
	DirectSound_Shutdown();
	DirectInput_Shutdown();
	Direct3D_ShutDown();
}
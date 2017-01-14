#include "MyDirectX.h"
const string APPTITLE = "Sprite Rotaion and Scaling Demo";
const int SCREENW = 1440;
const int SCREENH = 900;

LPDIRECT3DTEXTURE9 sunflower = NULL;
int frame = 0, columns, width, height;
int startframe, ebdframe, starttime = 0, delay;
D3DCOLOR color;

bool Game_Init(HWND window)
{
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
	//load the sprite image
	sunflower = LoadTexture("sunflower.bmp");

	return true;
}

void Game_Run(HWND window)
{
	static float scale = 0.001f;
	static float r = 0;
	static float s = 1.0f;
	//make sure the Direct3D device is vaild
	if (!d3ddev)	return;
	//update input devices
	DirectInput_Update();

	//clear the backbuffer
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);
	//start rendering
	if (d3ddev->BeginScene())
	{
		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
		//set rotaion and scaling
		r = timeGetTime() / 600.f;
		s += scale;
		//draw sprite
		width = height = 512;
		frame = 0;
		columns = 1;
		color = D3DCOLOR_XRGB(255, 255, 255);
		Sprite_Transfrom_Draw(sunflower, 300, 150, width, height, frame, columns, r, s, color);
		//stop rendering
		spriteobj->End();

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	//escape key exits
	if (Key_Down(DIK_SPACE) || Key_Down(DIK_ESCAPE))
		gameover = true;
	//controller back button also exits
	if (controllers[0].wButtons & XINPUT_GAMEPAD_BACK)
		gameover = true;
}

void Game_End()
{
	DirectInput_Shutdown();
	Direct3D_ShutDown();
}
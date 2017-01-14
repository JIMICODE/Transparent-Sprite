#include "MyDirectX.h"
const string APPTITLE = "Sprite Rotation and Animation Demo";
const int SCREENW = 1440;
const int SCREENH = 900;

LPDIRECT3DTEXTURE9 paladin = NULL;
float scale = 0.004f;
float r = 0.0f;
float s = 1.0f;
int frame = 0, columns, width, height;
int startframe, endframe, starttime = 0, delay;
D3DCOLOR color = D3DCOLOR_XRGB(255,255,255);

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
	paladin = LoadTexture("paladin_walk.png");
	if (!paladin)
	{
		MessageBox(window, "Error loading sprite", "Fuck!!!", NULL);
		return false;
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
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);
	//start rendering
	if (d3ddev->BeginScene())
	{
		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
		//set scaling
		s += scale;
		if (s < 0.5f || s > 6.0f)	scale *= -1;
		//draw sprite`
		width = height = 96;
		columns = 8;
		startframe = 24;
		endframe = 31;
		delay = 90;
		Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);
		Sprite_Transfrom_Draw(paladin, 300, 200, width, height, frame, columns, 0, s, color);
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
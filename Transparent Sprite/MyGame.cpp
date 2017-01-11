#include "MyDirectX.h"
const string APPTITLE = "Transparent Sprite Demo";
const int SCREENW = 1024;
const int SCREENH = 768;
LPDIRECT3DTEXTURE9 image_colorkey = NULL;
LPDIRECT3DTEXTURE9 image_alpha = NULL;
LPDIRECT3DTEXTURE9 image_notrans = NULL;

int score = 0;
int vibrating = 0;

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

	//get the back buffer surface
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	//load non_transparent image
	image_notrans = LoadTexture("n1.bmp");
	if (!image_notrans) return false;
	//load color-keyed transparent image
	image_colorkey = LoadTexture("c1.bmp");
	if (!image_colorkey)	return false;
	//load_alpha transparent image
	image_alpha = LoadTexture("1.tga");
	if (!image_alpha)	return false;

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
		//startdrawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
		//draw the sprite
		D3DXVECTOR3 pos1(10, 10, 0);
		spriteobj->Draw(image_notrans, NULL, NULL, &pos1, D3DCOLOR_XRGB(255, 255, 255));
		D3DXVECTOR3 pos2(350, 10, 0);
		spriteobj->Draw(image_colorkey, NULL, NULL, &pos2, D3DCOLOR_XRGB(255, 255, 255));
		D3DXVECTOR3 pos3(700, 10, 0);
		spriteobj->Draw(image_alpha, NULL, NULL, &pos3, D3DCOLOR_XRGB(255, 255, 255));
		//stop drawing
		spriteobj->End();
		//stop rendering
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
	image_alpha->Release();
	image_notrans->Release();
	image_colorkey->Release();
}
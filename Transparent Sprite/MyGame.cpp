#include "MyDirectX.h"
#include<sstream>
const string APPTITLE = "Tile_Based Static Scrolling";
const int SCREENW = 1440;
const int SCREENH = 900;

LPD3DXFONT font;
//setting for the scroller
const int TILEWIDTH = 64;
const int TILEHEIGHT = 64;
const int MAPWIDTH = 25;
const int MAPHEIGHT = 18;
//scrolling window size
const int WINDOWWIDTH = (SCREENW / TILEWIDTH) * TILEWIDTH;
const int WINDOWHEIGHT = (SCREENH / TILEHEIGHT) * TILEHEIGHT;
//entire game world dimensions
const int GAMEWORLDWIDTH = TILEWIDTH * MAPWIDTH;
const int GAMEWORLDHEIGHT = TILEHEIGHT * MAPWIDTH;
int ScrollX, ScrollY;
int SpeedX, SpeedY;
long start;
LPDIRECT3DSURFACE9 gameworld = NULL;
int MAPDATA[MAPWIDTH*MAPHEIGHT] = {
	80,81,81,81,81,81,81,81,81,81,81,81,81,81,81,81,81,81,81,81,81,
	81,81,81,82,90,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,92,3,3,3,3,3,92,3,
	92,90,3,13,83,96,3,3,23,3,92,3,13,92,3,3,3,3,3,3,11,3,13,3,3,92,
	90,3,3,3,3,3,3,3,10,3,3,3,3,3,23,3,3,3,3,3,3,3,13,3,92,90,3,96,
	3,13,3,3,3,3,3,3,3,3,3,3,3,3,96,3,23,3,96,3,3,92,90,3,3,3,3,3,3,
	13,3,3,3,13,3,3,11,3,3,3,3,3,3,3,13,3,92,90,3,83,11,3,92,3,3,3,
	3,3,11,3,3,3,3,3,3,3,83,3,3,3,92,92,90,3,3,3,96,3,13,3,3,3,11,
	10,3,3,3,3,3,13,3,3,13,3,3,3,92,90,3,23,3,3,3,3,3,3,96,3,3,83,
	3,3,3,92,3,3,3,3,3,13,3,92,90,3,3,3,3,3,3,3,3,3,3,3,3,23,3,3,3,
	3,3,3,3,3,3,3,92,90,3,3,3,11,3,92,3,3,13,3,3,131,3,10,3,3,3,96,
	3,92,3,96,3,92,90,3,13,83,3,3,3,3,3,3,3,3,3,3,3,13,3,3,3,3,3,3,
	3,3,92,90,3,3,3,3,13,3,3,3,3,3,11,96,3,3,3,3,3,3,13,3,13,3,11,
	92,90,92,3,13,3,3,3,3,3,3,92,3,10,3,23,3,3,3,3,3,3,3,3,3,92,90,
	3,3,3,3,3,96,3,23,3,3,3,3,3,3,3,3,83,3,3,13,3,96,3,92,90,3,3,3,
	3,92,3,3,3,3,3,13,3,3,3,13,3,3,3,11,3,3,3,3,92,90,3,13,3,3,3,3,
	3,3,3,96,3,3,3,3,3,3,3,3,3,3,92,3,3,92,100,101,101,101,101,101,
	101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,
	101,101,102 };


void DrawTile(
	LPDIRECT3DSURFACE9 source,
	int tilenum,
	int width,
	int height,
	int columns,
	LPDIRECT3DSURFACE9 dest,
	int destx,
	int desty
)
{
	//create a RECT to describe the source image
	RECT r1;
	r1.left = (tilenum % columns) * width;
	r1.top = (tilenum / columns) * height;
	r1.right = r1.left + width;
	r1.bottom = r1.top + height;

	//set destination rect
	RECT r2 = { destx, desty, destx + width, desty + height };

	//draw the tile
	d3ddev->StretchRect(source, &r1, dest, &r2, D3DTEXF_NONE);
}

void BuildGameWorld()
{
	HRESULT result;
	int x, y;
	LPDIRECT3DSURFACE9 tiles;

	//load the bitmap image containing all the tiles
	tiles = LoadSurface("groundtiles.bmp");

	//create the scrolling game world bitmap
	result = d3ddev->CreateOffscreenPlainSurface(
		GAMEWORLDWIDTH,
		GAMEWORLDHEIGHT,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&gameworld,
		NULL
	);
	if (result != D3D_OK)
	{
		MessageBox(NULL, "Error creating working surface!", "Error", NULL);
		return;
	}
	//filll the game world bitmao with tiles
	for(y = 0; y < MAPHEIGHT; ++y)
		for (x = 0; x < MAPWIDTH; ++x)
		{
			DrawTile(tiles, MAPDATA[y * MAPWIDTH + x], 64, 64, 16,
				gameworld, x * 64, y * 64);
		}

	//now tiles bitmap is no longer needed
	tiles->Release();
}

void ScrollScreen()
{
	//update horizontal scrolling position and speed
	ScrollX += SpeedX;
	if (ScrollX < 0)
	{
		ScrollX = 0;
		SpeedX = 0;
	}
	else if (ScrollX > GAMEWORLDWIDTH - SCREENW)
	{
		ScrollX = GAMEWORLDWIDTH - SCREENW;
		SpeedX = 0;
	}
	//update vertical scrolling position and speed
	ScrollY += SpeedY;
	if (ScrollY < 0)
	{
		ScrollY = 0;
		SpeedY = 0;
	}
	else if (ScrollY > GAMEWORLDHEIGHT - SCREENH)
	{
		ScrollY = GAMEWORLDHEIGHT - SCREENH;
		SpeedY = 0;
	}
	//set dimensions of the source image
	RECT r1 = { ScrollX, ScrollY, ScrollX + SCREENW - 1, ScrollY + SCREENH - 1 };
	//set the destination rect
	RECT r2 = { 0,0,SCREENW - 1, SCREENH - 1 };

	//set the current game world view
	d3ddev->StretchRect(gameworld, &r1, backbuffer, &r2, D3DTEXF_NONE);
}

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
	//create pointer to the back buffer
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	//create a font
	font = MakeFont("Arial", 24);
	BuildGameWorld();
	start = GetTickCount();


	return true;
}

void Game_Run(HWND window)
{
	//make sure the Direct3D device is vaild
	if (!d3ddev)	return;
	//update input devices
	DirectInput_Update();

	//clear the backbuffer
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//scroll based on key or controller input
	if (Key_Down(DIK_DOWN) || controllers[0].sThumbLY < -2000)
		ScrollY += 1;
	if (Key_Down(DIK_UP) || controllers[0].sThumbLY > 2000)
		ScrollY -= 1;
	if (Key_Down(DIK_LEFT) || controllers[0].sThumbLX < -2000)
		ScrollX -= 1;
	if (Key_Down(DIK_RIGHT) || controllers[0].sThumbLX > 2000)
		ScrollX += 1;
	//keep the game running at steady frame rate
	if (GetTickCount() - start > 30)
	{
		//reset timing
		start = GetTickCount();
		//start rendering
		if (d3ddev->BeginScene())
		{
			//update the scrolling view
			ScrollScreen();
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
			std::ostringstream oss;
			oss << "Scroll Posiotion = " << ScrollX << "," << ScrollY;
			FontPrint(font, oss.str(), 0, 0);

			spriteobj->End();
			//stop rendering
			d3ddev->EndScene();
			d3ddev->Present(NULL, NULL, NULL, NULL);
		}
	}

	//start rendering
	/*if (d3ddev->BeginScene())
	{
		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		//stop rendering
		spriteobj->End();

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}*/

	//escape key exits
	if (Key_Down(DIK_SPACE) || Key_Down(DIK_ESCAPE))
		gameover = true;
	//controller back button also exits
	if (controllers[0].wButtons & XINPUT_GAMEPAD_BACK)
		gameover = true;
}

void Game_End()
{
	if (gameworld) gameworld->Release();
	DirectInput_Shutdown();
	Direct3D_ShutDown();
}
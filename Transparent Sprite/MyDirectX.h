#pragma once
//header file
#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800

#include<Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<dinput.h>
#include<Xinput.h>
#include<ctime>
#include<iostream>
#include<iomanip>
#include<sstream>
using namespace std;
//libraries
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")
//Program values
extern const string APPTITLE;
extern const int SCREENW;
extern const int SCREENH;
extern bool gameover;
//Direct3D objects
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;
extern LPD3DXSPRITE spriteobj;
//DIrect3D functions
bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen);
void Direct3D_ShutDown();
LPDIRECT3DSURFACE9 LoadSurface(string filename);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);
//DirectInput object, devices, and states
extern LPDIRECTINPUT8 input;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;
extern XINPUT_GAMEPAD controllers[4]; 
//DirectInput functions
bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
int Key_Down(int);
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();
void XInput_Vibrate(int contNum = 0, int amount = 65535);
bool XInput_Controller_Found();
//Direct3D texture and sprite
D3DXVECTOR2 GetBitmapSize(string filename);
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0,0,0));
//game function
bool Game_Init(HWND window);
void Game_Run(HWND window);
void Game_End();
//sprite ainimation function
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty,
	int framenum, int framew, int frameh, int colums);
void Sprite_Animate(int &frame, int startframe, int endframe,
	int direction, int &starttime, int delay);
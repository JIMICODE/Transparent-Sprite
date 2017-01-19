#include "MyDirectX.h"

const string APPTITLE = "Stock Mesh Demo";
const string ERRSTR = "Fuck!!!";
const int SCREENW = 1024;
const int SCREENH = 768;
#define STR_ERR ERRSTR.c_str()

D3DXMATRIX mProj, mView, mWorld, mViewProj;
D3DXMATRIX mTrans, mRot, mScale;
D3DXVECTOR3 vTrans, vRot, vScale;
LPD3DXMESH torus = NULL;
ID3DXEffect *shader1 = NULL;

void SetCamera(float x, float y, float z)
{
	double p_fov = D3DX_PI / 4.0;
	double p_aspectRatio = 1024 / 768;
	double p_nearRange = 1.0;
	double p_farRange = 2000.0;
	D3DXVECTOR3 p_updir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 p_position = D3DXVECTOR3(x, y, z);
	D3DXVECTOR3 p_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 p_target = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//set the camera's view and perspective matrix
	D3DXMatrixPerspectiveFovLH(&mProj,
		(float)p_fov,
		(float)p_aspectRatio,
		(float)p_nearRange,
		(float)p_farRange);
	D3DXMatrixLookAtLH(&mView, &p_position, &p_target, &p_updir);
	//optimization
	mViewProj = mView * mProj;
}

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

	//create a torus mesh
	D3DXCreateTorus(d3ddev, 0.5f, 1.0f, 40, 40, &torus, NULL);
	//set the camera position
	SetCamera(0.0f, 1.0f, -20.0f);
	//load the effect files
	ID3DXBuffer *errors = 0;
	D3DXCreateEffectFromFile(d3ddev, "shader.fx", 0, 0, D3DXSHADER_DEBUG, 0,
		&shader1, &errors);
	if (errors)
	{
		MessageBox(0, (char*)errors->GetBufferPointer(), ERRSTR.c_str(), 0);
		return 0;
	}
	//set the default technique
	shader1->SetTechnique("technique1");
	return true;
}

void Game_Run(HWND window)
{
	UINT numPasses = 0;
	static float y = 0.0f;
	//make sure the Direct3D device is vaild
	if (!d3ddev)	return;
	//update input devices
	DirectInput_Update();
	//clear the backbuffer
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	//transform the mesh
	y += 0.001;
	D3DXMatrixRotationYawPitchRoll(&mRot, y, 0.0f, 0.0f);
	D3DXMatrixTranslation(&mTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&mScale, 4.0f, 4.0f, 4.0f);
	//pass the matirx to the shader via a parameter
	mWorld = mRot * mTrans * mScale;
	shader1->SetMatrix("matWorld", &mWorld);
	shader1->SetMatrix("matViewProj", &mViewProj);
	//start rendering
	if (d3ddev->BeginScene())
	{
		shader1->Begin(&numPasses, 0);
		for (int i = 0; i < numPasses; ++i)
		{
			shader1->BeginPass(i);
			torus->DrawSubset(0);
			shader1->EndPass();
		}
		shader1->End();

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
	torus->Release();
	shader1->Release();
	//Direct component
	DirectSound_Shutdown();
	DirectInput_Shutdown();
	Direct3D_ShutDown();
}
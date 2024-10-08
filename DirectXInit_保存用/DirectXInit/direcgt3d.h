#pragma once
#include <d3d11.h>  // DirectX11を使うためのヘッダーファイル
#include <DirectXMath.h>

#define SCREEN_WIDTH (640)	// ウインドウの幅
#define SCREEN_HEIGHT (480)	// ウインドウの高さ

// 関数のプロトタイプ宣言
HRESULT D3D_Create(HWND hwnd);
void    D3D_Release();
void    D3D_StartRender();
void    D3D_FinishRender();

struct Vertex//頂点データを表す構造体
{
	float x, y, z;//場所

	float r, g, b, t;//色

	float u, v;//切り抜き
};

struct ConstBuffer
{
	//頂点カラー行列
	DirectX::XMFLOAT4 color;
	//UV座標移動行列
	DirectX::XMMATRIX matrixTex;
	//プロジェクト変換行列
	DirectX::XMMATRIX matrixProj;
	//ワールド変換行列
	DirectX::XMMATRIX matrixWorld;
};

extern ID3D11Device* g_pDevice;
extern ID3D11DeviceContext* g_pDeviceContext;
extern ID3D11Buffer* g_pConstantBuffer;//定数バッファを他ファイルと共有するためにextern句を利用してる

#define SAFE_RELEASE(p){if(NULL != p){p->Release();p = NULL;}}
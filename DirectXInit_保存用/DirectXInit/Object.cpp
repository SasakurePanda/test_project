#include "Object.h"
void Object::Init(const wchar_t* imgname,int sx,int sy)
{
	splitX = sx;
	splitY = sy;
	vertexList[1].u = 1.0f / splitX;
	vertexList[2].v = 1.0f / splitY;
	vertexList[3].u = 1.0f / splitX;
	vertexList[3].v = 1.0f / splitY;

	//頂点バッファを制作する
	//※頂点バッファ→VRAMに頂点データを置くための機能
	D3D11_BUFFER_DESC bufferDesc;///サイトだとvbDesc
	bufferDesc.ByteWidth = sizeof(vertexList);//確保するバッファサイズを指定
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//頂点バッファ制作を指定

	bufferDesc.CPUAccessFlags= 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem = vertexList;// VRAMに送るデータを指定
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	HRESULT hr = g_pDevice->CreateBuffer(&bufferDesc, &subResourceData, &m_pVertexBuffer);

	//テクスチャ読み込み
	hr = DirectX::CreateWICTextureFromFile(g_pDevice,imgname, NULL, &m_pTextureView);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "テクスチャ読み込み失敗", "エラー", MB_ICONERROR | MB_OK);

		return ;
	}
}

void Object::Draw()
{
	UINT strides = sizeof(Vertex);
	UINT offsets = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);
	g_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);

	//定数バッファを更新
	ConstBuffer cb;
	//プロジェクション変換行列
	cb.matrixProj = DirectX::XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 3.0f);
	cb.matrixProj = DirectX::XMMatrixTranspose(cb.matrixProj);

	//ワールド変換行列の制作
	//→オブジェクトの位置・大きさ・向きを指定
	cb.matrixWorld = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	cb.matrixWorld *= DirectX::XMMatrixRotationZ(angle * 3.14f / 180);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);

	//UVアニメーションの行列作成
	float u = (float)numU / splitX;
	float v = (float)numV / splitY;
	cb.matrixTex = DirectX::XMMatrixTranslation(u, v, 0.0f);
	cb.matrixTex = DirectX::XMMatrixTranspose(cb.matrixTex);

	//頂点カラーのデータ制作
	cb.color = color;

	//行列シェーダーに渡す
	g_pDeviceContext->UpdateSubresource(g_pConstantBuffer,0,NULL,&cb,0,0);


	g_pDeviceContext->Draw(4, 0); // 描画命令

}

void Object::Uninit()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pTextureView);
}

void Object::SetPos(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void Object::SetSize(float x, float y, float z)
{
	size.x = x;
	size.y = y;
	size.z = z;
}

void Object::SetAngle(float a)
{
	angle = a;
}

void Object::SetColor(float r, float g, float b, float a)
{
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = a;
}

DirectX::XMFLOAT3 Object::GetPos(void)
{
	return pos;
}

DirectX::XMFLOAT3 Object::GetSize(void)
{
	return size;
}

float Object::GetAngle(void)
{
	return angle;
}

DirectX::XMFLOAT4 Object::GetColor(void)
{
	return color;
}


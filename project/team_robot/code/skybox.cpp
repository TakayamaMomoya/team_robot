//*****************************************************
//
// スカイボックスの処理[skybox.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "skybox.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float CHANGE_TEX_LINE = 100000.0f;	// テクスチャを変えるライン
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CSkybox *CSkybox::m_pSkybox = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CSkybox::CSkybox(int nPriority) : CObjectX(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CSkybox::~CSkybox()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSkybox::Init(void)
{
	// 継承クラスの初期化
	CObjectX::Init();

	// モデルの読込
	int nIdx = CModel::Load("data\\MODEL\\skybox\\sky00.x");
	BindModel(nIdx);

	EnableLighting(true);
	EnableFog(false);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSkybox::Uninit(void)
{
	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSkybox::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();

	// プレイヤーの追従
	FollowPlayer();

	// テクスチャ変更の検知
	DetectionChangeTexture();
}

//=====================================================
// プレイヤーの追従
//=====================================================
void CSkybox::FollowPlayer(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	SetPosition(pPlayer->GetPosition());
}

//=====================================================
// テクスチャ変更の検知
//=====================================================
void CSkybox::DetectionChangeTexture(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 movePlayer = pPlayer->GetMove();
	D3DXVECTOR3 posPlayerNext = posPlayer + movePlayer;

	if (posPlayer.x < CHANGE_TEX_LINE && posPlayerNext.x >= CHANGE_TEX_LINE)
	{
		// テクスチャを峠にする
		CModel::Model *pModel = CModel::GetModel(GetIdxModel());

		if (pModel->pIdxTexture == nullptr)
			return;

		int nIdx = Texture::GetIdx("data\\TEXTURE\\BG\\sky01.jpg");

		*pModel->pIdxTexture = nIdx;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CSkybox::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CRenderer::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ライティングを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 継承クラスの描画
	CObjectX::Draw();

	// ライティングを有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=====================================================
// 生成処理
//=====================================================
CSkybox *CSkybox::Create()
{
	CSkybox *pSkybox = nullptr;

	if (pSkybox == nullptr)
	{
		pSkybox = new CSkybox;

		if (pSkybox != nullptr)
		{
			// 初期化
			pSkybox->Init();
		}
	}

	return pSkybox;
}
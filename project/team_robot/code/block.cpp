//*****************************************************
//
// ブロックの処理[block.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "block.h"
#include "blockManager.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "effect3D.h"
#include "fan3D.h"
#include "polygon3D.h"
#include "player.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float SPEED_MOVE(1.0f);	// 移動速度
	const char* MAP_FILE = "data\\MAP\\map01.bin";	// マップのファイルパス
	const float RADIUS_DRIFT_DEFAULT = 1000.0f;	// ドリフト半径のデフォルト値
	const float POLYGON_SIZE_X = 100.0f;        // 鉤縄ポイントUIのサイズ
	const float POLYGON_SIZE_Y = 100.0f;        // 鉤縄ポイントUIのサイズ
	const float STANDARD_LENGHT = 7000.0f;      // 鉤縄ポイントUIが出る距離
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CBlock::m_nNumAll = 0;	// 総数

//=====================================================
// コンストラクタ
//=====================================================
CBlock::CBlock(int nPriority)
{
	m_fLife = 0.0f;
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_nIdx = -1;

	// 先頭、最後尾アドレス取得
	CBlockManager *pManager = CBlockManager::GetInstance();
	CBlock *pHead = nullptr;
	CBlock *pTail = nullptr;

	if (pManager != nullptr)
	{
		pHead = pManager->GetHead();
		pTail = pManager->GetTail();
	}

	if (pHead == nullptr)
	{// 先頭と最後尾アドレスの代入
		pManager->SetHead(this);
		pManager->SetTail(this);

		return;
	}

	// 前のアドレスに最後尾のアドレスを代入する
	m_pPrev = pTail;

	// 最後尾のアドレスを自分にする
	pManager->SetTail(this);

	if (m_pPrev != nullptr)
	{
		// 前のオブジェクトの次のアドレスを自分にする
		m_pPrev->m_pNext = this;
	}

	m_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CBlock::~CBlock()
{
	// 先頭、最後尾アドレス取得
	CBlockManager *pManager = CBlockManager::GetInstance();
	CBlock *pHead = nullptr;
	CBlock *pTail = nullptr;

	if (pManager != nullptr)
	{
		pHead = pManager->GetHead();
		pTail = pManager->GetTail();
	}

	m_nNumAll--;

	if (pTail != this && pHead != this)
	{// 真ん中のアドレスの破棄
		if (m_pPrev != nullptr)
		{
			// 前のアドレスから次のアドレスをつなぐ
			m_pPrev->m_pNext = m_pNext;
		}

		if (m_pNext != nullptr)
		{
			// 次のアドレスから前のアドレスをつなぐ
			m_pNext->m_pPrev = m_pPrev;
		}
	}

	if (pHead == this)
	{// 先頭アドレスの破棄
		//if (m_pNext != nullptr)
		{// 先頭アドレスを次のアドレスに引き継ぐ
			pManager->SetHead(m_pNext);

			if (m_pNext != nullptr)
			{
				m_pNext->m_pPrev = nullptr;
			}
		}
	}

	if (pTail == this)
	{// 最後尾アドレスの破棄
		if (m_pPrev != nullptr)
		{// 最後尾アドレスを前のアドレスに引き継ぐ
			pManager->SetTail(m_pPrev);

			m_pPrev->m_pNext = nullptr;
		}
	}

	m_nNumAll--;
}

//=====================================================
// 生成処理
//=====================================================
CBlock *CBlock::Create(BEHAVIOUR behaviour)
{
	CBlock *pBlock = nullptr;

	// インスタンス生成
	switch (behaviour)
	{
	case BEHAVIOUR_NORMAL:
		pBlock = new CBlock;
		break;
	case BEHAVIOUR_GRAB:
		pBlock = new CBlockGrab;
		break;
	default:
		assert(("ブロックのビヘイビアーに不正な値が入力されました", false));
		break;
	}

	if (pBlock != nullptr)
	{
		// 初期化処理
		pBlock->Init();
	}

	return pBlock;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBlock::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 継承クラスの初期化
	CObjectX::Init();

	// タイプの設定
	SetType(TYPE_BLOCK);

	m_fLife = 300.0f;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBlock::Uninit(void)
{
	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CBlock::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();
}

//=====================================================
// 位置設定
//=====================================================
void CBlock::SetPosition(D3DXVECTOR3 pos)
{
	CObjectX::SetPosition(pos);
}

//=====================================================
// 描画処理
//=====================================================
void CBlock::Draw(void)
{
	// 継承クラスの描画
	CObjectX::Draw();
}

//=====================================================
// 保存処理
//=====================================================
void CBlock::Save(FILE *pFile)
{
	fprintf(pFile, "SETBLOCK\n");

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	int nIdx = GetIdx();

	fprintf(pFile, " IDX = %d\n", nIdx);
	fprintf(pFile, " POS = %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
	fprintf(pFile, " ROT = %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);
}

//=====================================================
// 読込処理
//=====================================================
void CBlock::Load(FILE *pFile, char* pTemp)
{
	if (strcmp(pTemp, "IDX") == 0)
	{// インデックス
		int nIdx;

		CBlockManager *pBlockManager = CBlockManager::GetInstance();

		if (pBlockManager != nullptr)
		{
			(void)fscanf(pFile, "%s", pTemp);

			(void)fscanf(pFile, "%d", &nIdx);

			SetIdx(nIdx);

			CBlockManager::SInfoBlock *InfoBlock = pBlockManager->GetInfoBlock();

			BindModel(InfoBlock[nIdx].nIdxModel);
		}
	}

	if (strcmp(pTemp, "POS") == 0)
	{// 位置
		D3DXVECTOR3 pos;

		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &pos.x);
		(void)fscanf(pFile, "%f", &pos.y);
		(void)fscanf(pFile, "%f", &pos.z);

		SetPosition(pos);
	}

	if (strcmp(pTemp, "ROT") == 0)
	{// 向き
		D3DXVECTOR3 rot;

		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &rot.x);
		(void)fscanf(pFile, "%f", &rot.y);
		(void)fscanf(pFile, "%f", &rot.z);

		SetRotation(rot);
	}
}

//=====================================================
// 当たり判定処理
//=====================================================
bool CBlock::Collide(D3DXVECTOR3* pPos, D3DXVECTOR3 posOld)
{
	float fHeight = pPos->y;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 BlockCorner[4];
	D3DXVECTOR3 BlockNor;
	D3DXVECTOR3 BlockMax = GetVtxMax();
	D3DXVECTOR3 BlockMin = GetVtxMin();

	BlockCorner[0] = universal::PosRelativeMtx(pos, rot, D3DXVECTOR3(BlockMax.x, BlockMax.y, BlockMin.z));
	BlockCorner[1] = universal::PosRelativeMtx(pos, rot, D3DXVECTOR3(BlockMin.x, BlockMax.y, BlockMin.z));
	BlockCorner[2] = universal::PosRelativeMtx(pos, rot, D3DXVECTOR3(BlockMin.x, BlockMax.y, BlockMax.z));
	BlockCorner[3] = universal::PosRelativeMtx(pos, rot, D3DXVECTOR3(BlockMax.x, BlockMax.y, BlockMax.z));

	BlockNor = universal::NorRelativeMtx(pos, rot, BlockMax, BlockMin);

	// ポリゴンの下に入っているか判定する
	if (D3DXVec3Length(&(pos - *pPos)) > 10000.0f)
		return false;

	if (!universal::IsOnSquare(BlockCorner[0], BlockCorner[1], BlockCorner[2], BlockCorner[3], BlockNor, *pPos, posOld, fHeight))
		return false;

	// 高さが一定の高さ以内か判定する
	if (200.0f < fHeight - pPos->y)
		return false;

	pPos->y = fHeight;

	return true;
}

//============================================================================
// 掴めるブロッククラス
//============================================================================
//=====================================================
// コンストラクタ
//=====================================================
CBlockGrab::CBlockGrab() : m_bCurrent(false), m_fRadiusOffset(0.0f),m_pFan(nullptr)
{
	for (int i = 0; i < NUM_OFFSET; i++)
	{
		m_afAngleOffset[i] = 0.0f;
	}

	m_pGrabPointUI = nullptr;
}
 
//=====================================================
// デストラクタ
//=====================================================
CBlockGrab::~CBlockGrab()
{

}

//=====================================================
// 初期化
//=====================================================
HRESULT CBlockGrab::Init(void)
{
	// 掴みブロックリストに追加
	CBlockManager *pBlockManager =  BlockManager::GetInstance();

	if (pBlockManager != nullptr)
		pBlockManager->AddGrabList(this);

	// 基底クラスの初期化
	CBlock::Init();

	// オフセットの初期化
	m_afAngleOffset[0] = D3DX_PI;
	m_afAngleOffset[1] = D3DX_PI * 0.7f;
	m_fRadiusOffset = 1000.0f;

#ifdef _DEBUG
	// 判定可視化用の扇生成
	if (m_pFan == nullptr)
	{
		m_pFan = CFan3D::Create();
		
		if (m_pFan != nullptr)
		{
			m_pFan->SetRotation(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
			m_pFan->SetRadius(400.0f);
			m_pFan->SetVtx();

			SetFan();
		}
	}
#endif

	return S_OK;
}

//=====================================================
// 終了
//=====================================================
void CBlockGrab::Uninit(void)
{
	// 掴みブロックリストから除外
	CBlockManager *pBlockManager = BlockManager::GetInstance();

	if (pBlockManager != nullptr)
		pBlockManager->RemoveGrabList(this);
	
	// 基底クラスの終了
	CBlock::Uninit();
}

//=====================================================
// 更新
//=====================================================
void CBlockGrab::Update(void)
{
	CBlock::Update();

	// 鉤縄ポイントのUI表示
	GrabPointUI();

#ifdef _DEBUG
	SetFan();	// 扇ポリゴンの設定
#endif
}

//=====================================================
// 描画
//=====================================================
void CBlockGrab::Draw(void)
{
	if (m_bCurrent)
	{
		CEffect3D::Create(GetPosition(), 200.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	CBlock::Draw();
}

//=====================================================
// 掴めるかの判定
//=====================================================
bool CBlockGrab::CanGrab(D3DXVECTOR3 pos)
{
	bool bCanGrab1 = true;
	bool bCanGrab2 = true;

	// 判定の設置
	D3DXMATRIX mtxVec1;
	D3DXMATRIX mtxVec2;
	D3DXMATRIX mtx = GetMatrix();

	// オフセットの設定
	D3DXVECTOR3 offset1 = { sinf(m_afAngleOffset[0]) * m_fRadiusOffset,0.0f,cosf(m_afAngleOffset[0]) * m_fRadiusOffset };
	D3DXVECTOR3 offset2 = { sinf(m_afAngleOffset[1]) * m_fRadiusOffset,0.0f,cosf(m_afAngleOffset[1]) * m_fRadiusOffset };

	universal::SetOffSet(&mtxVec1, mtx, offset1);
	universal::SetOffSet(&mtxVec2, mtx, offset2);

	D3DXVECTOR3 posMtx1 = { mtxVec1._41,mtxVec1._42 ,mtxVec1._43 };
	D3DXVECTOR3 posMtx2 = { mtxVec2._41,mtxVec2._42 ,mtxVec2._43 };

	bCanGrab1 = universal::IsCross(pos, posMtx1, GetPosition(), nullptr);
	bCanGrab2 = universal::IsCross(pos, GetPosition(), posMtx2, nullptr);

#if 0
	CDebugProc::GetInstance()->Print("\n掴める1[%d]", bCanGrab1);
	CDebugProc::GetInstance()->Print("\n掴める2[%d]", bCanGrab2);
#endif

	bool bOK = bCanGrab1 && bCanGrab2;

#ifdef _DEBUG
	CEffect3D::Create(posMtx1, 100.0f, 3, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	CEffect3D::Create(posMtx2, 100.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	
	if (bCanGrab1)
	{
		posMtx1.y += 100.0f;
		CEffect3D::Create(posMtx1, 100.0f, 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	
	if (bCanGrab2)
	{
		posMtx2.y += 100.0f;
		CEffect3D::Create(posMtx2, 100.0f, 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (bOK)
	{
		CEffect3D::Create(GetPosition(), 150.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
#endif

	return bOK;
}

//=====================================================
// 扇ポリゴンの設定
//=====================================================
void CBlockGrab::SetFan(void)
{
	// オフセットの設定
	D3DXMATRIX mtxVec1;
	D3DXMATRIX mtxVec2;
	D3DXMATRIX mtx = GetMatrix();

	D3DXVECTOR3 offset1 = { sinf(m_afAngleOffset[0]) * m_fRadiusOffset,0.0f,cosf(m_afAngleOffset[0]) * m_fRadiusOffset };
	D3DXVECTOR3 offset2 = { sinf(m_afAngleOffset[1]) * m_fRadiusOffset,0.0f,cosf(m_afAngleOffset[1]) * m_fRadiusOffset };

	universal::SetOffSet(&mtxVec1, mtx, offset1);
	universal::SetOffSet(&mtxVec2, mtx, offset2);

	D3DXVECTOR3 posMtx1 = { mtxVec1._41,mtxVec1._42 ,mtxVec1._43 };
	D3DXVECTOR3 posMtx2 = { mtxVec2._41,mtxVec2._42 ,mtxVec2._43 };

	CEffect3D::Create(posMtx1, 100.0f, 3, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	CEffect3D::Create(posMtx2, 100.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// 扇の角度の設定
	if (m_pFan != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		float fAngleOffset1 = atan2f(posMtx1.x - pos.x, posMtx1.z - pos.z);
		float fAngleOffset2 = atan2f(posMtx2.x - pos.x, posMtx2.z - pos.z);

		float fDiff = fAngleOffset1 - fAngleOffset2;

		float fRate = fDiff / (D3DX_PI * 2.0f);

		if (fRate < 0)
			fRate = 1.0f + fRate;

		m_pFan->SetRateAngle(fRate);

		pos.y += 10.0f;
		m_pFan->SetPosition(pos);
		m_pFan->SetRotation(D3DXVECTOR3(D3DX_PI * 0.5f, fAngleOffset2, 0.0f));
		m_pFan->SetVtx();
	}
}

//=====================================================
// 位置設定
//=====================================================
void CBlockGrab::SetPosition(D3DXVECTOR3 pos)
{
	CBlock::SetPosition(pos);

#ifdef _DEBUG
	Draw();

	if (m_pFan != nullptr)
	{
		SetFan();
	}
#endif
}

//=====================================================
// 向きの設定
//=====================================================
void CBlockGrab::SetRotation(D3DXVECTOR3 rot)
{
	CBlock::SetRotation(rot);

#ifdef _DEBUG
	Draw();

	if (m_pFan != nullptr)
	{
		SetFan();
	}
#endif
}

//=====================================================
// 鉤縄ポイントUI表示処理
//=====================================================
void CBlockGrab::GrabPointUI(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		// プレイヤーと鉤縄ポイントの距離
		float fLenght = (pos.x + pos.z) - (PlayerPos.x + PlayerPos.z);
		fLenght = fabs(fLenght);

		if (fLenght < STANDARD_LENGHT)
		{// 範囲内

			if (m_pGrabPointUI == nullptr)
			{
				// UIの設定
				m_pGrabPointUI = CPolygon3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\FK.png");
				m_pGrabPointUI->SetIdxTexture(nIdx);
				D3DXVECTOR3 vtxmax = GetVtxMax();
				m_pGrabPointUI->SetPosition(D3DXVECTOR3(pos.x, pos.y + vtxmax.y, pos.z));
				m_pGrabPointUI->SetMode(CPolygon3D::MODE_BILLBOARD);
				m_pGrabPointUI->SetSize(POLYGON_SIZE_X, POLYGON_SIZE_Y);
				m_pGrabPointUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));
				m_pGrabPointUI->SetVtx();
				m_pGrabPointUI->EnableZtest(true);
			}

			if(m_fColA <= 1.0f)
			   m_fColA += 0.1f;

			m_pGrabPointUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));
		}
		else
		{// 範囲外

			if (m_pGrabPointUI != nullptr)
			{
				if (m_fColA >= 0.0f)
					m_fColA -= 0.1f;

				m_pGrabPointUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));

				if (m_fColA <= 0.0f)
				{
					m_pGrabPointUI->Uninit();
					m_pGrabPointUI = nullptr;
				}
			}
		}
	}
}

//=====================================================
// 保存処理
//=====================================================
void CBlockGrab::Save(FILE *pFile)
{
	fprintf(pFile, "SETGRABBLOCK\n");

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	int nIdx = GetIdx();

	fprintf(pFile, " IDX = %d\n", nIdx);
	fprintf(pFile, " POS = %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
	fprintf(pFile, " ROT = %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);

	fprintf(pFile, " ANGLE_OFFSET = %.2f %.2f\n", m_afAngleOffset[0], m_afAngleOffset[1]);
	fprintf(pFile, " RADIUS_OFFSET = %.2f\n", m_fRadiusOffset);
}

//=====================================================
// 読込処理
//=====================================================
void CBlockGrab::Load(FILE *pFile, char* pTemp)
{
	// 共通の読込
	CBlock::Load(pFile, pTemp);

	if (strcmp(pTemp, "ANGLE_OFFSET") == 0)
	{// オフセットの角度
		(void)fscanf(pFile, "%s", pTemp);

		for (int i = 0; i < NUM_OFFSET; i++)
		{
			(void)fscanf(pFile, "%f", &m_afAngleOffset[i]);
		}
	}

	if (strcmp(pTemp, "RADIUS_OFFSET") == 0)
	{// オフセットの半径
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_fRadiusOffset);
	}
}
//*****************************************************
//
// リザルトのBehavior[resultBehavior.cpp]
// Author:高山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "resultBehavior.h"
#include "number.h"
#include "ranking.h"
#include "inputManager.h"
#include "fade.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int NUM_PLACE = 6;
const D3DXVECTOR3 POS_NUMBER[CResultPlayerScore::TYPE_MAX] =
{// 初期表示スコアの数値の位置
	{ SCRN_MID.x, SCRN_MID.y - 250.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y - 130.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 30.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 210.0f, 0.0f },
};
const D3DXVECTOR2 SIZE_NUMBER = { 25.0f,50.0f };	// 数字のサイズ
const int COST_REPAIR = 50;	// 損傷率1%における修理費用
const char* PATH_TEXTURE_CAPTION[CResultPlayerScore::TYPE_MAX] =
{// 項目の見出しのテクスチャパス
	"data\\TEXTURE\\UI\\resultCaption00.png",
	"data\\TEXTURE\\UI\\resultCaption01.png",
	"data\\TEXTURE\\UI\\resultCaption02.png",
	"data\\TEXTURE\\UI\\resultCaption03.png",
};
const D3DXVECTOR2 SIZE_CAPTION = { 70.0f,50.0f };	// 項目見出しのサイズ
}

//=====================================================
// コンストラクタ
//=====================================================
CResultBehavior::CResultBehavior()
{

}

//=====================================================
// デストラクタ
//=====================================================
CResultBehavior::~CResultBehavior()
{

}

//=====================================================
// プレイヤースコア表示
//=====================================================
CResultPlayerScore::CResultPlayerScore()
{// コンストラクタ
	ZeroMemory(&m_apNumber[0], sizeof(m_apNumber));
	ZeroMemory(&m_apCaption[0], sizeof(m_apCaption));
	m_pFrame = nullptr;
}

CResultPlayerScore::~CResultPlayerScore()
{// デストラクタ

}

void CResultPlayerScore::Init(CRanking *pResult)
{// 初期化

}

void CResultPlayerScore::Uninit(CRanking *pResult)
{// 終了
	for (int i = 0; i < TYPE_NUMBER::TYPE_MAX; i++)
	{
		if (m_apNumber[i] != nullptr)
		{
			m_apNumber[i]->Uninit();
			m_apNumber[i] = nullptr;
		}

		if (m_apCaption[i] != nullptr)
		{
			m_apCaption[i]->Uninit();
			m_apCaption[i] = nullptr;
		}
	}

	if (m_pFrame != nullptr)
	{
		m_pFrame->Uninit();
		m_pFrame = nullptr;
	}
}

void CResultPlayerScore::Update(CRanking *pResult)
{// 更新
	// フェードする操作
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{
		if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
		{
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_TITLE);
			}
		}

		return;
	}
}

//=====================================================
// ランキング表示
//=====================================================
CResultRanking::CResultRanking()
{// コンストラクタ

}

CResultRanking::~CResultRanking()
{// デストラクタ

}

void CResultRanking::Init(CRanking *pResult)
{// 初期化

}

void CResultRanking::Update(CRanking *pResult)
{// 更新
	// フェードする操作
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{
		if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
		{
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_TITLE);
			}
		}

		if (pInputManager->GetTrigger(CInputManager::BUTTON_BACK))
		{// 自スコア表示に戻る
			pResult->ChangeBehavior(new CResultPlayerScore);

			return;
		}
	}
}
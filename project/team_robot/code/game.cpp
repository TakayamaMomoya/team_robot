//*****************************************************
//
// Q[[game.cpp]
// Author:ûüRç
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "manager.h"
#include "game.h"
#include "inputkeyboard.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "debugproc.h"
#include "UIManager.h"
#include "skybox.h"
#include "pause.h"
#include "player.h"
#include "slow.h"
#include "blockManager.h"
#include "CameraState.h"
#include "meshCube.h"

//*****************************************************
// }Nè`
//*****************************************************
#define TRANS_TIME	(100)	// I¹ÜÅÌ]CÌt[
namespace
{
const char* PATH_GAME_ROAD = "data\\MAP\\road00.bin";	// Q[bV[hÌpX
const D3DXVECTOR3 POS_MESHFIELD = { 342028.0f,1000.0f, -30640.0f };	// bVtB[hÌÊu
}

//*****************************************************
// ÃIoÏé¾
//*****************************************************
CGame::STATE CGame::m_state = STATE_NONE;	// óÔ
CGame *CGame::m_pGame = nullptr;	// ©gÌ|C^

//=====================================================
// RXgN^
//=====================================================
CGame::CGame()
{
	m_nCntState = 0;
	m_bStop = false;
	m_pEdit = nullptr;
}

//=====================================================
// ú»
//=====================================================
HRESULT CGame::Init(void)
{
	m_pGame = this;

	m_state = STATE_NORMAL;
	m_bStop = false;

	// UI}l[W[ÌÇÁ
	CUIManager::Create();

	// BGMÄ¶
	Sound::Play(CSound::LABEL_BGM_GAME01);

	// vC[Ì¶¬
	CPlayer::Create();

	// X[ÇÌ¶¬
	CSlow::Create();

	// XJC{bNX¶¬
	CSkybox::Create();

	return S_OK;
}

//=====================================================
// I¹
//=====================================================
void CGame::Uninit(void)
{
	// IuWFNgSü
	CObject::ReleaseAll();

	m_pGame = nullptr;
}

//=====================================================
// XV
//=====================================================
void CGame::Update(void)
{
	if (!m_bStop)
	{
		// V[ÌXV
		CScene::Update();
	}

	// JXV
	UpdateCamera();

	// óÔÇ
	ManageState();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// JÌXV
//=====================================================
void CGame::UpdateCamera(void)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	pCamera->Update();
}

//=====================================================
// óÔÇ
//=====================================================
void CGame::ManageState(void)
{
	CFade *pFade = CFade::GetInstance();

	switch (m_state)
	{
	case CGame::STATE_NORMAL:
		break;

		break;
	case CGame::STATE_END:

		m_nCntState++;

		if (m_nCntState >= TRANS_TIME && pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_RANKING);
		}

		break;
	default:
		break;
	}
}

//=====================================================
// â~óÔÌØèÖ¦
//=====================================================
void CGame::ToggleStop(void)
{
	m_bStop = m_bStop ? false : true;

	if (m_bStop)
	{
		Camera::ChangeState(new CMoveControl);
	}
	else
	{
		Camera::ChangeState(new CFollowPlayer);
	}
}

//=====================================================
// fobO
//=====================================================
void CGame::Debug(void)
{
	// üÍæ¾
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr)
	{
		return;
	}

	if (pKeyboard->GetTrigger(DIK_F))
	{// â~óÔÌØèÖ¦
		ToggleStop();
	}
}

//=====================================================
// `æ
//=====================================================
void CGame::Draw(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	char *apString[STATE::STATE_MAX] =
	{
		"NONE",
		"NORMAL",
		"END",
	};
}
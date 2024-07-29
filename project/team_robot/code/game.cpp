//*****************************************************
//
// �Q�[������[game.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
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

//*****************************************************
// �}�N����`
//*****************************************************
#define TRANS_TIME	(100)	// �I���܂ł̗]�C�̃t���[����
namespace
{
const char* PATH_GAME_ROAD = "data\\MAP\\road00.bin";	// �Q�[�����b�V�����[�h�̃p�X
const D3DXVECTOR3 POS_MESHFIELD = { 342028.0f,1000.0f, -30640.0f };	// ���b�V���t�B�[���h�̈ʒu
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CGame::STATE CGame::m_state = STATE_NONE;	// ���
CGame *CGame::m_pGame = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CGame::CGame()
{
	m_nCntState = 0;
	m_bStop = false;
	m_pEdit = nullptr;
}

//=====================================================
// ����������
//=====================================================
HRESULT CGame::Init(void)
{
	m_pGame = this;

	m_state = STATE_NORMAL;
	m_bStop = false;

	// UI�}�l�[�W���[�̒ǉ�
	CUIManager::Create();

	// �X�J�C�{�b�N�X�̐���
	CSkybox::Create();

	// BGM�Đ�
	Sound::Play(CSound::LABEL_BGM_GAME01);

	// �v���C���[�̐���
	CPlayer::Create();

	// �X���[�Ǘ��̐���
	CSlow::Create();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGame::Uninit(void)
{
	// �I�u�W�F�N�g�S��
	CObject::ReleaseAll();

	m_pGame = nullptr;
}

//=====================================================
// �X�V����
//=====================================================
void CGame::Update(void)
{
	if (!m_bStop)
	{
		// �V�[���̍X�V
		CScene::Update();
	}

	// �J�����X�V
	UpdateCamera();

	// ��ԊǗ�
	ManageState();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �J�����̍X�V
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
// ��ԊǗ�
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
// ��~��Ԃ̐؂�ւ�
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
// �f�o�b�O����
//=====================================================
void CGame::Debug(void)
{
	// ���͎擾
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr)
	{
		return;
	}

	if (pKeyboard->GetTrigger(DIK_F))
	{// ��~��Ԃ̐؂�ւ�
		ToggleStop();
	}
}

//=====================================================
// �`�揈��
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
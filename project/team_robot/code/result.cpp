//*****************************************************
//
// ���U���g����[result.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "result.h"
#include "player.h"
#include "camera.h"
#include "manager.h"
#include "cameraState.h"
#include "inputManager.h"
#include "game.h"
#include "number.h"
#include "timer.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_DISP_TIME = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.4f, 0.0f };	// �^�C���̕\���ʒu
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CResult::CResult() : CObject(1), m_pState(nullptr)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CResult::~CResult()
{

}

//=====================================================
// ��������
//=====================================================
CResult *CResult::Create(void)
{
	CResult *pResult = nullptr;

	pResult = new CResult;

	if (pResult != nullptr)
	{
		pResult->Init();
	}

	return pResult;
}

//=====================================================
// ����������
//=====================================================
HRESULT CResult::Init(void)
{
	// �v���C���[�̐ݒ�
	SetPlayer();

	// �J�����ʒu�̐ݒ�
	SetCamera();

	// �X�e�C�g�̕ύX
	ChangeState(new CStateResultDispTime);

	return S_OK;
}

//=====================================================
// �v���C���[�ݒ�
//=====================================================
void CResult::SetPlayer(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;
	
	// �v���C���[�𑀍�ł��Ȃ��悤�ɂ���
	pPlayer->SetEnableInput(false);
}

//=====================================================
// �J�����ݒ�
//=====================================================
void CResult::SetCamera(void)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return;
	
	pCamera->ChangeState(new CCameraStateResult);
}

//=====================================================
// �I������
//=====================================================
void CResult::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CResult::Update(void)
{
	if (m_pState != nullptr)
	{
		m_pState->Update(this);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CResult::Draw(void)
{

}

//=====================================================
// �X�e�C�g�̕ύX
//=====================================================
void CResult::ChangeState(CStateResult *pState)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;

	if (m_pState != nullptr)
	{
		m_pState->Init(this);
	}
}

//=====================================================
// �t�F�[�h���n�߂鏈��
//=====================================================
void CResult::StartFade(void)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame == nullptr)
		return;

	pGame->SetState(CGame::STATE::STATE_END);
}

//=====================================================
// �X�e�C�g�̏I��
//=====================================================
void CStateResult::Uninit(CResult *pResult)
{

}

//********************************************************************************
// �^�C���\��
//********************************************************************************
//=====================================================
// �R���X�g���N�^
//=====================================================
CStateResultDispTime::CStateResultDispTime() : m_apTime()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CStateResultDispTime::~CStateResultDispTime()
{

}

//=====================================================
// ������
//=====================================================
void CStateResultDispTime::Init(CResult *pResult)
{
	// �����̐ݒ�
	SetNumber();
}

//=====================================================
// �����̐ݒ�
//=====================================================
void CStateResultDispTime::SetNumber(void)
{
	CTimer *pTime = CTimer::GetInstance();

	if (pTime == nullptr)
		return;

	int aTime[NUMBER_MAX] =
	{
		pTime->GetMinutes(),
		(int)pTime->GetSecond(),
		(int)pTime->GetMilli()
	};

	for (int i = 0; i < NUMBER_MAX; i++)
	{
		m_apTime[i] = CNumber::Create(2, aTime[i]);


	}
}

//=====================================================
// �I��
//=====================================================
void CStateResultDispTime::Uninit(CResult *pResult)
{
	CStateResult::Uninit(pResult);
}

//=====================================================
// �X�V
//=====================================================
void CStateResultDispTime::Update(CResult *pResult)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	// �t�F�[�h���n�߂�
	if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
		pResult->StartFade();
}
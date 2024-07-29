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
#include "object.h"
#include "inputkeyboard.h"
#include "inputManager.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "scene.h"
#include "debugproc.h"
#include "UIManager.h"
#include "polygon3D.h"
#include "texture.h"
#include "skybox.h"
#include "block.h"
#include "renderer.h"
#include "animEffect3D.h"
#include "pause.h"
#include "player.h"
#include "slow.h"
#include "blockManager.h"
#include "meshfield.h"
#include "CameraState.h"
#include "particle.h"
#include "timer.h"
#include "meter.h"
#include "meshRoad.h"
#include "goal.h"
#include "edit.h"
#include "editMesh.h"
#include "editMeshfield.h"
#include "editBlock.h"
#include "editGoal.h"
#include "enemy.h"
#include "meshCube.h"
#include "enemySpawner.h"
#include "tutorial.h"
#include "blur.h"

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

	// �RD�A�j���[�V�����Ǘ��̐���
	CAnimEffect3D::Create();

	// BGM�Đ�
	Sound::Play(CSound::LABEL_BGM_GAME01);

	// �t�H�O��������
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(true);
	}

	// 
	CBlockManager::Create();

	// �v���C���[�̐���
	CPlayer::Create();

	// �X���[�Ǘ��̐���
	CSlow::Create();

	// ���[�^�[����
	CMeter::Create();

	// �S�[������
	//CGoal::Create(D3DXVECTOR3(432987.3f, -1721.7f, -301192.4f), D3DX_PI);
	CGoal::Create(D3DXVECTOR3(12726.0f, 2500.7f, -27695.0f), D3DX_PI);

	// ���b�V�����[�h����
	CMeshRoad::Create(PATH_GAME_ROAD);

	// �`���[�g���A���̐���
	CTutorial::Create();

#ifdef _DEBUG
	// ���b�V���L���[�u�̃e�X�g����
	CMeshCube::Create();
#endif

	// ���b�V���t�B�[���h�̐���
	CMeshField *pMeshField = CMeshField::Create();

	if (pMeshField != nullptr)
	{
		pMeshField->SetPosition(POS_MESHFIELD);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGame::Uninit(void)
{
	if (m_pEdit != nullptr)
	{
		m_pEdit->Uninit();
		delete m_pEdit;
		m_pEdit = nullptr;
	}

	// �I�u�W�F�N�g�S��
	CObject::ReleaseAll();

	m_pGame = nullptr;
}

//=====================================================
// �X�V����
//=====================================================
void CGame::Update(void)
{
	CFade *pFade = CFade::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();
	CSound* pSound = CSound::GetInstance();

	// �V�[���̍X�V
	CScene::Update();

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
// �G�f�B�^�[�̕ύX
//=====================================================
void CGame::ChangeEdit(CEdit *pEdit)
{
	if (m_pEdit != nullptr)
	{
		m_pEdit->Uninit();
		delete m_pEdit;
		m_pEdit = nullptr;
	}

	m_pEdit = pEdit;

	if (m_pEdit != nullptr)
	{
		m_pEdit->Init();
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

	if (pKeyboard->GetTrigger(DIK_F2))
	{// �G�f�B�b�g�폜
		ToggleStop();
	}

	ImGui::Text("[EditMode]");

	if (ImGui::Button("MeshRoad", ImVec2(70, 30)))	// ���b�V�����[�h�G�f�B�b�g
		ChangeEdit(new CEditMesh);

	if (ImGui::Button("MeshField", ImVec2(70, 30)))	// ���b�V���t�B�[���h�G�f�B�b�g
		ChangeEdit(new CEditMeshfield);

	if (ImGui::Button("Block", ImVec2(70, 30)))	// �u���b�N�G�f�B�b�g
		ChangeEdit(new CEditBlock);

	if (ImGui::Button("Goal", ImVec2(70, 30)))	// �S�[���G�f�B�b�g
		ChangeEdit(new CEditGoal);

	if (m_pEdit != nullptr)
		m_pEdit->Update();
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
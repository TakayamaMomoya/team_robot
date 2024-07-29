//*****************************************************
//
// �v���C���[�̏���[player.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "player.h"
#include "manager.h"
#include "cameraState.h"
#include "camera.h"
#include "slow.h"
#include "inputManager.h"
#include "inputjoypad.h"
#include "inputkeyboard.h"
#include "pause.h"
#include "debugproc.h"
#include "blockManager.h"
#include "effect3D.h"
#include "polygon3D.h"
#include "blur.h"
#include "renderer.h"
#include "game.h"
#include "MyEffekseer.h"
#include "sound.h"
#include "orbit.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const std::string PATH_PARAM = "data\\TEXT\\playerParam.txt";	// �p�����[�^�[�f�[�^�̃p�X
const float NOTROTATE = 1.0f;		// ��]���Ȃ��悤�ɂ���l
const float DIST_LIMIT = 5000.0f;	// ���C���[��������
const float LINE_CORRECT_DRIFT = 40.0f;	// �h���t�g�␳�̂������l
const float SIZE_BLUR = -20.0f;	// �u���[�̃T�C�Y
const float DENSITY_BLUR = 0.5f;	// �u���[�̔Z��
const float SE_CHANGE_SPEED = 10.0f;  // �G���W�����ƃA�N�Z�������؂�ւ�鑬�x�̒l
const float HANDLE_INERTIA = 0.03f;  // �J�[�u���̊p�x�ύX����
const float HANDLE_INERTIA_RESET = 0.07f;  // �̐��p�x���Z�b�g���̊p�x�ύX�����{��
const float HANDLE_INERTIA_DRIFT = 0.08f;  // �h���t�g���̊p�x�ύX�����{��
const float HANDLE_INERTIA_DEFAULT = 0.1f;  // �h���t�g�p������ʏ�p���ɖ߂鎞�̊p�x�ύX�����{��
const float HANDLE_CURVE_MAG = -0.04f;  // �̐�����J�[�u�ւ̔{��
const float SIZE_SPEEDBLUR = 13.0f;	// �X�s�[�h�u���[�̃T�C�Y
const float DENSITY_SPEEDBLUR = 0.3f;	// �X�s�[�h�u���[�̔Z��
const float ROT_CURVE_LIMIT = 0.02f;  // �n���h�����삪�����p�ɂȂ�p�x�̌��E
const float ROT_Z_DRIFT = 1.0f;  // �h���t�g����Z���̊p�x
const float GRAVITY = -0.2f;  // �d�͂̔{��
const float GRAVITY_GROUND = -9.0f;  // �ڒn���̏d��
const float HEIGH_FRONT_WHEEL = 55.0f;  // �O�ւ̍���
const float HEIGH_REAR_WHEEL = 65.0f;  // ��ւ̍���
const float ROT_BIKE_FRONT_LIMIT = 1.5f;  // �O���̊p�x���E
const float ROT_BIKE_REAR_LIMIT = -1.35f;  // �����̊p�x���E
const float ROT_AIRBIKE_MAG = 0.015f;  // �󒆂ł̉�]�{��
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CPlayer *CPlayer::s_pPlayer = nullptr;	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	s_pPlayer = this;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayer::~CPlayer()
{

}

//=====================================================
// ��������
//=====================================================
CPlayer *CPlayer::Create(void)
{
	if (s_pPlayer == nullptr)
	{
		s_pPlayer = new CPlayer;

		if (s_pPlayer != nullptr)
		{
			s_pPlayer->Init();
		}
	}

	return s_pPlayer;
}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayer::Init(void)
{
	// �u���[�������Ȃ��ݒ�ɂ���
	EnableBlur(false);

	// �p���N���X�̏�����
	CMotion::Init();

	SetMotion(MOTION_WALK_FRONT);

	// �Ǎ�
	Load();

	// ���f���̐ݒ�
	CMotion::Load(&m_param.m_stPathBody[0]);

	// �����g�����X�t�H�[���̐ݒ�
	SetPosition(Player::DEFAULT_POS);
	SetRotation(Player::DEFAULT_ROT);
	MultiplyMtx(false);	// �K�w�\���̃}�g���b�N�X���������킹��

	// �J�����̍s���ݒ�
	Camera::ChangeState(new CFollowPlayer);
	Camera::SkipToDest();	// �ڕW�ʒu�܂ŃJ�������΂�

	// �T�E���h�C���X�^���X�̎擾
	CSound* pSound = CSound::GetInstance();

	return S_OK;
}

//=====================================================
// �Ǎ�����
//=====================================================
void CPlayer::Load(void)
{
	std::ifstream file(PATH_PARAM);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// �ǂݍ��ނ��̂��Ȃ��Ȃ�܂œǍ�
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "BODY_PATH")
			{// �����ڂ̃p�X
				iss >> m_param.m_stPathBody;
			}

			if (file.eof())
			{// �ǂݍ��ݏI��
				break;
			}
		}
		
		file.close();
	}
	else
	{
		assert(("�����L���O�t�@�C�����J���܂���ł���", false));
	}
}

//=====================================================
// �I������
//=====================================================
void CPlayer::Uninit(void)
{
	s_pPlayer = nullptr;

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayer::Update(void)
{
#ifdef _DEBUG
	if (CGame::GetInstance()->GetStop())
		return;
#endif

	// ����
	Input();

	// �O��̈ʒu��ۑ�
	D3DXVECTOR3 pos = GetPosition();
	SetPositionOld(pos);

	// �p���N���X�̍X�V
	CMotion::Update();

// �f�o�b�O����
#if _DEBUG
	Debug();
#endif // _DEBUG
}

//=====================================================
// ���͏���
//=====================================================
void CPlayer::Input(void)
{
	// �ړ�����
	InputMove();

	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{// �|�[�Y�̔���
		if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
		{
			CPause::Create();
		}
	}
}

//=====================================================
// �ړ��̓���
//=====================================================
void CPlayer::InputMove(void)
{
	CSlow *pSlow = CSlow::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	// �T�E���h�C���X�^���X�̎擾
	CSound* pSound = CSound::GetInstance();

	if (pInputManager == nullptr)
	{
		return;
	}
}

//=====================================================
// �C�x���g�̊Ǘ�
//=====================================================
void CPlayer::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };

}

//=====================================================
// �`�揈��
//=====================================================
void CPlayer::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}

//=====================================================
// �q�b�g����
//=====================================================
void CPlayer::Hit(float fDamage)
{

}

//=====================================================
// �f�o�b�O�\��
//=====================================================
void CPlayer::Debug(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

#if 1
	pDebugProc->Print("\n�v���C���[�̈ʒu[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	pDebugProc->Print("\n�v���C���[�̈ړ���[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);
	pDebugProc->Print("\n�v���C���[�̌���[%f,%f,%f]", GetRotation().x, GetRotation().y, GetRotation().z);
#endif
}
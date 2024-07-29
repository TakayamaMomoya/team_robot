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
#include "meshRoad.h"
#include "game.h"
#include "MyEffekseer.h"
#include "sound.h"
#include "guardRail.h"
#include "playerNinja.h"
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
CPlayer *CPlayer::m_pPlayer = nullptr;	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	m_pPlayer = this;

	ZeroMemory(&m_info, sizeof(CPlayer::SInfo));
	ZeroMemory(&m_param, sizeof(CPlayer::SParam));
	ZeroMemory(&m_fragMotion, sizeof(CPlayer::SFragMotion));
	ZeroMemory(&m_fragNinja, sizeof(CPlayer::SFragNinja));
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
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CPlayer;

		if (m_pPlayer != nullptr)
		{
			m_pPlayer->Init();
		}
	}

	return m_pPlayer;
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
	CMotion::Load(&m_param.aPathBody[0]);

	// �o�C�N�ɏ���Ă���E�҂̏������ƃ��f���̐ݒ�
	if (m_pPlayerNinja == nullptr)
	{
		m_pPlayerNinja = CPlayerNinja::Create();

		if (m_pPlayerNinja != nullptr)
		{
			m_pPlayerNinja->SetMatrix(GetMatrix());
		}
	}

	m_info.pRoap = CPolygon3D::Create(GetPosition());

	// �f�t�H���g�l�ݒ�
	m_info.fLengthDrift = 1500.0f;
	m_info.bGrabOld = true;
	m_info.fDesityBlurDrift = DENSITY_BLUR;
	m_info.fSizeBlurDrift = SIZE_BLUR;

	// �����g�����X�t�H�[���̐ݒ�
	SetPosition(Player::DEFAULT_POS);
	SetRotation(Player::DEFAULT_ROT);

	MultiplyMtx(false);	// �K�w�\���̃}�g���b�N�X���������킹��

	// �J�����̍s���ݒ�
	Camera::ChangeState(new CFollowPlayer);

	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return E_FAIL;

	pCamera->SkipToDest();	// �ڕW�ʒu�܂ŃJ�������΂�

	// �e�[�������v�p�O�Ղ̐���
	m_info.orbitColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_info.pOrbitLamp = COrbit::Create(GetMatrix(), D3DXVECTOR3(20.0f, 220.0f, -80.0f), D3DXVECTOR3(-20.0f, 220.0f, -80.0f), m_info.orbitColor, 60);

	// �T�E���h�C���X�^���X�̎擾
	CSound* pSound = CSound::GetInstance();

	// �G���W����SE�Đ�
	if (pSound != nullptr)
		pSound->Play(pSound->LABEL_SE_ENGIN);

	m_info.bEnableInput = true;

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
				iss >> m_param.aPathBody;
			}

			if (key == "SPEED_MAX")
			{// �ő呬�x
				iss >> m_param.fSpeedMax;
				m_param.fSpeedMaxInitial = m_param.fSpeedMax;
			}

			if (key == "FACT_ACCELE")
			{// �����W��
				iss >> m_param.fFactAccele;
			}

			if (key == "FACT_ATTENU")
			{// �����W��
				iss >> m_param.fFactAttenu;
			}

			if (key == "FACT_BRAKE")
			{// �u���[�L�����W��
				iss >> m_param.fFactBrake;
			}

			if (key == "ANGLE_MAX_CURVE")
			{// �J�[�u�ő�p�x
				iss >> m_param.fAngleMaxCurve;
			}

			if (key == "COLLIDER_SIZE")
			{// �v���C���[�����蔻��T�C�Y
				iss >> m_param.sizeCollider.x >> m_param.sizeCollider.y >> m_param.sizeCollider.z;
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
	m_pPlayer = nullptr;
	m_info.pOrbitLamp->Uninit();

	// �p���N���X�̏I��
	CMotion::Uninit();

	// �T�E���h�̒�~
	// �T�E���h�C���X�^���X�̎擾
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
		pSound->Stop();
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

	if (m_info.bEnableInput)
	{
		// ����
		Input();

		//�����蔻��
		Collision();
	}

	// �O��̈ʒu��ۑ�
	D3DXVECTOR3 pos = GetPosition();
	SetPositionOld(pos);

	// ��ԊǗ�
	ManageState();

	// ���[�V�����Ǘ�
	ManageMotion();

	// �E�҂̃��[�V�����Ǘ�
	ManageMotionNinja();

	// �p���N���X�̍X�V
	CMotion::Update();

	if (m_pPlayerNinja != nullptr)
	{// �o�C�N�ɏ�����E�҂̒Ǐ]
		m_pPlayerNinja->SetPosition(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
		CObject3D::Draw();
		m_pPlayerNinja->SetMatrixParent(GetMatrix());
	}

	// �X�s�[�h�ɂ��u���[�̊Ǘ�
	ManageSpeedBlur();

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

	// ���C���[�̑���
	InputWire();

	// ���̑���
	InputKatana();

	// �X�s�[�h�̊Ǘ�
	ManageSpeed();

	// �a���G�t�F�N�g�̊Ǘ�
	ManageSlashEffect();

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

	// �A�N�Z���l�̎擾
	float fAccele = pInputManager->GetAccele();

	m_info.fSpeedDest = fAccele * m_param.fSpeedMax;

#if 0
	CDebugProc::GetInstance()->Print("\n�A�N�Z���l[%f]", fAccele);
#endif

	if (m_info.pBlockGrab == nullptr)
	{
		// �n���h���̑���
		CInputManager::SAxis axis = pInputManager->GetAxis();
		m_info.fAngleHandle = axis.axisMove.x;
	}

	// �u���[�L�l�̎擾
	float fBrake = pInputManager->GetBrake();

	m_info.fSpeed += (0.0f - m_info.fSpeed) * m_param.fFactBrake * fBrake;

	if (m_info.fSpeed < SE_CHANGE_SPEED && m_bMove)
	{
		// �A�N�Z��SE����G���W��SE�֕ύX
		if (pSound != nullptr)
		{
			pSound->Play(pSound->LABEL_SE_ENGIN);
			pSound->Stop(pSound->LABEL_SE_ACCELERATOR);
		}

		m_bMove = false;
	}
	else if (m_info.fSpeed >= SE_CHANGE_SPEED && !m_bMove)
	{
		// �G���W��SE����A�N�Z��SE�֕ύX
		if (pSound != nullptr)
		{
			pSound->Play(pSound->LABEL_SE_ACCELERATOR);
			pSound->Stop(pSound->LABEL_SE_ENGIN);
		}

		m_bMove = true;
	}

#if 0
	CDebugProc::GetInstance()->Print("\n�u���[�L�l[%f]", fBrake);
#endif
}

//=====================================================
// ���C���[�̑���
//=====================================================
void CPlayer::InputWire(void)
{
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	if (pJoypad == nullptr)
		return;

	D3DXVECTOR2 vecStickR =
	{
		pJoypad->GetJoyStickRX(0),
		pJoypad->GetJoyStickRY(0)
	};

	// �X�e�B�b�N���͊p�x
	float fAngleInput = atan2f(vecStickR.x, vecStickR.y);

	// ���͊p�x�ƃJ�����p�x�𑫂����p�x
	D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetCamera()->rot;

	float fAngle = fAngleInput + rotCamera.y + D3DX_PI;
	universal::LimitRot(&fAngle);

	// �X�e�B�b�N���͂̋���
	float fLength = sqrtf(vecStickR.x * vecStickR.x + vecStickR.y * vecStickR.y);

	universal::LimitRot(&fAngleInput);

	// �u���b�N�̃`�F�b�N
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	CBlock *pBlock = pBlockManager->GetHead();

	if (m_info.pBlockGrab != nullptr)
	{
		m_info.pBlockGrab->EnableCurrent(true);

		// �u���[��������
		Blur::AddParameter(0.0f, 0.01f, 15.0f, 0.0f, 0.7f);

		// �����p�x�̌v�Z
		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 posBlock = m_info.pBlockGrab->GetPosition();
		D3DXVECTOR3 vecDiff = posBlock - posPlayer;
		float vecLength = D3DXVec3Length(&(posBlock - posPlayer));

		D3DXVECTOR3 vecDiffNormal = vecDiff;
		D3DXVec3Normalize(&vecDiffNormal, &vecDiff);

		D3DXVECTOR3 move = GetMove();
		universal::VecConvertLength(&vecDiff, fabs(D3DXVec3Dot(&move, &vecDiffNormal)));

		float fAngleDiff = atan2f(vecDiff.x, vecDiff.z);

		// �h���t�g��ς��邩�̔���
		JudgeChangeDrift(fAngle, fAngleDiff, fLength);

		// ��]�̐���
		ManageRotateGrab(fAngleDiff);

		// ���C���[���O�����̔���
		JudgeRemoveWire(fLength);

		// �h���t�g�̕␳
		LimitDrift(m_info.fLengthDrift);

		// ���[�v�̐���
		ControlRoap();

		// ��ւ̈ʒu�擾
		float PosX = GetParts(3)->pParts->GetMatrix()._41;
		float PosY = GetParts(3)->pParts->GetMatrix()._42;
		float PosZ = GetParts(3)->pParts->GetMatrix()._43;

		// �G�t�F�N�g�̍Đ�
		MyEffekseer::CreateEffect(CEffekseer::TYPE_DRIFT, D3DXVECTOR3(PosX, PosY, PosZ), D3DXVECTOR3(0.0f, fAngleDiff, 0.0f));
		
		// �T�E���h�C���X�^���X�̎擾
		CSound* pSound = CSound::GetInstance();

		// �h���t�gSE�̍Đ�
		if (pSound != nullptr && !m_bDrift)
			pSound->Play(pSound->LABEL_SE_DRIFT);

		// �h���t�g���Ă���
		m_bDrift = true;
	}
	else
	{
		// �J�����̃��[���l���܂������ɖ߂�
		Camera::ControlRoll(0.0f, 0.1f);

		// �͂ރu���b�N�̒T�m
		SarchGrab();

		// �T�E���h�C���X�^���X�̎擾
		CSound* pSound = CSound::GetInstance();

		// �h���t�gSE�̒�~
		if (pSound != nullptr && m_bDrift)
			pSound->Stop(pSound->LABEL_SE_DRIFT);

		// �h���t�g���Ă��Ȃ�
		m_bDrift = false;
	}

	if (CInputKeyboard::GetInstance() != nullptr)
	{
		if (CInputKeyboard::GetInstance()->GetTrigger(DIK_F4))
		{// ������@�ύX
			m_info.bManual = m_info.bManual ? false : true;
		}
	}
}

//=====================================================
// �h���t�g��ς��邩�̔���
//=====================================================
void CPlayer::JudgeChangeDrift(float fAngle, float fAngleDiff, float fLength)
{
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	if (pJoypad == nullptr)
		return;

	if (m_info.fTimerDriftChange > 0.0f)
	{
		m_info.fTimerDriftChange -= CManager::GetDeltaTime();

		if (pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_DOWN, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_LEFT, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_UP, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_RIGHT, 0))
		{// �e�����u��
			m_info.fTimerFlip = 0.8f;

			m_info.nCntFlip++;

			float fDiffInput = fAngle - fAngleDiff;

			m_info.fAngleDrift = 0.29f;

			Blur::AddParameter(5.0f, 0.5f, 15.0f, 0.0f, 0.7f);
		}
		else
		{
			if (m_info.fTimerFlip > 0.0f)
			{// �e���^�C�}�[���Z
				m_info.fTimerFlip -= CManager::GetDeltaTime();
			}
			else
			{// �P�\���Ԃ��߂���ƒe���J�E���^�[���Z�b�g
				if (fLength <= 0.5f)
				{// �����͂�ł��Ȃ���΃��Z�b�g
					m_info.nCntFlip = 0;
				}
			}
		}
	}
}

//=====================================================
// �u���b�N��͂�ł�Ƃ��̉�]����
//=====================================================
void CPlayer::ManageRotateGrab(float fAngleDiff)
{
	float fDiff = m_info.rotDriftStart.y - fAngleDiff;

	universal::LimitRot(&fDiff);

	D3DXVECTOR3 rotDest = m_info.rotDriftStart;

	if (fDiff > 0.0f)
	{
		rotDest.y = fAngleDiff + D3DX_PI * m_info.fAngleDrift;

		// �J�������[��
		Camera::ControlRoll(0.3f, 0.04f);
	}
	else
	{
		rotDest.y = fAngleDiff - D3DX_PI * m_info.fAngleDrift;

		// �J�������[��
		Camera::ControlRoll(-0.3f, 0.04f);
	}

	universal::LimitRot(&rotDest.y);

	universal::FactingRot(&m_info.rotDriftStart.y, rotDest.y, 0.15f);
}

//=====================================================
// ���C���[���O�����̔���
//=====================================================
void CPlayer::JudgeRemoveWire(float fLength)
{
	// �u���b�N�̃G���A�����̔���
	D3DXVECTOR3 posPlayer = GetPosition();
	bool bGrab = m_info.pBlockGrab->CanGrab(posPlayer);

	if (m_info.bManual)
	{
		if (fLength <= 0.5f)
		{// �X�e�B�b�N�𗣂����烏�C���[���O��
			RemoveWire();
		}
	}
	else
	{
		if (m_info.bGrabOld && !bGrab)
		{// �u���b�N�w��̃G���A�����؂����烏�C���[���O��
			RemoveWire();
		}
	}

	// ���݂̃G���A�����̔����ۑ�
	m_info.bGrabOld = bGrab;
}

//=====================================================
// ���[�v�̐���
//=====================================================
void CPlayer::ControlRoap(void)
{
	if (m_info.pRoap != nullptr && m_info.pBlockGrab != nullptr)
	{
		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 posBlock = m_info.pBlockGrab->GetPosition();
		D3DXVECTOR3 vecDiff = posBlock - posPlayer;

		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_info.pRoap->GetVtxBuff();

		//���_���̃|�C���^
		VERTEX_3D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		float fHeight = 100.0f;

		D3DXVECTOR3 vec = { vecDiff.z, 0.0f, -vecDiff.x };

		D3DXVec3Normalize(&vec, &vec);

		pVtx[0].pos = D3DXVECTOR3(posBlock.x - vec.x * 20.0f, fHeight, posBlock.z);
		pVtx[1].pos = D3DXVECTOR3(posBlock.x + vec.x * 20.0f, fHeight, posBlock.z);
		pVtx[2].pos = D3DXVECTOR3(posPlayer.x - vec.x * 20.0f, fHeight, posPlayer.z);
		pVtx[3].pos = D3DXVECTOR3(posPlayer.x + vec.x * 20.0f, fHeight, posPlayer.z);

		//���_�o�b�t�@���A�����b�N
		pVtxBuff->Unlock();
	}
}

//=====================================================
// �͂ރu���b�N�̒T�m
//=====================================================
void CPlayer::SarchGrab(void)
{
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	if (pJoypad == nullptr)
		return;

	// ���͊p�x�̌v�Z
	D3DXVECTOR2 vecStickR =
	{
		pJoypad->GetJoyStickRX(0),
		pJoypad->GetJoyStickRY(0)
	};

	float fAngleInput = atan2f(vecStickR.x, vecStickR.y);

	// �u���b�N�}�l�[�W���[�̎擾
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	// �v�Z�p�ϐ�
	CBlockGrab *pBlockGrab = nullptr;	
	CBlockGrab* pBlockMin = nullptr;
	float fLengthMin = 0.0f;
	float fAngleMin = D3DX_PI;

	// �u���b�N���X�g�擾
	std::list<CBlockGrab*> *pListGrab = pBlockManager->GetListGrab();

	for (CBlockGrab *pBlock : *pListGrab)
	{
		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 rotPlayer = GetRotation();
		D3DXVECTOR3 posBlock = pBlock->GetPosition();
		D3DXVECTOR3 vecBlockDiff = posBlock - posPlayer;
		float fAngleDiff = atan2f(vecBlockDiff.x, vecBlockDiff.z) - rotPlayer.y;
		float fDiff = fabs(fAngleInput - fAngleDiff);

		universal::LimitRot(&fDiff);
		pBlock->EnableCurrent(false);

		if (fDiff < D3DX_PI * 0.5f && fDiff > -D3DX_PI * 0.5f)
		{
			float fLengthDiff = sqrtf(vecBlockDiff.x * vecBlockDiff.x + vecBlockDiff.z * vecBlockDiff.z);

			if (fLengthDiff <= DIST_LIMIT && (pBlockMin == nullptr || fLengthMin > fLengthDiff))
			{
				pBlockMin = pBlock;

				fAngleMin = fDiff;

				fLengthMin = fLengthDiff;
			}
		}
	}

	if (pBlockMin != nullptr)
	{
		pBlockGrab = pBlockMin;

		m_info.fLengthDrift = fLengthMin;

		m_info.fAngleDrift = 0.4f;
	}

	if (pBlockGrab != nullptr)
	{
		if (pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_DOWN, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_LEFT, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_UP, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_RIGHT, 0))
		{// �e�����u��
			m_info.pBlockGrab = pBlockGrab;

			m_info.fTimerDriftChange = 0.7f;

			m_info.rotDriftStart = GetRotation();
			m_info.rotDriftStart.x += D3DX_PI * 0.5f;
		}
	}
}

//=====================================================
// ���C���[���O��
//=====================================================
void CPlayer::RemoveWire(void)
{
	m_info.nCntFlip = 0;
	m_info.fCntAngle = 0.0f;

	m_info.pBlockGrab = nullptr;

	m_info.fLengthDrift = 0.0f;

	m_info.rotDriftStart.x -= D3DX_PI * 0.5f;
	universal::LimitRot(&m_info.rotDriftStart.x);

	// �u���[��߂�
	Blur::ResetBlur();
}

//=====================================================
// ���̑���
//=====================================================
void CPlayer::InputKatana(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	if (pInputManager->GetTrigger(CInputManager::BUTTON_KATANA))
	{
		// �����[�V�����t���O�Ǘ�
		ManageKanataAtttack();
	}
}

//=====================================================
// ���̍U���̊Ǘ�
//=====================================================
void CPlayer::ManageKanataAtttack(void)
{
	if (m_fragNinja.bSlashDown)
	{
		m_fragNinja.bSlashUp = true;
	}
	else
	{
		m_fragNinja.bSlashDown = true;
	}
}

//=====================================================
// �h���t�g�̕␳
//=====================================================
void CPlayer::LimitDrift(float fLength)
{
	if (m_info.pBlockGrab == nullptr)
		return;

	D3DXVECTOR3 posPlayer = GetPosition();
	D3DXVECTOR3 posBlock = m_info.pBlockGrab->GetPosition();
	D3DXVECTOR3 vecDiff = posPlayer - posBlock;

	vecDiff.y = 0.0f;

	float fLengthDiff = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

	//if (fLengthDiff < DIST_LIMIT - LINE_CORRECT_DRIFT)
	{
		//m_info.fAngleHandle = 0.2f;

		/*universal::VecConvertLength(&vecDiff, fLength);

		D3DXVECTOR3 posDest = posBlock + vecDiff;

		universal::MoveToDest(&posPlayer, posDest, 0.1f);

		SetPosition(posPlayer);*/
	}
	/*else if(fLengthDiff >= fLength + LINE_CORRECT_DRIFT)
	{
		m_info.fAngleHandle = 0.2f;
	}
	else
	{
		m_info.fAngleHandle = 0.0f;
	}*/
}

//=====================================================
// �����蔻�菈��
//=====================================================
void CPlayer::Collision(void)
{
	// �O��̈ʒu��ۑ�
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posOld = GetPositionOld();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 posParts[2];
	D3DXVECTOR3 posOldParts[2];
	D3DXVECTOR3 posDef, posDefOld;
	bool bRoad[2];
	CInputManager* pInputManager = CInputManager::GetInstance();

	// �v�Z�p�}�g���b�N�X��錾
	D3DXMATRIX* mtx = &GetMatrix();
	D3DXMATRIX mtxTrans, mtxRot;

	// �v���C���[���̓����蔻��T�C�Y���画��pOBB��ݒ�
	D3DXVECTOR3 paramSize = m_param.sizeCollider;
	D3DXVECTOR3 sizeX = universal::PosRelativeMtx(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, rot.y, 0.0f), paramSize);
	D3DXVECTOR3 sizeZ = universal::PosRelativeMtx(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -rot.y, 0.0f), D3DXVECTOR3(paramSize.x, 0.0f, paramSize.z));

	// �^�C���̈ʒu�ۑ�
	posParts[0] = universal::GetMtxPos(GetParts(2)->pParts->GetMatrix()) + (pos - posOld);
	posParts[0].y -= HEIGH_FRONT_WHEEL;
	posParts[1] = universal::GetMtxPos(GetParts(3)->pParts->GetMatrix()) + (pos - posOld);
	posParts[1].y -= HEIGH_REAR_WHEEL;

	// �^�C���̉ߋ��ʒu�ۑ�
	posOldParts[0] = universal::GetMtxPos(*GetParts(2)->pParts->GetMatrixOld()) + (pos - posOld);
	posOldParts[0].y -= HEIGH_FRONT_WHEEL;
	posOldParts[1] = universal::GetMtxPos(*GetParts(3)->pParts->GetMatrixOld()) + (pos - posOld);
	posOldParts[1].y -= HEIGH_REAR_WHEEL;

	// �^�C���̒��_���v�Z
	posDef = (posParts[0] + posParts[1]) * 0.5f;

	// ���b�V�����[�h�̃��X�g�擾
	std::list<CMeshRoad*> listRoad = CMeshRoad::GetArray();
	for (auto it : listRoad)
	{
		// �K�[�h���[���Ƃ̓����蔻��
		// �K�[�h���[����vector���擾
		std::vector<CGuardRail*>* aGuardRail = it->GetArrayGR();
		for (auto itGuardRail : *aGuardRail)
		{
			// �K�[�h���[���Ɠ������Ă��邩���肷��
			if (!itGuardRail->CollideGuardRail(&pos, &move, sizeX, &m_info.fSpeed))
				continue;

			// �ꂩ�����肵���甲����
			rot.y = atan2f(move.x, move.z);

			// �h���t�g���̎��h���t�g��Ԃ���������
			m_info.rotDriftDest = 0.0f;
			RemoveWire();

			break;
		}

		// �K�[�h���[���̔��蕪�^�C���̈ʒu���v�Z����
		posParts[0] += pos - posOld;
		posParts[1] += pos - posOld;
		posOldParts[0] += pos - posOld;
		posOldParts[1] += pos - posOld;

		// �^�C�����ꂼ���meshRoad�Ɠ����蔻����Ƃ�
		bRoad[0] = it->CollideRoad(&posParts[0], posOldParts[0]);
		bRoad[1] = it->CollideRoad(&posParts[1], posOldParts[1]);

		// �ǂ��炩�̃^�C����meshRoad�Ɠ������Ă����烋�[�v�𔲂���
		if (bRoad[0] || bRoad[1])
			break;
	}

	// �^�C�����ꂼ���block�Ɠ����蔻����Ƃ�
	// �擪�I�u�W�F�N�g����
	CBlock* pBlock = CBlockManager::GetInstance()->GetHead();
	while (pBlock)
	{
		// ���̃A�h���X��ۑ�
		CBlock* pBlockNext = pBlock->GetNext();

		if (D3DXVec3Length(&(pBlock->GetPosition() - pos)) < 10000.0f)
		{
			// �^�C����block�œ����蔻����Ƃ�
			if (pBlock->Collide(&posParts[0], posOldParts[0]))
				bRoad[0] = true;

			if (pBlock->Collide(&posParts[1], posOldParts[1]))
				bRoad[1] = true;
		}

		// �����������ʂ��Ă����甲����
		if (bRoad[0] && bRoad[1])
			break;

		// ���̃A�h���X����
		pBlock = pBlockNext;
	}

	// �v���C���[�̍����𒲐�
	pos.y += ((posParts[0] + posParts[1]) * 0.5f).y - posDef.y;

	// �^�C���̈ʒu�֌W����p�x���v�Z
	if ((posParts[1].y - posParts[0].y) < D3DXVec3Length(&(posParts[0] - posParts[1])) && (bRoad[0] || bRoad[1]))
		rot.x = asinf((posParts[1].y - posParts[0].y) / D3DXVec3Length(&(posParts[0] - posParts[1])));

	// �^�C���̏�Ԃ𔻒肷��
	if (bRoad[0] && bRoad[1])
	{// �^�C�����������ɐG��Ă���Ƃ�
		move.y = GRAVITY_GROUND;

		m_info.bAir = false;
	}
	else if (bRoad[0] || bRoad[1])
	{// �^�C�����Е��������ɐG��Ă���Ƃ�
		move.y = GRAVITY_GROUND;

		m_info.bAir = true;
	}
	else
	{// �^�C�����ǂ�������ɐG��Ă��Ȃ��Ƃ�

		if (pInputManager != nullptr)
		{
			// �n���h���̑���
			CInputManager::SAxis axis = pInputManager->GetAxis();

			// �󒆂ł̑O��̉�]����
			if (axis.axisMove.z > 0.0f)
				rot.x += ROT_AIRBIKE_MAG;
			else if (axis.axisMove.z < 0.0f)
				rot.x -= ROT_AIRBIKE_MAG;
			else
			{
				rot.x += ROT_AIRBIKE_MAG;
			}
		}

		m_info.bAir = true;
	}

	// �p�x����
	if (rot.x > ROT_BIKE_FRONT_LIMIT)
		rot.x = ROT_BIKE_FRONT_LIMIT;

	if (rot.x < ROT_BIKE_REAR_LIMIT)
		rot.x = ROT_BIKE_REAR_LIMIT;

#ifdef _DEBUG
	if (CInputJoypad::GetInstance() != nullptr)
	{
		if (CInputJoypad::GetInstance()->GetPress(CInputJoypad::PADBUTTONS_A, 0) || 
			CInputKeyboard::GetInstance()->GetPress(DIK_SPACE))
		{// ������@�ύX
			pos.y += 30.0f;
			move.y = 0.0f;
			rot.x = 0.0f;
		}
	}
#endif // _DEBUG

	// �ʒu�ƈړ��ʂƊp�x����
	SetPosition(pos);
	SetMove(move);
	SetRotation(rot);
}

//=====================================================
// �X�s�[�h�̊Ǘ�
//=====================================================
void CPlayer::ManageSpeed(void)
{
	CSlow* pSlow = CSlow::GetInstance();

	// �X�s�[�h�̑���
	if (m_info.fSpeedDest >= m_info.fSpeed)
	{// �������Ă���Ƃ�
		m_info.fSpeed += (m_info.fSpeedDest - m_info.fSpeed) * m_param.fFactAccele;

#if 0
		CDebugProc::GetInstance()->Print("\n������");
#endif

		//// �T�E���h�C���X�^���X�̎擾
		//CSound* pSound = CSound::GetInstance();

		//// �A�N�Z����SE�Đ�
		//if (pSound != nullptr)
		//	pSound->Play(pSound->LABEL_SE_ACCELERATOR);
	}
	else
	{// �������Ă���Ƃ�
		m_info.fSpeed += (m_info.fSpeedDest - m_info.fSpeed) * m_param.fFactAttenu;

#if 0
		CDebugProc::GetInstance()->Print("\n������");
#endif
	}

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rot = GetRotation();
	float rotBlock;

	// �X���[��Ԃ��l�������ړ��ʂ̒���
	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		pos += move * fScale;
		SetPosition(pos);
	}
	else
	{
		pos += move;
		SetPosition(pos);
	}

	m_info.orbitColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);

	if (m_info.pBlockGrab != nullptr)
	{
		// �����p�x�̌v�Z
		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 posBlock = m_info.pBlockGrab->GetPosition();
		rotBlock = atan2f((posBlock.x - posPlayer.x), (posBlock.z - posPlayer.z));
		universal::LimitRot(&rotBlock);

		if (D3DXVec3Cross(&posPlayer, &GetMove(), &(posBlock - posPlayer))->y > 0.0f)
		{
			rot.z += (-ROT_Z_DRIFT - rot.z) * HANDLE_INERTIA_DRIFT;
			m_info.rotDriftDest = D3DX_PI * 0.5f;
		}
		else
		{
			rot.z += (ROT_Z_DRIFT - rot.z) * HANDLE_INERTIA_DRIFT;
			m_info.rotDriftDest = D3DX_PI * -0.5f;
		}
		
		universal::LimitRot(&rot.z);

		float rotDef = rotBlock - rot.y;
		universal::LimitRot(&rotDef);
		rot.y += rotDef;
		universal::LimitRot(&rot.y);

		m_info.orbitColor = D3DXCOLOR(1.0f, 0.15f, 0.0f, 1.0f);
	}
	else if (m_info.fSpeed >= NOTROTATE)
	{// �n���h���̉�]��ǉ�
		if (m_info.fAngleHandle == 0.0f)
			rot.z += (m_info.fAngleHandle * m_param.fAngleMaxCurve - rot.z) * HANDLE_INERTIA_RESET;
		else
			rot.z += (m_info.fAngleHandle * m_param.fAngleMaxCurve - rot.z) * HANDLE_INERTIA;
		
		universal::LimitRot(&rot.z);

		if (fabsf(m_info.rotDriftDest) < ROT_CURVE_LIMIT)
		{
			rot.y += rot.z * HANDLE_CURVE_MAG;
			universal::LimitRot(&rot.y);
		}
	}
	else
	{
		rot.z += (0.0f - rot.z) * HANDLE_INERTIA_RESET;
		universal::LimitRot(&rot.z);
	}

	if (m_info.pBlockGrab == nullptr)
	{
		rot.y += (0.0f - m_info.rotDriftDest) * HANDLE_INERTIA_DEFAULT;
		m_info.rotDriftDest += (0.0f - m_info.rotDriftDest) * HANDLE_INERTIA_DEFAULT;
	}

	SetRotation(rot);

	// �ɍ��W����������Ă��镪��␳
	rot.x += D3DX_PI * 0.5f;

	if (m_info.pBlockGrab == nullptr)
	{
		m_info.rotDriftStart = rot;
	}

	CDebugProc::GetInstance()->Print("\n�����p�x %f", m_info.rotDriftDest);

	D3DXVECTOR3 vecForward = universal::PolarCoordinates(m_info.rotDriftStart);

	// ���݂̃X�s�[�h�ƑO���x�N�g���������Ĉړ��ʂɓK�p
	move.x = vecForward.x * m_info.fSpeed;
	if(!m_info.bAir)
		move.y = vecForward.y * m_info.fSpeed;
	move.z = vecForward.z * m_info.fSpeed;

	// �ړ��ʂɏd�͂�K�p
	move.y += GRAVITY;

	SetMove(move);

	if (m_info.pOrbitLamp != nullptr)
		m_info.pOrbitLamp->SetOffset(GetMatrix(), m_info.orbitColor, m_info.pOrbitLamp->GetID());
}

//=====================================================
// �X�s�[�h�ɂ��u���[�̊Ǘ�
//=====================================================
void CPlayer::ManageSpeedBlur(void)
{
	float fRate = m_info.fSpeed / m_param.fSpeedMaxInitial;

	Blur::SetBlur(SIZE_SPEEDBLUR * fRate, fRate * DENSITY_SPEEDBLUR);
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CPlayer::ManageState(void)
{
	switch (m_info.state)
	{
	case STATE_NORMAL:
	{// �ʏ���

	}
	default:
		break;
	}
}

//=====================================================
// ���[�V�����Ǘ�
//=====================================================
void CPlayer::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (m_fragMotion.bMove)
	{// �������[�V����
		if (nMotion != MOTION_WALK_FRONT)
		{
			SetMotion(MOTION_WALK_FRONT);
		}
	}
	else
	{// �ҋ@���[�V����
		if (nMotion != MOTION_NEUTRAL)
		{
			SetMotion(MOTION_NEUTRAL);
		}
	}
}

//=====================================================
// �E�҂̃��[�V�����Ǘ�
//=====================================================
void CPlayer::ManageMotionNinja(void)
{
	if (m_pPlayerNinja == nullptr)
		return;
	
	int nMotion = m_pPlayerNinja->GetMotion();
	bool bFinish = m_pPlayerNinja->IsFinish();

	if (m_fragNinja.bSlashUp)
	{
		if (nMotion == MOTION_NINJA::MOTION_NINJA_SLASHDOWN)
		{// �؂艺�낵���I����Ă��烂�[�V�����Đ�
			if(bFinish)
				m_pPlayerNinja->SetMotion(MOTION_NINJA::MOTION_NINJA_SLASHUP);
		}
		else
		{// �؂艺�낵����̔h���łȂ��ꍇ
			if (nMotion != MOTION_NINJA::MOTION_NINJA_SLASHUP)
				m_pPlayerNinja->SetMotion(MOTION_NINJA::MOTION_NINJA_SLASHUP);

			if (bFinish)
				m_fragNinja.bSlashUp = false;
		}
	}
	else if (m_fragNinja.bSlashDown)
	{
		if (nMotion != MOTION_NINJA::MOTION_NINJA_SLASHDOWN)
		{
			// �G�t�F�N�V�A�擾
			CEffekseer* pEffekseer = CManager::GetMyEffekseer();

			float PosX = m_pPlayerNinja->GetParts(1)->pParts->GetMatrix()._41;
			float PosY = m_pPlayerNinja->GetParts(1)->pParts->GetMatrix()._42;
			float PosZ = m_pPlayerNinja->GetParts(1)->pParts->GetMatrix()._43;

			// �G�t�F�N�g�̍Đ�
			CEffekseerEffect *pEffect = MyEffekseer::CreateEffect(CEffekseer::TYPE_SLASH, D3DXVECTOR3(PosX, PosY, PosZ));

			m_listSlashEffect.push_back(pEffect);

			m_pPlayerNinja->SetMotion(MOTION_NINJA::MOTION_NINJA_SLASHDOWN);
		}

		if (bFinish)
			m_fragNinja.bSlashDown = false;
	}
	else
	{
		if (nMotion != MOTION_NINJA::MOTION_NINJA_NEUTRAL)
		{
			m_pPlayerNinja->SetMotion(MOTION_NINJA::MOTION_NINJA_NEUTRAL);
		}
	}
}

//=====================================================
// �a���G�t�F�N�g�̊Ǘ�
//=====================================================
void CPlayer::ManageSlashEffect(void)
{
	if (m_pPlayerNinja == nullptr)
		return;

	D3DXVECTOR3 posEfect = m_pPlayerNinja->GetMtxPos(0);

	for (auto it : m_listSlashEffect)
	{
		if(it != nullptr)
			it = it->FollowPosition(posEfect);
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
	pDebugProc->Print("\n�v���C���[�̌���[%f,%f,%f]", GetRotation().x, GetRotation().y, GetRotation().z);/*
	pDebugProc->Print("\n�ڕW���x[%f]", m_info.fSpeedDest);
	pDebugProc->Print("\n���݂̑��x[%f]", m_info.fSpeed);
	pDebugProc->Print("\n�e���J�E���^�[[%d]", m_info.nCntFlip);
	pDebugProc->Print("\n�p�x�J�E���^�[[%f]", m_info.fCntAngle);
	pDebugProc->Print("\n�u���[�̃T�C�Y[%f]", m_info.fSizeBlurDrift);
	pDebugProc->Print("\n�u���[�̔Z��[%f]", m_info.fDesityBlurDrift);*/
#endif

	// �u���[�̃T�C�Y����
	if (CInputKeyboard::GetInstance()->GetPress(DIK_UP))
	{
		m_info.fSizeBlurDrift += 0.3f;
	}
	else if (CInputKeyboard::GetInstance()->GetPress(DIK_DOWN))
	{
		m_info.fSizeBlurDrift -= 0.3f;
	}

	// �u���[�̔Z������
	if (CInputKeyboard::GetInstance()->GetPress(DIK_LEFT))
	{
		m_info.fDesityBlurDrift += 0.03f;
	}
	else if (CInputKeyboard::GetInstance()->GetPress(DIK_RIGHT))
	{
		m_info.fDesityBlurDrift -= 0.03f;
	}
}
//*****************************************************
//
// �W���C�p�b�h���͏���[inputjoypad.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "inputjoypad.h"
#include "debugproc.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float LINE_TRIGGER = 100;	// �g���K�[�{�^���̃g���K�[����̂������l
const BYTE BYTE_MAX = 255;	// �o�C�g�^�̍ő�l
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CInputJoypad *CInputJoypad::m_pJoyPad = nullptr;

//====================================================
// �R���X�g���N�^
//====================================================
CInputJoypad::CInputJoypad()
{
	ZeroMemory(&m_aState, sizeof(m_aState));
	ZeroMemory(&m_aStateRelease, sizeof(m_aStateRelease));
	ZeroMemory(&m_aStateTrigger, sizeof(m_aStateTrigger));
	ZeroMemory(&m_aStateRepeat, sizeof(m_aStateRepeat));
	ZeroMemory(&m_aVibration, sizeof(m_aVibration));
	ZeroMemory(&m_aVibState,sizeof(m_aVibState));
	ZeroMemory(&m_aVibTimer, sizeof(m_aVibTimer));
	ZeroMemory(&m_abTriggerTB, sizeof(m_abTriggerTB));
	ZeroMemory(&m_abPressTB, sizeof(m_abPressTB));
	ZeroMemory(&m_abTrigggerLStick, sizeof(m_abTrigggerLStick));
	ZeroMemory(&m_abTrigggerRStick, sizeof(m_abTrigggerRStick));
	ZeroMemory(&m_abyTriggerL, sizeof(m_abyTriggerL));
	ZeroMemory(&m_abyTriggerR, sizeof(m_abyTriggerR));
}

//====================================================
// �f�X�g���N�^
//====================================================
CInputJoypad::~CInputJoypad()
{

}

//====================================================
// ��������
//====================================================
CInputJoypad *CInputJoypad::Create(void)
{
	if (m_pJoyPad == nullptr)
	{
		m_pJoyPad = new CInputJoypad;

		if (m_pJoyPad != nullptr)
		{
			m_pJoyPad->Init();
		}
	}

	return m_pJoyPad;
}

//====================================================
// ����������
//====================================================
HRESULT CInputJoypad::Init(void)
{
	//xinput��L��������
	XInputEnable(true);

	//�������̃N���A
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		memset(&m_aState[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_aVibration[nCount], 0, sizeof(XINPUT_VIBRATION));
		memset(&m_aStateTrigger[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_aVibState[nCount], 0, sizeof(PADVIB));
	}

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CInputJoypad::Uninit(void)
{
	m_pJoyPad = nullptr;

	// xinput�𖳌�������
	XInputEnable(false);
}

//====================================================
// �X�V����
//====================================================
void CInputJoypad::Update(void)
{
	XINPUT_STATE aState[MAX_PLAYER];

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntKey = 0; nCntKey < CInputJoypad::PADBUTTONS_MAX; nCntKey++)
		{
			if ((m_aState[nCntPlayer].Gamepad.wButtons & (0x01 << nCntKey)))
			{// ���s�[�g�̃J�E���^�[
				m_aCntRepeat[nCntPlayer][nCntKey]++;
			}
			else
			{
				m_aCntRepeat[nCntPlayer][nCntKey] = 0;
			}
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (m_aVibTimer[nCntPlayer] > 0)
		{
			m_aVibTimer[nCntPlayer]--;
		}
		else if (m_aVibTimer[nCntPlayer] <= 0)
		{
			memset(&m_aVibState[nCntPlayer], 0, sizeof(PADVIB));
			m_aVibration[nCntPlayer].wLeftMotorSpeed = 0;
			m_aVibration[nCntPlayer].wRightMotorSpeed = 0;
			// �U����Ԃ�`�B
			XInputSetState(nCntPlayer, &m_aVibration[nCntPlayer]);
			m_aVibTimer[nCntPlayer] = 0;
		}

		if (m_aVibration[nCntPlayer].wLeftMotorSpeed < 0 ||
			m_aVibration[nCntPlayer].wRightMotorSpeed < 0)
		{
			m_aVibration[nCntPlayer].wLeftMotorSpeed = 0;
			m_aVibration[nCntPlayer].wRightMotorSpeed = 0;
			// �U����Ԃ�`�B
			XInputSetState(nCntPlayer, &m_aVibration[nCntPlayer]);
		}

		// ���̓f�o�C�X����f�[�^���擾
		if (XInputGetState(nCntPlayer, &aState[nCntPlayer]) == ERROR_SUCCESS)
		{
			// �X�e�B�b�N�̃g���K�[����
			CheckStickTrigger(aState[nCntPlayer], nCntPlayer);

			// �g���K�[�{�^���̔���
			CheckTrigger(aState[nCntPlayer], nCntPlayer);

			// �g���K�[
			m_aStateTrigger[nCntPlayer].Gamepad.wButtons =
				(m_aState[nCntPlayer].Gamepad.wButtons ^ aState[nCntPlayer].Gamepad.wButtons)
				& aState[nCntPlayer].Gamepad.wButtons;

			// �����[�X
			m_aStateRelease[nCntPlayer].Gamepad.wButtons =
				(m_aState[nCntPlayer].Gamepad.wButtons ^ aState[nCntPlayer].Gamepad.wButtons)
				& m_aState[nCntPlayer].Gamepad.wButtons;			

			// �v���X
			m_aState[nCntPlayer] = aState[nCntPlayer];

			m_abyTriggerL[nCntPlayer] = m_aState[nCntPlayer].Gamepad.bLeftTrigger;
			m_abyTriggerR[nCntPlayer] = m_aState[nCntPlayer].Gamepad.bRightTrigger;

			CDebugProc::GetInstance()->Print("\n�g���K�[�����[%f]", m_abyTriggerL[nCntPlayer]);
		}
	}
}

//====================================================
// �f�o�C�X�̍X�V����
//====================================================
void CInputJoypad::UpdateDevice(void)
{

}

//====================================================
// �g���K�[����擾
//====================================================
bool CInputJoypad::GetPressTB(TRIGGER trigger, int nPlayer)
{
	return m_abPressTB[nPlayer][trigger];
}

//====================================================
// �g���K�[����擾
//====================================================
bool CInputJoypad::GetTriggerTB(TRIGGER trigger, int nPlayer)
{
	return m_abTriggerTB[nPlayer][trigger];
}

//====================================================
// ���g���K�[�̓���
//====================================================
float CInputJoypad::GetTriggerL(int nPlayer)
{
	float fTrigger = 0.0f;

	fTrigger = (float)m_abyTriggerL[nPlayer] / BYTE_MAX;

	return fTrigger;
}

//====================================================
// �E�g���K�[�̓���
//====================================================
float CInputJoypad::GetTriggerR(int nPlayer)
{
	float fTrigger = 0.0f;

	fTrigger = (float)m_abyTriggerR[nPlayer] / BYTE_MAX;

	return fTrigger;
}

//====================================================
// �g���K�[�̓��͔���
//====================================================
void CInputJoypad::CheckTrigger(XINPUT_STATE state, int nPlayer)
{
	int nDiffRT, nDiffLT;

	nDiffRT = state.Gamepad.bRightTrigger - m_aState[nPlayer].Gamepad.bRightTrigger;
	nDiffLT = state.Gamepad.bLeftTrigger - m_aState[nPlayer].Gamepad.bLeftTrigger;

	// ��C�ɉ�����Ă�����g���K�[����
	m_abTriggerTB[nPlayer][TRIGGER_RT] = nDiffRT > LINE_TRIGGER;
	m_abTriggerTB[nPlayer][TRIGGER_LT] = nDiffLT > LINE_TRIGGER;

	// �v���X����
	m_abPressTB[nPlayer][TRIGGER_RT] = m_aState[nPlayer].Gamepad.bRightTrigger > LINE_TRIGGER;
	m_abPressTB[nPlayer][TRIGGER_LT] = m_aState[nPlayer].Gamepad.bLeftTrigger > LINE_TRIGGER;

	CDebugProc::GetInstance()->Print("\n������[%d]�E[%d]", nDiffLT, nDiffRT);
}

//====================================================
// �W���C�X�e�B�b�N�g���K�[���菈��
//====================================================
void CInputJoypad::CheckStickTrigger(XINPUT_STATE state, int nPlayer)
{
	// ���X�e�B�b�N==========================================
	// ���E
	float fDiff = (float)state.Gamepad.sThumbLX / USHRT_MAX * 2 - (float)m_aState[nPlayer].Gamepad.sThumbLX / USHRT_MAX * 2;
	
	m_abTrigggerLStick[nPlayer][DIRECTION_RIGHT] = fDiff > 0.5f && (float)m_aState[nPlayer].Gamepad.sThumbLX >= 0.0f;

	m_abTrigggerLStick[nPlayer][DIRECTION_LEFT] = fDiff < -0.5f && (float)m_aState[nPlayer].Gamepad.sThumbLX <= 0.0f;

	// �㉺
	fDiff = (float)state.Gamepad.sThumbLY / USHRT_MAX * 2 - (float)m_aState[nPlayer].Gamepad.sThumbLY / USHRT_MAX * 2;

	m_abTrigggerLStick[nPlayer][DIRECTION_UP] = fDiff > 0.5f && (float)m_aState[nPlayer].Gamepad.sThumbLY >= 0.0f;

	m_abTrigggerLStick[nPlayer][DIRECTION_DOWN] = fDiff < -0.5f && (float)m_aState[nPlayer].Gamepad.sThumbLY <= 0.0f;

	// ���X�e�B�b�N==========================================
	// ���E
	fDiff = (float)state.Gamepad.sThumbRX / USHRT_MAX * 2 - (float)m_aState[nPlayer].Gamepad.sThumbRX / USHRT_MAX * 2;

	m_abTrigggerRStick[nPlayer][DIRECTION_RIGHT] = fDiff > 0.5f && (float)m_aState[nPlayer].Gamepad.sThumbRX >= 0.0f;

	m_abTrigggerRStick[nPlayer][DIRECTION_LEFT] = fDiff < -0.5f && (float)m_aState[nPlayer].Gamepad.sThumbRX <= 0.0f;

	// �㉺
	fDiff = (float)state.Gamepad.sThumbRY / USHRT_MAX * 2 - (float)m_aState[nPlayer].Gamepad.sThumbRY / USHRT_MAX * 2;

	m_abTrigggerRStick[nPlayer][DIRECTION_UP] = fDiff > 0.5f && (float)m_aState[nPlayer].Gamepad.sThumbRY >= 0.0f;

	m_abTrigggerRStick[nPlayer][DIRECTION_DOWN] = fDiff < -0.5f && (float)m_aState[nPlayer].Gamepad.sThumbRY <= 0.0f;
}

//====================================================
// �W���C�X�e�B�b�N�g���K�[���
//====================================================
bool CInputJoypad::GetLStickTrigger(DIRECTION direction, int nPlayer)
{
	return m_abTrigggerLStick[nPlayer][direction];
}

//====================================================
// �W���C�X�e�B�b�N�g���K�[���
//====================================================
bool CInputJoypad::GetRStickTrigger(DIRECTION direction, int nPlayer)
{
	return m_abTrigggerRStick[nPlayer][direction];
}

//====================================================
// �W���C�X�e�B�b�N���擾
//====================================================
float CInputJoypad::GetJoyStickLX(int nPlayer)
{
	//����Ԃ�
	return (float)m_aState[nPlayer].Gamepad.sThumbLX / USHRT_MAX * 2;
}

//====================================================
// �W���C�X�e�B�b�N���擾
//====================================================
float CInputJoypad::GetJoyStickLY(int nPlayer)
{
	//����Ԃ�
	return (float)m_aState[nPlayer].Gamepad.sThumbLY / USHRT_MAX * 2;
}

//====================================================
// �W���C�X�e�B�b�N���擾
//====================================================
float CInputJoypad::GetJoyStickRX(int nPlayer)
{
	//����Ԃ�
	return (float)m_aState[nPlayer].Gamepad.sThumbRX / USHRT_MAX * 2;
}

//====================================================
// �W���C�X�e�B�b�N���擾
//====================================================
float CInputJoypad::GetJoyStickRY(int nPlayer)
{
	//����Ԃ�
	return (float)m_aState[nPlayer].Gamepad.sThumbRY / USHRT_MAX * 2;
}

//====================================================
// �W���C�X�e�B�b�N�ړ��ʎ擾�iL�j
//====================================================
D3DXVECTOR3 CInputJoypad::GetVecStickL(void)
{
	float fAngle = atan2f(GetJoyStickLX(0), GetJoyStickLY(0));
	float fLength = sqrtf(GetJoyStickLX(0) * GetJoyStickLX(0) + GetJoyStickLY(0) * GetJoyStickLY(0));

	D3DXVECTOR3 vec =
	{
		sinf(fAngle) * fLength,
		cosf(fAngle) * fLength,
		0.0f
	};

	return vec;
}

//====================================================
// �v���X���擾
//====================================================
bool CInputJoypad::GetPress(PADBUTTOS nKey, int nPlayer)
{
	//�O�����Z�q
	return(m_aState[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================
//�g���K�[���擾
//====================================================
bool CInputJoypad::GetTrigger(PADBUTTOS nKey, int nPlayer)
{
	//�O�����Z�q
	return(m_aStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================
//�����[�X���擾
//====================================================
bool CInputJoypad::GetRelease(PADBUTTOS nKey, int nPlayer)
{
	//�O�����Z�q
	return(m_aStateRelease[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================
//���s�[�g���擾
//====================================================
int CInputJoypad::GetRepeat(PADBUTTOS nKey, int nPlayer)
{
	//�O�����Z�q
	return m_aCntRepeat[nPlayer][nKey];
}

//====================================================
// �o�C�u���ݒ�
//====================================================
void CInputJoypad::Vibration(int nPlayer, PADVIB state, float fVib,int nTime)
{
	switch (state)
	{
	case PADVIB_USE:
		m_aVibration[nPlayer].wLeftMotorSpeed = (WORD)(USHRT_MAX * fVib);
		m_aVibration[nPlayer].wRightMotorSpeed = (WORD)(USHRT_MAX * fVib);
		m_aVibState[nPlayer] = state;
		break;
	default:
		break;
	}

	m_aVibTimer[nPlayer] = nTime;

	//�U����Ԃ�`�B
	XInputSetState(nPlayer, &m_aVibration[nPlayer]);
}
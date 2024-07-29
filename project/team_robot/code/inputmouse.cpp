//*****************************************************
//
// �}�E�X���͏���[inputmouse.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "inputmouse.h"
#include "manager.h"
#include "debugproc.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CInputMouse *CInputMouse::m_pMouse = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CInputMouse::CInputMouse()
{
	// �l�̃N���A
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));
	ZeroMemory(&m_aKeyState[0], sizeof(m_aKeyState));
	ZeroMemory(&m_aKeyStateTrigger[0], sizeof(m_aKeyStateTrigger));
	ZeroMemory(&m_aKeyStateRelease[0], sizeof(m_aKeyStateRelease));
	ZeroMemory(&m_aKeyStateRepeat[0], sizeof(m_aKeyStateRepeat));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CInputMouse::~CInputMouse()
{

}

//=====================================================
// ��������
//=====================================================
CInputMouse *CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pMouse == nullptr)
	{
		m_pMouse = new CInputMouse;

		if (m_pMouse != nullptr)
		{
			m_pMouse->Init(hInstance, hWnd);
		}
	}

	return m_pMouse;
}

//=====================================================
// ����������
//=====================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ��{�N���X�̏�����
	CInput::Init(hInstance, hWnd);

	//���̓f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g�̐���
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_REL;						//���Έʒu���[�h�iREL�j�A�iABS�j��Έʒu

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	//�}�E�X�̃A�N�Z�X���l��
	m_pDevice->Acquire();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CInputMouse::Uninit(void)
{
	m_pMouse = nullptr;

	// ��{�N���X�̏I������
	CInput::Uninit();
}

//===================================================================
//�}�E�X�X�V����
//===================================================================
void CInputMouse::Update(void)
{
	//�ϐ��錾
	DIMOUSESTATE2 aMouseState;						//�}�E�X���͏��
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &aMouseState)))
	{
		for (nCntKey = 0; nCntKey < NUM_BUTTON_MAX; nCntKey++)
		{
			if (m_mouseState.rgbButtons[nCntKey] == 0x80)
			{// ���s�[�g�̃J�E���^�[
				m_aCntRepeat[nCntKey]++;
			}
			else if (m_mouseState.rgbButtons[nCntKey] == 0 && aMouseState.rgbButtons[nCntKey] == 0)
			{
				m_aCntRepeat[nCntKey] = 0;
			}

			//�g���K�[���
			m_aKeyStateTrigger[nCntKey] = (m_mouseState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey])
				& aMouseState.rgbButtons[nCntKey];

			// �����[�X���
			m_aKeyStateRelease[nCntKey] = (m_mouseState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey]) 
				& m_mouseState.rgbButtons[nCntKey];
		}

		m_mouseState = aMouseState;
	}
	else
	{
		m_pDevice->Acquire();
	}

	//CDebugProc::GetInstance()->Print("\n�}�E�X�ړ�[%d,%d]", m_mouseState.lX, m_mouseState.lY);
}

//====================================================
// �}�E�X�f�o�C�X�̍X�V����
//====================================================
void CInputMouse::UpdateDevice(void)
{
	m_pDevice->Acquire();
}

//=====================================================
// �}�E�X�ړ��ʎ擾
//=====================================================
float CInputMouse::GetMoveIX(void)
{
	return (float)m_mouseState.lX;
}

float CInputMouse::GetMoveIY(void)
{
	return (float)m_mouseState.lY;
}

//=====================================================
// �z�C�[���擾
//=====================================================
LONG CInputMouse::GetMoveIZ(void)
{
	return m_mouseState.lZ;
}

//=====================================================
//�v���X���擾
//=====================================================
bool CInputMouse::GetPress(int nKey)
{
	//�O�����Z�q
	return(m_mouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//=====================================================
//�g���K�[���擾
//=====================================================
bool CInputMouse::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=====================================================
//�����[�X���擾
//=====================================================
bool CInputMouse::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=====================================================
//���s�[�g���擾
//=====================================================
int CInputMouse::GetRepeat(int nKey)
{
	return m_aCntRepeat[nKey];
}
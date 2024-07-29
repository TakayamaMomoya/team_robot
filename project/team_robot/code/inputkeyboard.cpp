//*****************************************************
//
// �L�[�{�[�h���͏���[inputkeyboard.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "inputkeyboard.h"
#include "debugproc.h"
#include "manager.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CInputKeyboard *CInputKeyboard::m_pKeyboard = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CInputKeyboard::CInputKeyboard()
{
	ZeroMemory(&m_aKeyState[0], sizeof(m_aKeyState));
	ZeroMemory(&m_aKeyStateTrigger[0], sizeof(m_aKeyStateTrigger));
	ZeroMemory(&m_aKeyStateRelease[0], sizeof(m_aKeyStateRelease));
	ZeroMemory(&m_aKeyStateRepeat[0], sizeof(m_aKeyStateRepeat));
	ZeroMemory(&m_aCntRepeat[0],sizeof(m_aCntRepeat));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=====================================================
// ��������
//=====================================================
CInputKeyboard *CInputKeyboard::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pKeyboard == nullptr)
	{
		m_pKeyboard = new CInputKeyboard;

		if (m_pKeyboard != nullptr)
		{
			m_pKeyboard->Init(hInstance, hWnd);
		}
	}

	return m_pKeyboard;
}

//=====================================================
// ����������
//=====================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ��{�N���X�̏�����
	CInput::Init(hInstance, hWnd);

	//���̓f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g�̐���
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�̃A�N�Z�X���l��
	m_pDevice->Acquire();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CInputKeyboard::Uninit(void)
{
	m_pKeyboard = nullptr;

	// ��{�N���X�̏I������
	CInput::Uninit();
}

//=====================================================
//�L�[�{�[�h�X�V����
//=====================================================
void CInputKeyboard::Update(void)
{
	//�ϐ��錾
	BYTE aKeyState[NUM_KEY_MAX];						//�L�[�{�[�h���͏��
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			if (m_aKeyStateRepeat[nCntKey] & 0x80)
			{// ���s�[�g�̃J�E���^�[
				m_aCntRepeat[nCntKey]++;
			}
			else if (m_aKeyState[nCntKey] == 0 && aKeyState[nCntKey] == 0)
			{
				m_aCntRepeat[nCntKey] = 0;
			}

			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey] = (m_aKeyState[nCntKey] & aKeyState[nCntKey]);
			m_aKeyState[nCntKey] = aKeyState[nCntKey];			//�L�[�̃v���X����ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//====================================================
// �L�[�{�[�h�f�o�C�X�̍X�V����
//====================================================
void CInputKeyboard::UpdateDevice(void)
{
	m_pDevice->Acquire();
}

//=====================================================
//�v���X���擾
//=====================================================
bool CInputKeyboard::GetPress(int nKey)
{
	//�O�����Z�q
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//=====================================================
//�g���K�[���擾
//=====================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=====================================================
//�����[�X���擾
//=====================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=====================================================
//���s�[�g���擾
//=====================================================
int CInputKeyboard::GetRepeat(int nKey)
{
	return m_aCntRepeat[nKey];
}
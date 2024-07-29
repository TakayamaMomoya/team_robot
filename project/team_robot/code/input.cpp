//*****************************************************
//
// ���͏���[input.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "input.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
LPDIRECTINPUT8 CInput::m_pInput = nullptr;	// DirectInput�I�u�W�F�N�g�ւ̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CInput::CInput()
{
	m_pDevice = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CInput::~CInput()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		//DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CInput::Uninit(void)
{
	if (m_pDevice != nullptr)
	{//���̓f�o�C�X�j��
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	if (m_pInput != nullptr)
	{//DirectInput�I�u�W�F�N�g�̔j��
		m_pInput->Release();
		m_pInput = nullptr;
	}
}
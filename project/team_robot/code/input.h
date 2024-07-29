//*****************************************************
//
// ���͏���[input.h]
// Author:���R����
//
//*****************************************************

#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"

//*****************************************************
// ���C�u����
//*****************************************************
#pragma comment(lib,"xinput.lib")

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CInput
{
public:
	CInput();	// �R���X�g���N�^
	virtual ~CInput();	// �f�X�g���N�^

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void UpdateDevice(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;	// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 m_pDevice;	// �e�f�o�C�X�ւ̃|�C���^
};

#endif
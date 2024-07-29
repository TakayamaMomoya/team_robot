//*****************************************************
//
// �}�E�X���͏���[inputmouse.h]
// Author:���R����
//
//*****************************************************

#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "input.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define NUM_BUTTON_MAX		(8)				//�}�E�X�{�^���̐�

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CInputMouse : public CInput
{
public:
	//�}�E�X�{�^��
	typedef enum
	{
		BUTTON_LMB = 0,					//���N���b�N
		BUTTON_RMB,						//�E�N���b�N
		BUTTON_WHEEL,						//�E�B�[���N���b�N
		BUTTON_SIDE01,						//�T�C�h�{�^���P
		BUTTON_SIDE02,						//�T�C�h�{�^���Q
		BUTTON_MAX
	}BUTTON;

	CInputMouse();
	~CInputMouse();

	static CInputMouse *Create(HINSTANCE hInstance, HWND hWnd);
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void UpdateDevice(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	int GetRepeat(int nKey);
	float GetMoveIX(void);
	float GetMoveIY(void);
	LONG GetMoveIZ(void);
	static CInputMouse *GetInstance(void) { return m_pMouse; }

private:
	DIMOUSESTATE2 m_mouseState;					//�}�E�X�̏��\����
	BYTE m_aKeyState[NUM_BUTTON_MAX];					// �v���X���
	BYTE m_aKeyStateTrigger[NUM_BUTTON_MAX];			// �g���K�[���
	BYTE m_aKeyStateRelease[NUM_BUTTON_MAX];			// �����[�X���
	BYTE m_aKeyStateRepeat[NUM_BUTTON_MAX];			// ���s�[�g���
	int m_aCntRepeat[NUM_BUTTON_MAX];	// ���s�[�g�J�E���^�[
	static CInputMouse *m_pMouse;	// ���g�̃|�C���^
};

#endif
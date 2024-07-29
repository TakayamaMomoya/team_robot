//*****************************************************
//
// �L�[�{�[�h���͏���[inputkeyboard.h]
// Author:���R����
//
//*****************************************************

#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "input.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define NUM_KEY_MAX			(256)			// �L�[�̍ő吔

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	static CInputKeyboard *Create(HINSTANCE hInstance, HWND hWnd);
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void UpdateDevice(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	int GetRepeat(int nKey);
	static CInputKeyboard *GetInstance(void) { return m_pKeyboard; }

private:
	BYTE m_aKeyState[NUM_KEY_MAX];					// �v���X���
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];			// �g���K�[���
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];			// �����[�X���
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];			// ���s�[�g���
	int m_aCntRepeat[NUM_KEY_MAX];	// ���s�[�g�J�E���^�[
	static CInputKeyboard *m_pKeyboard;	// ���g�̃|�C���^
};

#endif
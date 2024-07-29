//*****************************************************
//
// ���̓}�l�[�W���[[inputManager.h]
// Author:���R����
//
//*****************************************************

#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CInputManager
{
public:
	enum BUTTON
	{
		BUTTON_ENTER = 0,	// �G���^�[�{�^��
		BUTTON_BACK,	// �߂�{�^��
		BUTTON_PAUSE,	// �|�[�Y
		BUTTON_AXIS_UP,	// ������L�[
		BUTTON_AXIS_DOWN,	// �������L�[
		BUTTON_AXIS_RIGHT,	// �E�����L�[
		BUTTON_AXIS_LEFT,	// �������L�[
		BUTTON_TRIGGER_UP,	// ������e��
		BUTTON_TRIGGER_DOWN,	// �������e��
		BUTTON_TRIGGER_RIGHT,	// �E�����e��
		BUTTON_TRIGGER_LEFT,	// �������e��
		BUTTON_KATANA,	// ����U��
		BUTTON_MAX
	};
	struct SAxis
	{// �����̂܂Ƃ�
		D3DXVECTOR3 axisMove;	// �ړ�����
		D3DXVECTOR3 axisCamera;	// �J�����𓮂�������
	};

	CInputManager();	// �R���X�g���N�^
	~CInputManager();	// �f�X�g���N�^

	static CInputManager *Create(HINSTANCE hInstance, HWND hWnd);
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void UpdateDevice(void);
	static CInputManager *GetInstance(void) { return m_pInputManager; }
	bool GetTrigger(BUTTON button) { return m_info.abTrigger[button]; }
	bool GetPress(BUTTON button) { return m_info.abPress[button]; }
	SAxis GetAxis(void) { return m_axis; }
	float GetAccele(void) { return m_fAccele; }
	float GetBrake(void) { return m_fBreake; }

private:
	struct SInfo
	{// ���̍\����
		bool abTrigger[BUTTON_MAX];	// �g���K�[���
		bool abPress[BUTTON_MAX];	// �v���X���
	};

	static CInputManager *m_pInputManager;	// ���g�̃|�C���^
	SInfo m_info;	// ���
	SAxis m_axis;	// �����̏��
	float m_fAccele;	// �A�N�Z���̓���
	float m_fBreake;	// �u���[�L�̓���
};

#endif

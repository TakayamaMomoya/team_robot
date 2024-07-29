//*****************************************************
//
// �W���C�p�b�h���͏���[inputjoypad.h]
// Author:���R����
//
//*****************************************************

#ifndef _INPUTJOYPAD_H_
#define _INPUTJOYPAD_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "input.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_PLAYER	(4)	// �v���C���[�̍ő吔

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CInputJoypad
{
public:
	typedef enum
	{//�Q�[���p�b�h�{�^��
		PADBUTTONS_UP = 0,
		PADBUTTONS_DOWN,
		PADBUTTONS_LEFT,
		PADBUTTONS_RIGHT,
		PADBUTTONS_START,
		PADBUTTONS_BACK,
		PADBUTTONS_LSTICK,
		PADBUTTONS_RSTICK,
		PADBUTTONS_LB,
		PADBUTTONS_RB,
		PADBUTTONS_UNKNOWN,
		PADBUTTONS_UNKNOWN2,
		PADBUTTONS_A,
		PADBUTTONS_B,
		PADBUTTONS_X,
		PADBUTTONS_Y,
		PADBUTTONS_MAX
	}PADBUTTOS;
	enum TRIGGER
	{// �g���K�[����
		TRIGGER_RT = 0,
		TRIGGER_LT,
		TRIGGER_MAX
	};
	typedef enum
	{//�o�C�u���[�V�����̏��
		PADVIB_NONE = 0,						//�g�p���Ă��Ȃ����
		PADVIB_USE,								//�g�p���Ă�����
		PADVIB_MAX
	}PADVIB;
	enum DIRECTION
	{// �X�e�B�b�N�̃g���K�[
		DIRECTION_UP = 0,
		DIRECTION_DOWN,
		DIRECTION_RIGHT,
		DIRECTION_LEFT,
		DIRECTION_MAX
	};

	CInputJoypad();	// �R���X�g���N�^
	~CInputJoypad();	// �f�X�g���N�^

	static CInputJoypad *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void UpdateDevice(void);
	bool GetPress(PADBUTTOS nKey, int nPlayer);
	bool GetTrigger(PADBUTTOS nKey, int nPlayer);
	bool GetRelease(PADBUTTOS nKey, int nPlayer);
	int GetRepeat(PADBUTTOS nKey, int nPlayer);
	bool GetTriggerTB(TRIGGER trigger, int nPlayer);
	bool GetPressTB(TRIGGER trigger,int nPlayer);
	float GetJoyStickLX(int nPlayer);
	float GetJoyStickLY(int nPlayer);
	float GetJoyStickRX(int nPlayer);
	float GetJoyStickRY(int nPlayer);
	bool GetLStickTrigger(DIRECTION direction, int nPlayer);
	bool GetRStickTrigger(DIRECTION direction,int nPlayer);
	float GetTriggerL(int nPlayer);
	float GetTriggerR(int nPlayer);
	D3DXVECTOR3 GetVecStickL(void);
	void Vibration(int nPlayer, PADVIB state, float fVib, int nTime);
	static CInputJoypad *GetInstance(void) { return m_pJoyPad; }

private:
	void CheckTrigger(XINPUT_STATE state, int nPlayer);
	void CheckStickTrigger(XINPUT_STATE state,int nPlayer);

	XINPUT_STATE m_aState[MAX_PLAYER];				//�Q�[���p�b�h�̃v���X���
	XINPUT_STATE m_aStateTrigger[MAX_PLAYER];		//�Q�[���p�b�h�̃g���K�[���
	XINPUT_STATE m_aStateRelease[MAX_PLAYER];		//�Q�[���p�b�h�̃����[�X���
	XINPUT_STATE m_aStateRepeat[MAX_PLAYER];			//�Q�[���p�b�h�̃��s�[�g���
	XINPUT_VIBRATION m_aVibration[MAX_PLAYER];		//�Q�[���p�b�h�̐U�����
	PADVIB m_aVibState[MAX_PLAYER];					//�U���̏��
	int m_aVibTimer[MAX_PLAYER];		// �o�C�u���[�V�����̃^�C�}�[
	int m_aCntRepeat[MAX_PLAYER][PADBUTTONS_MAX];	// ���s�[�g�J�E���^�[
	bool m_abTrigggerLStick[MAX_PLAYER][DIRECTION_MAX];	// �X�e�B�b�N�̃g���K�[����
	bool m_abTrigggerRStick[MAX_PLAYER][DIRECTION_MAX];	// �X�e�B�b�N�̃g���K�[����
	bool m_abTriggerTB[MAX_PLAYER][TRIGGER_MAX];	// �g���K�[�̔���
	bool m_abPressTB[MAX_PLAYER][TRIGGER_MAX];	// �v���X�̔���
	BYTE m_abyTriggerL[MAX_PLAYER];	// ���g���K�[
	BYTE m_abyTriggerR[MAX_PLAYER];	// �E�g���K�[

	static CInputJoypad *m_pJoyPad;	// ���g�̃|�C���^
};

#endif

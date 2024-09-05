//*****************************************************
//
// �v���C���[�̏���[player.cpp]
// Author:���R����
//
//*****************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"
#include "playercontroller.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayer : public CMotion
{
public:
	// �񋓌^��`
	enum E_Motion
	{// ���[�V����
		MOTION_NEUTRAL = 0,	// �ҋ@
		MOTION_WALK_FRONT,	// �O��
		MOTION_MAX
	};

	// �\���̒�`
	struct S_Param
	{// �p�����[�^�[
		string m_stPathBody = "";	// �����ڂ̃o�X
	};

	CPlayer(int nPriority = 4);	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void Hit(float fDamage);	// �q�b�g����

	// �ϐ��擾�E�ݒ�֐�
	void EnableLockon(bool bLockon) { m_bLockOn = bLockon; }	// ���b�N�I���t���O
	bool IsLockon(void) { return m_bLockOn; }

	// �ÓI�����o�֐�
	static CPlayer *Create(void);	// ��������
	static CPlayer *GetInstance(void) { return s_pPlayer; }	// �C���X�^���X�̎擾

private:
	// �\���̒�`
	struct S_FragMotion
	{// ���[�V�����̃t���O
		bool bMove = false;	// �ړ�
	};

	// �����o�֐�
	void Load(void);	// �p�����[�^�[�Ǎ�
	void Lockon(void);	// ���b�N�I�����
	void Input(void);	// ����
	void InputMove(void);	// �ړ��̓���
	void Event(EVENT_INFO *pEventInfo);	// �C�x���g�̏���
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	S_FragMotion m_fragMotion;	// ���[�V�����̃t���O
	S_Param m_param;	// �p�����[�^�[
	CPlayerController *m_pController;	// �R���g���[���[
	bool m_bLockOn;	// ���b�N�I���t���O

	// �ÓI�����o�ϐ�
	static CPlayer *s_pPlayer;	// ���g�̃|�C���^
};

namespace Player
{
const D3DXVECTOR3 DEFAULT_POS = { 0.0f,0.0f,0.0f };	// �����ʒu
const D3DXVECTOR3 DEFAULT_ROT = { 0.0f,0.0f,0.0f };	// ��������
}

#endif
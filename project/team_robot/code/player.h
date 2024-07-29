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

//*****************************************************
// �O���錾
//*****************************************************
class CBlockGrab;
class CPolygon3D;
class CPlayerNinja;
class CMeshRoad;
class COrbit;
class CEffekseerEffect;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayer : public CMotion
{
public:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// �ҋ@
		MOTION_WALK_FRONT,	// �O��
		MOTION_MAX
	};
	enum MOTION_NINJA
	{
		MOTION_NINJA_NEUTRAL = 0,	// �ҋ@
		MOTION_NINJA_SLASHDOWN,	// �؂艺�낵
		MOTION_NINJA_SLASHUP,	// �؂�グ
		MOTION_NINJA_MAX
	};
	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_MAX
	};
	struct SParam
	{
		char aPathBody[MAX_STRING];	// �����ڂ̃p�X
		float fSpeedMax;	// �ő呬�x
		float fSpeedMaxInitial;	// �����ő呬�x
		float fFactAccele;	// �����W��
		float fFactAttenu;	// �����W��
		float fFactBrake;	// �u���[�L�����W��
		float fAngleMaxCurve;	// �J�[�u�̍ő�p�x
		D3DXVECTOR3 sizeCollider;	// �R���C�_�[�̃T�C�Y
	};

	CPlayer(int nPriority = 4);	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	static CPlayer *Create(void);
	static CPlayer *GetInstance(void) { return m_pPlayer; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage);
	SParam GetParam(void) { return m_param; }
	void SetParam(SParam param) { m_param = param; }
	CBlockGrab *GetBlock(void) { return m_info.pBlockGrab; }
	CMotion *GetNInjaBody(void) { return m_pPlayerNinja; }	// ��ɏ���Ă���E�҂̎擾
	bool IsDrift(void) { return m_info.pBlockGrab != nullptr; }	// �h���t�g���Ă��邩�ǂ���

	// �ϐ��擾�E�ݒ�֐�
	bool IsEnableInput(void) { return m_info.bEnableInput; }	// ���͗L���t���O
	void SetEnableInput(bool bEnable) { m_info.bEnableInput = bEnable; }
	float GetSpeed(void) { return m_info.fSpeed; }	// �X�s�[�h
	void SetSpeed(float fSpeed) { m_info.fSpeed = fSpeed; }

private:
	struct SFragMotion
	{
		bool bMove;	// �ړ�
	};
	struct SFragNinja
	{
		bool bNeutral;	// �ҋ@���[�V����
		bool bSlashDown;	// �؂艺�낵
		bool bSlashUp;	// �؂�グ
	};
	struct SInfo
	{
		STATE state;	// ���
		float fSpeed;	// ���݂̑��x
		float fSpeedDest;	// �ڕW���x
		float fAngleHandle;	// �n���h���������Ă���l
		float fTimerFlip;	// �h���t�g�J�E���^�[
		float fAngleDrift;	// �h���t�g���̊p�x�{���̒l
		int nCntFlip;	// �e���J�E���^�[
		CBlockGrab *pBlockGrab;	// �͂�ł���u���b�N
		CPolygon3D *pRoap;
		float fCntAngle;	// �A���O���̃J�E���^�[
		bool bGrabOld;
		bool bManual;		// �}�j���A������
		bool bAir;			// �󒆂ɂ��邩�ǂ���
		bool bEnableInput;	// ���͗L���t���O
		float fLengthDrift;
		float fTimerDriftChange;
		float fSizeBlurDrift;	// �h���t�g���̃u���[�̋���
		float fDesityBlurDrift;	// �h���t�g���̃u���[�̔Z��
		COrbit* pOrbitLamp;	// �e�[�������v�̋O��
		COrbit* pOrbitRope;	// �e�[�������v�̋O��
		D3DXCOLOR orbitColor;	// �e�[�������v�̋O��
		D3DXVECTOR3 rotDriftStart;	// �h���t�g�X�^�[�g���̊p�x
		float rotDriftDest;	// �h���t�g�I�����̊p�x�␳�l
	};

	void Load(void);
	void Input(void);
	void Collision(void);
	void InputMove(void);
	void InputWire(void);
	void InputKatana(void);
	void ManageKanataAtttack(void);	// ���̍U���Ǘ�
	void JudgeChangeDrift(float fAngle, float fAngleDiff, float fLength);	// �h���t�g�ω��̔���
	void ControlRoap(void);	// ���[�v�̐���
	void SarchGrab(void);	// �͂ރu���b�N�̒T�m
	void ManageRotateGrab(float fAngleDiff);	// �͂�ł���Ƃ��̉�]����
	void JudgeRemoveWire(float fLength);	// ���C���[���O�����̔���
	void RemoveWire(void);	// ���C���[���O������
	void LimitDrift(float fLength);
	void ManageSpeed(void);
	void ManageSpeedBlur(void);	// �X�s�[�h�ɂ��u���[�̊Ǘ�
	void ManageState(void);
	void ManageMotion(void);
	void ManageMotionNinja(void);	// �E�҂̃��[�V�����Ǘ�
	void ManageSlashEffect(void);	// �a���G�t�F�N�g�̊Ǘ�
	void Event(EVENT_INFO *pEventInfo);
	void Debug(void);

	// �����o�ϐ�
	SInfo m_info;	// ���g�̏��
	SParam m_param;	// �p�����[�^���
	SFragMotion m_fragMotion;	// ���[�V�����̃t���O
	SFragNinja m_fragNinja;	// �E�҂̃��[�V�����t���O
	CMotion* m_pPlayerNinja;  // �o�C�N�ɏ���Ă���E��
	bool m_bMove = false;     // �ړ����Ă��邩�ǂ���
	bool m_bDrift = false;    // �h���t�g���Ă��邩�ǂ���
	std::list<CEffekseerEffect*> m_listSlashEffect;	// �a���G�t�F�N�g�̃��X�g

	// �ÓI�����o�ϐ�
	static CPlayer *m_pPlayer;	// ���g�̃|�C���^
};

namespace Player
{
const D3DXVECTOR3 DEFAULT_POS = { 8500.1f,800.0f,-28621.6f };	// �����ʒu
const D3DXVECTOR3 DEFAULT_ROT = { 0.0f,2.0f,0.0f };	// ��������
}

#endif
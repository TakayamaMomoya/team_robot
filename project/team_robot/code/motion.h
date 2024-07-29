//*****************************************************
//
// ���[�V�����̏���[motion.h]
// Author:���R����
//
//*****************************************************

#ifndef _MOTION_H_
#define _MOTION_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "parts.h"
#include "object3D.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_PARTS	(20)	// �p�[�c�̍ő吔
#define MAX_MOTION	(40)	// ���[�V�����̍ő吔

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMotion : public CObject3D
{
public:
	//�L�[�̍\����
	typedef struct
	{
		float fPosX;				//�ʒuX
		float fPosY;				//�ʒuY
		float fPosZ;				//�ʒuZ
		float fRotX;				//����X
		float fRotY;				//����Y
		float fRotZ;				//����Z
	}KEY;

	//�L�[���\����
	typedef struct
	{
		int nFrame;					//�Đ��t���[��
		KEY aKey[MAX_PARTS];			//�e���f���̃L�[�v�f
	}KEY_INFO;

	typedef struct
	{// �p�[�e�B�N�����\����
		int nKey;	//��������L�[
		int nFrame;	//��������t���[��
		float fNumFrame;    // �����t���[��
		float fTimer;    // �����^�C�}�[
		float fRadius;    // ���a
		D3DXVECTOR3 offset;	// �I�t�Z�b�g�ʒu
		int nIdxParent;	// �e�ƂȂ�p�[�c�̔ԍ�
	}EVENT_INFO;
	
	//���[�V�������̍\����
	typedef struct
	{
		bool bLoop;						//���[�v���邩�ǂ���
		int nNumKey;					//�L�[�̑���
		KEY_INFO aKeyInfo[MAX_PARTS];	//�L�[���
		int nNumEvent;	// �p�[�e�B�N���̐�
		int nNumCollision;	// �����蔻��̐�
		EVENT_INFO *pEvent;	// �C�x���g���̃|�C���^
	}MOTION_INFO;

	// �p�[�c�̍\����
	typedef struct
	{
		int nIdx;
		int nIdxParent;
		CParts *pParts;
	}Parts;
	
	CMotion(int nPriority = 5);	// �R���X�g���N�^
	~CMotion();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Load(char *pPath);
	void MultiplyMtx(bool bDrawParts = true);	// �}�g���b�N�X���������킹��
	void SetPosShadow(D3DXVECTOR3 pos) { m_posShadow = pos; }	// �ݒ菈��
	void SetPositionOld(D3DXVECTOR3 pos) { m_posOld = pos; }	// �ݒ菈��
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	D3DXVECTOR3 GetMtxPos(int nIdx);
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }	// �擾����
	void SetMotion(int nMotionType);
	int GetMotion(void) { return m_motionType; }
	void SetKeyOld(void);
	static CMotion *Create(char *pPath);
	Parts *GetParts(int nIdx) { return m_apParts[nIdx]; }
	bool IsFinish(void) { return m_bFinish; }
	void CalcMatrix(void);
	float GetRadiusMax(void);
	void SetAfterImage(D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f }, int m_nLife = 10);
	int GetKey(void) { return m_nKey; }
	float GetFrame(void) { return m_fCounterMotion; }
	void SetAllCol(D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; }
	void ResetAllCol(void);
	void InitPose(int nMotion);
	void EnableShadow(bool bShadow) { m_bShadow = bShadow; }
	void EnableIndependent(bool bInde) { m_bInde = bInde; }
	bool IsIndependent(void) { return m_bInde; }
	EVENT_INFO *GetInfoEvent(int nMotion) { return m_aMotionInfo[nMotion].pEvent; }
	int GetNumEventInfo(int nMotion) { return m_aMotionInfo[nMotion].nNumEvent; }
	virtual void Event(EVENT_INFO *pEventInfo) {};
	void EnableMotion(int nIdx, bool bMotion);
	void ResetEnableMotion(void);
	D3DXVECTOR3 GetForward(void);
	int GetNumParts(void) { return m_nNumParts; }

private:
	Parts *m_apParts[MAX_PARTS];	// �p�[�c�̍\����
	MOTION_INFO m_aMotionInfo[MAX_MOTION];	//���[�V�������̍\����
	KEY m_aKeyOld[MAX_PARTS];	// �O��̃L�[���̍\����
	bool m_abMotion[MAX_PARTS];	// �p�[�c���ƂɃ��[�V���������邩�ǂ���
	int m_nNumMotion;	//���[�V�����̑���
	int m_motionType;	//���[�V�����̎��
	int m_motionTypeOld;	//�O��̃��[�V�����̎��
	int m_nFrame;	// �t���[����
	bool m_bLoopMotion;	//���[�v���邩�ǂ���
	int m_nNumKey;	//�L�[�̑���
	int m_nKey;	//���݂̃L�[
	float m_fCounterMotion;	//���[�V�����J�E���^�[
	int m_nNumParts;	// �p�[�c�̐�
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_posShadow;	// �e�̈ʒu
	D3DXVECTOR3 m_move;	// �ړ���
	D3DXCOLOR m_col;	// �F
	bool m_bFinish;	// ���[�V�������I��������ǂ���
	bool m_bShadow;	// �e��`�悷�邩�ǂ���
	bool m_bInde;	// �������Ă��邩�ǂ���
};

#endif
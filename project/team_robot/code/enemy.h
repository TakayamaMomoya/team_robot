//*****************************************************
//
// �G�l�~�[�̏���[enemy.h]
// Author:�匴�叫
//
//*****************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"
#include "enemyBehaviour.h"

//*****************************************************
// �O����`
//*****************************************************
class CBehaviourEnemy;
class COrbit;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemy : public CMotion
{
public:
	// �񋓌^��`
	enum MOTION
	{// ���[�V����
		MOTION_NEUTRAL = 0,	// �ҋ@���
		MOTION_ATTACK,	// �������[�V����
		MOTION_MAX
	};

	// �\���̒�`
	struct SFragMotion
	{// ���[�V�����t���O
		bool bAttack;	// �U��
	};

	CEnemy(int nPriority = 4);	// �R���X�g���N�^
	~CEnemy();	                // �f�X�g���N�^

	HRESULT Init(void);         // ����������
	void Uninit(void);          // �I������
	void Update(void);          // �X�V����
	void Draw(void);            // �`�揈��
	SFragMotion *GetFragMotion(void) { return &m_fragMotion; }	// ���[�V�����t���O�̎擾

	// �ϐ��擾�E�ݒ�֐�
	int GetIdxSpline(void) { return m_info.nIdxSpline; }	// �X�v���C���ԍ�
	void SetIdxSpline(int nIdx) { m_info.nIdxSpline = nIdx; }

	// �ÓI�����o�֐�
	static CEnemy* Create();    // ��������

private:
	enum STATE
	{// ���
		STATE_NONE = 0,	         // ���ł��Ȃ����
		STATE_NORMAL,	         // �ʏ���
		STATE_MAX
	};
	struct SInfo
	{// ���
		STATE state;             // ���[
		int nIdxSpline;	// �X�v���C���̔ԍ�
		COrbit* pOrbit;	// �e�[�������v�̋O��
	};

	// �����o�֐�
	void ManageMotion(void);	// ���[�V�����̊Ǘ�
	void ReleaseAllBehaviour(void);	// �S�Ẵr�w�C�r�A�̉��

	// �����o�ϐ�
	SInfo m_info;	// ���
	SFragMotion m_fragMotion;	// ���[�V�����t���O
	std::list<CEnemyBehaviour*> m_listBehaviour;	// �r�w�C�r�A�̃��X�g
};

#endif
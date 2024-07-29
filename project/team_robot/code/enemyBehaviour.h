//*****************************************************
//
// �G�l�~�[�r�w�C�r�A[enemyBehaviour.h]
// Author:���R����
//
//*****************************************************
#ifndef _ENEMYBEHAVIOUR_H_
#define _ENEMYBEHAVIOUR_H_

//*****************************************************
// �C���N���[�h
//*****************************************************

//*****************************************************
// �O����`
//*****************************************************
class CEnemy;
class CCutMullSpline;
class CObjectX;
class CEffekseerEffect;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyBehaviour
{// �r�w�C�r�A�̊��N���X
public:
	CEnemyBehaviour() {};	// �R���X�g���N�^
	virtual ~CEnemyBehaviour() {};	 // �f�X�g���N�^

	// �����o�֐�
	virtual void Init(CEnemy *pEnemy) = 0;	// ����������
	virtual void Uninit(CEnemy *pEnemy) = 0;	// �I������
	virtual void Update(CEnemy *pEnemy) = 0;	// �X�V����

private:
};

class CEnemyBehaviourChasePlayer : public CEnemyBehaviour
{// �v���C���[�̒ǐ�
public:
	CEnemyBehaviourChasePlayer();	// �R���X�g���N�^
	virtual ~CEnemyBehaviourChasePlayer();	 // �f�X�g���N�^

	// �����o�֐�
	void Init(CEnemy *pEnemy) override;	// ����������
	void Uninit(CEnemy *pEnemy) override;	// �I������
	void Update(CEnemy *pEnemy) override;	// �X�V����

private:
	// �񋓌^��`
	enum STATE
	{
		STATE_NONE = 0,	// �������Ă��Ȃ����
		STATE_CHASE,	// �ǐՏ��
		STATE_ATTACK,	// �U�����
		STATE_ESCAPE,   // �������
		STATE_MAX
	};

	// �����o�֐�
	void ManageState(CEnemy *pEnemy);	// ��Ԃ̊Ǘ�
	bool CollidePlayerFront(CEnemy *pEnemy);	// �v���C���[�̑O�ɏo������
	void CalcSpeed(CEnemy *pEnemy);	// �X�s�[�h�̌v�Z
	void InterpolatePosition(CEnemy *pEnemy);	// �ʒu�̕��
	void ControllRot(CEnemy *pEnemy);	// �����̐���
	void CreateBigShuriken(void);	// �ł����藠���̐���
	void FollowBigShuriken(CEnemy *pEnemy);	// �ł����藠���̒Ǐ]
	void ThrowShuriken(CEnemy *pEnemy);	// �藠���𓊂���
	void ThrowEffect(CEnemy *pEnemy);	// �G�t�F�N�g�̒Ǐ]
	void Debug(CEnemy *pEnemy);	// �f�o�b�O�\��

	// �����o�ϐ�
	STATE m_state;	// ���
	CCutMullSpline *m_pSpline;	// �X�v���C��
	std::vector<D3DXVECTOR3> m_vPos;	// �f�[�^�_�̃x�N�^�[
	float m_fSpeed;	// ���x
	float m_fSpeedDefault;	// ��ɂ��鑬�x
	float m_fRate;
	float m_fRateOld;	// �O��̃X�v���C���̊���
	float m_fTimerAttack;	// �U���^�C�}�[
	int m_nNumShuriken;  // �����Ă���藠���̐�
	CEffekseerEffect *m_pBigShuriken;	// ������O�Ɏ茳�ɏo���傫�Ȏ藠��
	CEffekseerEffect *m_pFlashEffect;	// ������O�̑M���̃G�t�F�N�g
};

#endif
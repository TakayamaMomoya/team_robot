//*****************************************************
//
// �S�[������[goal.h]
// Author:�X��x��
//
//*****************************************************

#ifndef _GOAL_H_
#define _GOAL_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "player.h"
#include "polygon3D.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CGoal : public CObject
{
public:
	CGoal(int nPriority = 7);	// �R���X�g���N�^
	~CGoal();	//	�f�X�g���N�^

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetGoal(void);	// �S�[���̐ݒ�

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// �ʒu�擾
	void SetRotation(float fRot) { m_fRot = fRot; }	// �����擾�ݒ�
	float GetRotation() { return m_fRot; }	// �����擾����

	// �ÓI�����o�֐�
	static CGoal* Create(D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f }, float fRot = 0.0f, float fLength = 1000.0f);	// ����
	static CGoal *GetInstance(void) { return m_pGoal; }

private:
	//�����o�ϐ�
	int m_nTransitionTime;		// �J�ڎ���
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posStart;	// �S�[���̎n�_
	D3DXVECTOR3 m_posEnd;	// �S�[���̏I�_
	float m_fRot;		// ����
	float m_fLength;	// ����
	CPolygon3D* m_pObj3D;	// �I�u�W�F�N�g3D�̃|�C���^

	// �ÓI�����o�ϐ�
	static CGoal *m_pGoal;	// ���g�̃|�C���^
};

#endif

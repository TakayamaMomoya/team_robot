//*****************************************************
//
// ����[Myphysics.h]
// Author �匴�叫
//
//*****************************************************
#ifndef _MYPHYSICS_H_             //���̃}�N����`������ĂȂ�������
#define _MYPHYSICS_H_             //2�d�C���N���[�h�h�~�̃}�N����`������

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "btBulletDynamicsCommon.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObjectX;

//*****************************************************
// �N���X��`
//*****************************************************
class CPhysics
{
public:
	CPhysics();
	~CPhysics();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	btDiscreteDynamicsWorld* GetDynamicsWorld(void) { return m_pDynamicsWorld; }

private:

	btDiscreteDynamicsWorld* m_pDynamicsWorld;  //�_�C�i�~�N�X���[���h
	btAxisSweep3* m_pBroadphase;  // 
	btDefaultCollisionConfiguration* m_pCollisionConfiguration;  // �f�t�H���g�̏Փ˔���
	btCollisionDispatcher* m_pDispatcher;  // �f�B�X�p�b�`
	btSequentialImpulseConstraintSolver* m_pSolver;  // �Փˉ����\���o
	int allFrame;
};

#endif
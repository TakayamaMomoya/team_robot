//*****************************************************
//
// �Q�[���I�u�W�F�N�g[gameObject.cpp]
// Author:���R����
//
//*****************************************************
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CGameObject : public CObject
{
public:
	CGameObject(int nPriority) : CObject(nPriority), m_pos(), m_rot() {};	// �R���X�g���N�^
	~CGameObject() {};	// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) = 0;	// ������
	virtual void Uninit(void) { CObject::Release(); }	// �I��
	virtual void Update(void) = 0;	// �X�V
	virtual void Draw(void) = 0;	// �`��

	// �ϐ��擾�E�ݒ�֐�
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// �ʒu
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }	// ����
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }

private:
	// �����o�ϐ�
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_rot;	// ����
};

#endif
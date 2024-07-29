//*****************************************************
//
// �G�f�B�^�[�̏���[edit.h]
// Author:���R����
//
//*****************************************************
#ifndef _EDIT_H_
#define _EDIT_H_

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEdit
{
public:
	CEdit();	// �R���X�g���N�^
	~CEdit();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void TeleportPlayer(void);

private:
	// �����o�֐�

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_rot;	// ����
};

#endif
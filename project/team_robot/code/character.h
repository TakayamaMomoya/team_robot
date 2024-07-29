//*****************************************************
//
// �L�����N�^�[�̏���[character.h]
// Author:���R����
//
//*****************************************************

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CMotion;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CCharacter : public CObject
{
public:
	CCharacter(int nPriority = 4);	// �R���X�g���N�^
	~CCharacter();	// �f�X�g���N�^

	static CCharacter *Create(char *pPath);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_info.pos; }
	void SetPositionOld(D3DXVECTOR3 pos) { m_info.posOld = pos; }
	D3DXVECTOR3 GetPositionOld(void) { return m_info.posOld; }
	void SetMove(D3DXVECTOR3 move) { m_info.move = move; }
	D3DXVECTOR3 GetMove(void) { return m_info.move; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }
	D3DXVECTOR3 GetRotation(void) { return m_info.rot; }
	CMotion *GetBody(void) { return m_info.pBody; }
	void Load(char *pPath);
	void SetMotion(int nMotion);
	int GetMotion(void);

private:
	struct SInfo
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 posOld;	// �O��̈ʒu
		D3DXVECTOR3 move;	// �ړ���
		D3DXVECTOR3 rot;	// ����
		CMotion *pBody;	// ��
		char *pPath;	// �̂̃p�X
	};

	SInfo m_info;	// ���g�̏��
};

#endif
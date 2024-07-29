//*****************************************************
//
// �u���b�N�̏���[block.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "ObjectX.h"

//*****************************************************
// �O���錾
//*****************************************************
class CFan3D;
class CPolygon3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
//=====================================================
// �ʏ�u���b�N�N���X
//=====================================================
class CBlock : public CObjectX
{
public:
	typedef enum
	{// ���
		TYPE_WALL = 0,	// ��
		TYPE_MAX
	}TYPE;

	typedef enum
	{// �s��
		BEHAVIOUR_NORMAL = 0,	// �ʏ�
		BEHAVIOUR_GRAB,	// �͂߂����
		BEHAVIOUR_MAX
	}BEHAVIOUR;

	typedef struct
	{// �ۑ�����Ƃ��̏��
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		TYPE type;
	}MemBlock;

	CBlock(int nPriority = 3);	// �R���X�g���N�^
	~CBlock();	// �f�X�g���N�^

	static CBlock *Create(BEHAVIOUR behaviour = BEHAVIOUR_NORMAL);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int GetIdx(void) { return m_nIdx; }
	void SetIdx(int nIdx) { m_nIdx = nIdx; }
	CBlock *GetNext(void) { return m_pNext; }
	CBlock *GetPrev(void) { return m_pPrev; }
	static int GetNumAll(void) { return m_nNumAll; }
	void SetPosition(D3DXVECTOR3 pos);
	virtual void Save(FILE *pFile);	// �ۑ�����
	virtual void Load(FILE *pFile, char* pTemp);	// �Ǎ�����

	bool Collide(D3DXVECTOR3* pPos, D3DXVECTOR3 posOld);

private:
	static int m_nNumAll;	// ����
	float m_fLife;	// �̗�
	int m_nIdx;	// ��ނ̃C���f�b�N�X

	CBlock *m_pPrev;	// �O�̃A�h���X
	CBlock *m_pNext;	// ���̃A�h���X
};

//=====================================================
// �͂ރu���b�N�N���X
//=====================================================
class CBlockGrab : public CBlock
{
public:
	CBlockGrab();	// �R���X�g���N�^
	~CBlockGrab();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool CanGrab(D3DXVECTOR3 pos);
	void Save(FILE *pFile) override;	// �ۑ�����
	void Load(FILE *pFile, char* pTemp) override;	// �Ǎ�����
	void SetFan(void);	// ��|���S���̐ݒ�

	// �ϐ��擾�E�ݒ�
	void SetPosition(D3DXVECTOR3 pos);	// �ʒu�̐ݒ�
	void SetRotation(D3DXVECTOR3 rot);	// �����̐ݒ�
	void EnableCurrent(bool bCurrent) { m_bCurrent = bCurrent; }	// �I���t���O
	bool IsCurrent(void) { return m_bCurrent; }
	float GetRadiusOffset(void) { return m_fRadiusOffset; }	// �I�t�Z�b�g���a
	void SetRadiusOffset(float fRadius) { m_fRadiusOffset = fRadius; }
	float GetAngleOffset(int nIdx) { return m_afAngleOffset[nIdx]; }	// �I�t�Z�b�g�p�x
	void SetAngleOffset(float fAngle, int nIdx) { m_afAngleOffset[nIdx] = fAngle; }

private:

	// �����o�֐�
	void GrabPointUI(void);  // ���|�C���gUI�\��

	// �萔
	static const int NUM_OFFSET = 2;	// �I�t�Z�b�g�̐�

	// �����o�ϐ�
	float m_afAngleOffset[NUM_OFFSET];	// �I�t�Z�b�g�̊p�x
	float m_fRadiusOffset;	// �I�t�Z�b�g�̔��a
	bool m_bCurrent;	// �I������Ă��邩�ǂ���
	CFan3D *m_pFan;	// ��������p�̐�|���S��
	CPolygon3D *m_pGrabPointUI;  // ���|�C���gUI�\���p�|���S��
	float m_fColA = 0.0f;  // ���|�C���gUI�\���p�|���S���̃��l

};

#endif

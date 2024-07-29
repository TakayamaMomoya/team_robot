//******************************************
//
//  �O�Ղ̐���(orbit.h)
//  Author : ���R�@����
//
//******************************************

#ifndef _ORBIT_H_
#define _ORBIT_H_

//******************************************
// �C���N���[�h
//******************************************
#include "main.h"
#include "object.h"

//******************************************
// �}�N����`
//******************************************
#define MAX_ORBIT	(64)	//�O�Ղ̍ő吔
#define MAX_EDGE	(500)	//�ӂ̍ő吔
#define NUM_OFFSET	(2)	//�I�t�Z�b�g�̐�

//******************************************
// �N���X��`
//******************************************
class COrbit : public CObject
{
public:
	COrbit(int nPriority = 6);	// �R���X�g���N�^
	~COrbit();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetOffset(D3DXMATRIX mtxWorld, D3DXCOLOR col, int nIdxOrbit);
	static COrbit *Create(D3DXMATRIX mtxWorld, D3DXVECTOR3 posOffset1, D3DXVECTOR3 posOffset2, D3DXCOLOR col, int nNumEdge);
	bool IsEnd(void) { return m_bEnd; }
	void SetEnd(bool bEnd) { m_bEnd = bEnd; }
	int GetID(void) { return m_nID; }

private:
	void UpdatePolygon(void);

	int m_nIdxTexture;	// �e�N�X�`���ԍ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_posOffset[NUM_OFFSET];	// �I�t�Z�b�g�ʒu
	D3DXVECTOR3 m_aPosPoint[MAX_EDGE][NUM_OFFSET];	// ���_���W�ۑ��p
	D3DXCOLOR m_colOffset[NUM_OFFSET];	// �F
	D3DXCOLOR m_aColPoint[MAX_EDGE][NUM_OFFSET];	// ���_�J���[�ۑ��p
	D3DXMATRIX m_aMtxOffset[NUM_OFFSET];	// �I�t�Z�b�g�̃}�g���b�N�X
	int m_nNumEdge;	//�ӂ̐�
	int m_nID;	// �ԍ�
	bool m_bEnd;	// �I�����Ă邩�ǂ���
};

#endif
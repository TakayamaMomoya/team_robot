//*****************************************************
//
// �c���̏���[afterimage.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _AFTERIMAGE_H_
#define _AFTERIMAGE_H_

#include "ObjectX.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionCube;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CAfterImage : public CObjectX
{
public:
	CAfterImage(int nPriority = 6);	// �R���X�g���N�^
	~CAfterImage();	// �f�X�g���N�^

	static CAfterImage *Create(D3DXMATRIX mtx, int nIdxModel,D3DXCOLOR col,int m_nLife);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static int m_nNumAll;	// ����
	int m_nLife;	// �̗�
	float m_fDecAlpha;	// �����x������
};

#endif

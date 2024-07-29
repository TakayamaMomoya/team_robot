//*****************************************************
//
// 3D�G�t�F�N�g����[effect3D.h]
// Author:���R����
//
//*****************************************************

#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "polygon3D.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEffect3D : public CPolygon3D
{
public:
	CEffect3D(int nPriority = 4);	// �R���X�g���N�^
	~CEffect3D();	// �f�X�g���N�^

	static CEffect3D *Create(D3DXVECTOR3 pos, float fRadius, int nLife, D3DXCOLOR col, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f }, float fGravity = 0.0f, bool bAdd = true,float fDecrease = 0.0f,D3DXVECTOR3 *pPosOwner = nullptr,int nPriority = 6,bool bTurn = false);
	static CEffect3D* Create(const char* pTexName, D3DXVECTOR3 pos, float fRadius, int nLife, D3DXCOLOR col, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f }, float fGravity = 0.0f, bool bAdd = true, float fDecrease = 0.0f, D3DXVECTOR3* pPosOwner = nullptr, int nPriority = 6, bool bTurn = false);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);	// �Ǎ�
	static void Unload(void);

private:
	int m_nLife;	// ����
	float m_fDecreaseRadius;	// ���a������
	float m_fDecreaseAlpha;	// ���l������
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_relPos;	// ���Έʒu
	D3DXVECTOR3 *m_pPosOwner;	// ������̃|�C���^
	bool m_bAdd;	// ���Z����
	float m_fGravity;
};

#endif
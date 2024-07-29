//*****************************************************
//
// �G�t�F�N�g����[effect.h]
// Author:���R����
//
//*****************************************************

#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "polygon2D.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEffect2D : public CPolygon2D
{
public:
	CEffect2D(int nPriority = 5);	// �R���X�g���N�^
	~CEffect2D();	// �f�X�g���N�^

	static CEffect2D *Create(D3DXVECTOR3 pos,  float fRadius, int nLife, D3DXCOLOR col, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f });
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);	// �Ǎ�
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	int m_nLife;	// ����
	float m_fDecrease;	// ������
};

#endif
//*****************************************************
//
// X�t�@�C���̏���[objectX.cpp]
// Author:���R����
//
//*****************************************************
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object3D.h"
#include "model.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define NUM_TEX	(5)	// �e�N�X�`���̐�

//*****************************************************
// �N���X��`
//*****************************************************
class CObjectX : public CObject3D
{
public:
	CObjectX(int nPriority = 3);	// �R���X�g���N�^
	~CObjectX();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void JustDraw(void);
	void DrawShadow(void);
	static CObjectX *Create(D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f }, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });
	float GetWidth(void) { return 0.0f; }	// �T�C�Y�擾
	void SetPositionOld(D3DXVECTOR3 pos) { m_posOld = pos; }	// �ݒ菈��
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// �ݒ菈��
	D3DXVECTOR3 GetMove(void) { return m_move; }	// �擾����
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	void BindModel(int nIdx);
	int GetIdxModel(void) { return m_nIdxModel; }
	void SetIdxModel(int nIdx) { m_nIdxModel = nIdx; }
	void SetRadius(void);
	float GetRadius(void) { return m_fRadius; }
	void CalcMatrix(void);
	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }	// �擾����
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }	// �擾����
	void SetVtxMax(D3DXVECTOR3 vtx) { m_vtxMax = vtx; }
	void SetVtxMin(D3DXVECTOR3 vtx) { m_vtxMin = vtx; }
	void SetEmissiveCol(D3DXCOLOR col) { m_col = col; m_bChangeCol = true; }
	D3DXCOLOR GetEmissiveCol(void) { return m_col; }
	void ResetColor(void) { m_bChangeCol = false; }
	float GetScale(void) { return m_fScale; }
	void SetScale(float fScale) { m_fScale = fScale; }
	void EnableShadow(bool bEnable) { m_bShadow = bEnable; }
	bool IsDisp(void) { return m_bDisp; }
	void EnableDisp(bool bDisp) { m_bDisp = bDisp; }

private:
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_move;	// �ړ���
	D3DXCOLOR m_col;	// �F
	CModel::Model *m_pModel;
	int m_nIdxModel;	// ���f���̔ԍ�
	float m_fRadius;	// ���f���̔��a
	float m_fScale;	// �X�P�[��
	D3DXVECTOR3 m_vtxMax;	// �ő咸�_
	D3DXVECTOR3 m_vtxMin;	// �ŏ����_
	bool m_bChangeCol;
	bool m_bShadow;
	bool m_bDisp;	// �`�悷�邩�ǂ���
};

#endif
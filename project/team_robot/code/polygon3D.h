//*****************************************************
//
// 3D�|���S���̏���[polygon3D.h]
// Author:���R����
//
//*****************************************************
#ifndef _POLYGON3D_H_
#define _POLYGON3D_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object3D.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPolygon3D : public CObject3D
{
public:
	enum MODE
	{
		MODE_NORMAL = 0,	// �ʏ탂�[�h
		MODE_BILLBOARD,	// �r���{�[�h
		MODE_STRETCHBILLBOARD,	// �X�g���b�`�r���{�[�h
		MODE_MAX
	};

	CPolygon3D(int nPriority = 5);	// �R���X�g���N�^
	~CPolygon3D();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);	// ���_���̐ݒ�
	LPDIRECT3DVERTEXBUFFER9 CreateVtxBuff(int nNumVtx = 4);	// ���_�o�b�t�@�̐���

	// �ϐ��擾�E�ݒ�֐�
	float GetWidth(void) { return m_width; }	// ��
	float GetHeight(void) { return m_heigth; }	// ����
	void SetSize(float width, float height);
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }	// �e�N�X�`���ԍ�
	int GetIdxTexture(void) { return m_nIdxTexture; }
	D3DXCOLOR GetColor(void) { return m_col; }	// �F
	void SetColor(D3DXCOLOR col);
	void SetTex(D3DXVECTOR2 rd, D3DXVECTOR2 lu);	// �e�N�X�`�����W
	void SetFactSB(float fFact) { m_fFactSB = fFact; }	// �X�g���b�`�r���{�[�h�̕␳�l
	void SetMode(MODE mode);	// �`�惂�[�h
	MODE GetMode(void) { return m_mode; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }	// ���_�o�b�t�@
	void SetNormal(D3DXVECTOR3 nor);	// �@���̐ݒ�

	// �ÓI�����o�֐�
	static CPolygon3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });

private:
	// �����o�֐�
	void SetVtxNormal(void);
	void SetVtxStretchBillboard(void);
	void SetMtx(void);
	void SetMtxBillboard(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXCOLOR m_col;	// �F
	float m_width;	// ��
	float m_heigth;	// ����
	float m_fFactSB;	// �X�g���b�`�r���{�[�h�̂Ƃ��̌��₷���̕␳
	int m_nIdxTexture;	// �e�N�X�`���̔ԍ�
	int m_nNumVtx;	// ���_��
	MODE m_mode;	// ���[�h
};

#endif
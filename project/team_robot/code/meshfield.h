//*****************************************************
//
// ���b�V���t�B�|���h�̏���[meshfield.h]
// Author:���R����
//
//*****************************************************

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object3D.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CMeshField : public CObject3D
{
public:
	CMeshField();	// �R���X�g���N�^
	~CMeshField();	// �f�X�g���N�^

	typedef struct
	{
		int nNumIdx;							//�C���f�b�N�X��
		int nNumVtx;							//���_��
	}MeshField;

	static CMeshField *Create(void);
	static CMeshField *GetInstance(void) { return m_pMeshField; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	float GetHeight(D3DXVECTOR3 pos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pNor = nullptr);
	void Edit(void);
	void Load(std::string path);
	void Save(void);
	void SetNormal(void);
	void Dent(D3DXVECTOR3 pos, float fRadius, float fDepth);
	bool RayCheck(D3DXVECTOR3 pos, D3DXVECTOR3 vecDir, D3DXVECTOR3 posOwn);
	void SetCol(D3DXCOLOR col);
	void SetIdxTexture(int nIdxTexture) { m_nIdxTexture = nIdxTexture; }
	void Reset(void);

private:
	int m_nIdxTexture;	// �e�N�X�`���ԍ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	MeshField m_MeshField;	//�\���̂̏��
	D3DXCOLOR m_col;	// �F
	float m_fLengthMesh;	// ���b�V����ӂ̒���
	int m_nDivNumU;
	int m_nDivNumV;
	int m_nDivTex;
	static CMeshField *m_pMeshField;	// ���g�̃|�C���^
};
#endif
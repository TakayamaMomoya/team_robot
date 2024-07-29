//*****************************************************
//
// ���b�V���V�����_�[�̏���[meshcylinder.h]
// Author:���R����
//
//*****************************************************

#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace meshCylinder
{
const float  MESH_RADIUS = 100.0f;	// ���a
const int MESH_U = 16;	// ���̕�����
const int MESH_V = 1;	// �c�̕�����
const int SPLIT_TEX_U = 3;	// ���̃e�N�X�`��������
const int SPLIT_TEX_V = 1;	// �c�̃e�N�X�`��������
const float MESH_HEIGHT = 50.0f;	// ���b�V���̍���
}

//*****************************************************
// �N���X��`
//*****************************************************
class CMeshCylinder : public CObject
{
public:
	CMeshCylinder(int nPriority = 5);	// �R���X�g���N�^
	~CMeshCylinder();	// �f�X�g���N�^

	typedef struct
	{
		D3DXVECTOR3 pos;						//�ʒu
		D3DXVECTOR3 rot;						//����
		D3DXMATRIX mtxWorld;					//���[���h�}�g���b�N�X
		int nNumIdx;							//�C���f�b�N�X��
		int nNumVtx;							//���_��
		float fRadius;	// ���a
		float fHeight;	// ����
		int nMeshU;	// ���̕�����
		int nMeshV;	// �c�̕�����
		int nTexU;	// �e�N�X�`���̉��̕�����
		int nTexV;	// �e�N�X�`���̏c�̕�����
	}MeshCylinder;

	static CMeshCylinder *Create
	(
		int nMeshU = meshCylinder::MESH_U,
		int nMeshV = meshCylinder::MESH_V,
		int nTexU = meshCylinder::SPLIT_TEX_U,
		int nTexV = meshCylinder::SPLIT_TEX_V
	);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void JustDraw(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	void SetPosition(D3DXVECTOR3 pos) { m_meshCylinder.pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_meshCylinder.pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_meshCylinder.rot = rot; }
	D3DXVECTOR3 GetRotation(void) { return m_meshCylinder.rot; }
	int GetNumVtx(void) { return m_meshCylinder.nNumVtx; }
	void SetRadius(float fRadius) { m_meshCylinder.fRadius = fRadius; }
	void SetHeight(float fHeight) { m_meshCylinder.fHeight = fHeight; }
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }
	void SetNumMeshU(int nNumMesh) { m_meshCylinder.nMeshU = nNumMesh; }
	void SetNumMeshV(int nNumMesh) { m_meshCylinder.nMeshV = nNumMesh; }
	MeshCylinder *GetMeshCylinder(void) { return &m_meshCylinder; }
	void SetCol(D3DXCOLOR col);
	D3DXCOLOR GetCol(void) { return m_col; }
	void SetMtx(D3DXMATRIX mtx) { m_meshCylinder.mtxWorld = mtx; }
	void SetVtx(void);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	MeshCylinder m_meshCylinder;	//�\���̂̏��
	D3DXCOLOR m_col;	// �F
	int m_nIdxTexture;	// �e�N�X�`���ԍ�
};
#endif
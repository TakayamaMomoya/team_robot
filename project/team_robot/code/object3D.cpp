//*****************************************************
//
// 3D�I�u�W�F�N�g[object3D.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object3D.h"
#include "renderer.h"

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CObject3D::CObject3D(int nPriority) : CGameObject(nPriority), m_mtxWorld(), m_mtxParent()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CObject3D::~CObject3D()
{

}

//=====================================================
// ������
//=====================================================
HRESULT CObject3D::Init(void)
{
	// �e�}�g���b�N�X�̏�����
	ResetMtxParent();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CObject3D::Uninit(void)
{
	CGameObject::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CObject3D::Update(void)
{

}

//=====================================================
// �`��
//=====================================================
void CObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXVECTOR3 rot = GetRotation();
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXVECTOR3 pos = GetPosition();
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �e�}�g���b�N�X���������킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxParent);

	// ���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=====================================================
// �e�}�g���b�N�X�̃��Z�b�g
//=====================================================
void CObject3D::ResetMtxParent(void)
{
	D3DXMatrixIdentity(&m_mtxParent);
}
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
#include "debugproc.h"

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
	D3DXMatrixIdentity(&m_mtxWorld);

	// �e�}�g���b�N�X�̏�����
	ResetMtxParent();

	D3DXQuaternionIdentity(&m_quat);

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
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;

	// �N�H�[�^�j�I���p�̃I�t�Z�b�g
	D3DXMATRIX mtxUp;
	universal::SetOffSet(&mtxUp, m_mtxWorld, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVECTOR3 vecUp = { mtxUp._41 - m_mtxWorld._41,mtxUp._42 - m_mtxWorld._42 ,mtxUp._43 - m_mtxWorld._43 };

	// ���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXVECTOR3 rot = GetRotation();

	// Y��]
	D3DXQUATERNION quatUp;
	D3DXQuaternionRotationAxis(
		&quatUp, &vecUp, rot.y);
	D3DXQuaternionNormalize(&quatUp, &quatUp); // �N�H�[�^�j�I���̐��K��
	D3DXQuaternionMultiply(&m_quat, &m_quat, &quatUp);	// �N�H�[�^�j�I���̗ݐ�

	// �N�H�[�^�j�I���𔽉f������
	D3DXMatrixRotationQuaternion(
		&m_mtxWorld, &m_quat);	// �N�H�[�^�j�I���ł̉�]

	// X��]
	D3DXMATRIX mtxRight;
	universal::SetOffSet(&mtxRight, m_mtxWorld, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	D3DXVECTOR3 vecRight = { mtxRight._41 - m_mtxWorld._41,mtxRight._42 - m_mtxWorld._42 ,mtxRight._43 - m_mtxWorld._43 };

	D3DXQUATERNION quatRight;
	D3DXQuaternionRotationAxis(
		&quatRight, &vecRight, rot.x);
	D3DXQuaternionNormalize(&quatRight, &quatRight); // �N�H�[�^�j�I���̐��K��
	D3DXQuaternionMultiply(&m_quat, &m_quat, &quatRight);	// �N�H�[�^�j�I���̗ݐ�

	// �N�H�[�^�j�I���𔽉f������
	D3DXMatrixRotationQuaternion(
		&m_mtxWorld, &m_quat);	// �N�H�[�^�j�I���ł̉�]

	// Z����]
	D3DXVECTOR3 vecFront = { m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33 };
	D3DXQUATERNION quatFront;
	D3DXQuaternionRotationAxis(
		&quatFront, &vecFront, rot.z);	// �N�H�[�^�j�I�����쐬
	D3DXQuaternionNormalize(&quatFront, &quatFront); // �N�H�[�^�j�I���̐��K��
	D3DXQuaternionMultiply(&m_quat, &m_quat, &quatFront);	// �N�H�[�^�j�I���̗ݐ�

	// �N�H�[�^�j�I���𔽉f������
	D3DXMatrixRotationQuaternion(
		&m_mtxWorld, &m_quat);	// �N�H�[�^�j�I���ł̉�]

	// �ʒu�𔽉f
	D3DXVECTOR3 pos = GetPosition();
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �e�̃}�g���b�N�X�Ƃ������킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxParent);

	// ���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=====================================================
// �e�}�g���b�N�X�̃��Z�b�g
//=====================================================
void CObject3D::ResetMtxParent(void)
{
	D3DXMatrixIdentity(&m_mtxParent);
}

//=====================================================
// �O���x�N�g���̎擾
//=====================================================
D3DXVECTOR3 CObject3D::GetForward(void)
{
	return D3DXVECTOR3( m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33 );
}

//=====================================================
// ����x�N�g���̎擾
//=====================================================
D3DXVECTOR3 CObject3D::GetUp(void)
{
	return D3DXVECTOR3( m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23 );
}

//=====================================================
// �E���x�N�g���̎擾
//=====================================================
D3DXVECTOR3 CObject3D::GetRight(void)
{
	return D3DXVECTOR3( m_mtxWorld._11, m_mtxWorld._12, m_mtxWorld._13 );
}
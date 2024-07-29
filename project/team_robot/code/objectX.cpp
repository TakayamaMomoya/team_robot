//*****************************************************
//
// X�t�@�C���̏���[objectX.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "texture.h"
#include "model.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* INITIAL_MODEL = "data\\MODEL\\block\\bill00.x";	// �������f��
}

//====================================================
// �R���X�g���N�^
//====================================================
CObjectX::CObjectX(int nPriority) : CObject3D(nPriority)
{
	m_col = { 0.0f,0.0f,0.0f,0.0f };
	m_nIdxModel = -1;
	m_fRadius = 0.0f;
	m_fScale = 0.0f;
	m_bChangeCol = false;
	m_bShadow = false;
	m_pModel = nullptr;
	m_bDisp = true;
}

//====================================================
// �f�X�g���N�^
//====================================================
CObjectX::~CObjectX()
{

}

//====================================================
//����������
//====================================================
HRESULT CObjectX::Init(void)
{
	m_fScale = 1.0f;

	if (m_pModel == nullptr)
	{
		int nIdx = CModel::Load((char*)INITIAL_MODEL);

		// ���f���Ǎ�
		BindModel(nIdx);
	}

	CObject3D::Init();

	return S_OK;
}

//====================================================
//�I������
//====================================================
void CObjectX::Uninit(void)
{
	// ���g�̔j��
	CObject3D::Uninit();
}

//====================================================
//�X�V����
//====================================================
void CObjectX::Update(void)
{

}

//====================================================
// �`�揈��
//====================================================
void CObjectX::Draw(void)
{
	if (!m_bDisp)
		return;

	if (m_pModel != nullptr)
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

		if (m_bShadow)
		{
			D3DXMATRIX *pMtx = &GetMatrix();
			D3DXMATRIX mtxShadow;
			D3DLIGHT9 light;
			D3DXVECTOR4 posLight;
			D3DXVECTOR3 normal;
			D3DXPLANE plane;

			// ���C�g�̈ʒu�ݒ�
			pDevice->GetLight(2, &light);
			posLight = { -light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f };

			// ���ʏ��̐���
			normal = { 0.0f,1.0f,0.0f };
			D3DXPlaneFromPointNormal(&plane, &D3DXVECTOR3(0.0f, 0.5f, 0.0f), &normal);

			// �V���h�E�}�g���b�N�X������
			D3DXMatrixIdentity(&mtxShadow);

			// �V���h�E�}�g���b�N�X�̍쐬
			D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
			D3DXMatrixMultiply(&mtxShadow, pMtx, &mtxShadow);

			// �V���h�E�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

			DrawShadow();
		}

		// �}�g���b�N�X�̐ݒ�
		CalcMatrix();

		D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
		D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
		LPDIRECT3DTEXTURE9 pTexture;

		// ���݂̃}�e���A���擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_pModel->dwNumMat; nCntMat++)
		{
			// �}�e���A���̕ۑ�
			matDef = pMat[nCntMat].MatD3D;

			if (m_bChangeCol)
			{
				// �F�̐ݒ�
				pMat[nCntMat].MatD3D.Diffuse = m_col;
			}

			// �}�e���A���ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̎擾
			pTexture = CTexture::GetInstance()->GetAddress(m_pModel->pIdxTexture[nCntMat]);

			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, pTexture);

			// ���f���i�p�[�c�j�`��
			m_pModel->pMesh->DrawSubset(nCntMat);

			// �F��߂�
			pMat[nCntMat].MatD3D = matDef;
		}

		// �}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//====================================================
// �e�p�̕`�揈��
//====================================================
void CObjectX::DrawShadow(void)
{
	if (!m_bDisp)
		return;

	if (m_pModel == nullptr)
	{
		return;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	LPDIRECT3DTEXTURE9 pTexture;

	// ���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModel->dwNumMat; nCntMat++)
	{
		// �}�e���A���̕ۑ�
		matDef = pMat[nCntMat].MatD3D;

		// �F�̐ݒ�
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//�}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̎擾
		pTexture = CTexture::GetInstance()->GetAddress(m_pModel->pIdxTexture[nCntMat]);

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, pTexture);

		//���f���i�p�[�c�j�`��
		m_pModel->pMesh->DrawSubset(nCntMat);

		// �F��߂�
		pMat[nCntMat].MatD3D = matDef;

		// �}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=====================================================
// �`��̂ݍs��
//=====================================================
void CObjectX::JustDraw(void)
{
	if (!m_bDisp)
		return;

	if (m_pModel != nullptr)
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

		// ���[���h�}�g���b�N�X�ݒ�
		D3DXMATRIX mtxWorld = CObject3D::GetMatrix();
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
		D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
		LPDIRECT3DTEXTURE9 pTexture;

		// ���݂̃}�e���A���擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_pModel->dwNumMat; nCntMat++)
		{
			// �}�e���A���̕ۑ�
			matDef = pMat[nCntMat].MatD3D;

			if (m_bChangeCol)
			{
				// �F�̐ݒ�
				pMat[nCntMat].MatD3D.Diffuse = m_col;
			}

			// �}�e���A���ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̎擾
			pTexture = CTexture::GetInstance()->GetAddress(m_pModel->pIdxTexture[nCntMat]);

			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, pTexture);

			// ���f���i�p�[�c�j�`��
			m_pModel->pMesh->DrawSubset(nCntMat);

			// �F��߂�
			pMat[nCntMat].MatD3D = matDef;
		}

		// �}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=====================================================
// �}�g���b�N�X�ݒ菈��
//=====================================================
void CObjectX::CalcMatrix(void)
{
	CObject3D::Draw();
}

//=====================================================
//��������
//=====================================================
CObjectX *CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObjectX *pObjectX = nullptr;

	if (pObjectX == nullptr)
	{
		// �C���X�^���X����
		pObjectX = new CObjectX;

		pObjectX->SetPosition(pos);
		pObjectX->SetRotation(rot);


		// ����������
		pObjectX->Init();
	}

	return pObjectX;
}

//=====================================================
// ���f���̍Œ����_�擾
//=====================================================
void CObjectX::SetRadius(void)
{
	//�v�Z�p�ϐ�
	int nNumVtx;			//���_��
	DWORD dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^
	float fLength = 0.0f;
	D3DXVECTOR3 vtxMin = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 vtxMax = { 0.0f,0.0f,0.0f };

	//���_���̎擾
	nNumVtx = m_pModel->pMesh->GetNumVertices();

	//�t�H�[�}�b�g�T�C�Y����
	dwSizeFVF = D3DXGetFVFVertexSize(m_pModel->pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_pModel->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x > vtxMax.x)
		{//X�̍ő�l
			vtxMax.x = vtx.x;
		}
		else if (vtx.x < vtxMin.x)
		{//X�̍ŏ��l
			vtxMin.x = vtx.x;
		}

		if (vtx.z > vtxMax.z)
		{//Z�̍ő�l
			vtxMax.z = vtx.z;
		}
		else if (vtx.z < vtxMin.z)
		{//Z�̍ŏ��l
			vtxMin.z = vtx.z;
		}

		if (vtx.y > vtxMax.y)
		{//Y�̍ő�l
			vtxMax.y = vtx.y;
		}
		else if (vtx.y < vtxMin.y)
		{//Y�̍ŏ��l
			vtxMin.y = vtx.y;
		}

		if (D3DXVec3Length(&vtx) > fLength)
		{
			fLength = D3DXVec3Length(&vtx);
		}

		pVtxBuff += dwSizeFVF;
	}

	//�ő�E�ŏ����_�ݒ�
	m_vtxMax = vtxMax;
	m_vtxMin = vtxMin;

	//���_�o�b�t�@�̃A�����b�N
	m_pModel->pMesh->UnlockVertexBuffer();

	// ��������
	m_fRadius = fLength;
}

//=====================================================
// ���f���̊��蓖��
//=====================================================
void CObjectX::BindModel(int nIdx)
{
	// ���f�����蓖��
	m_pModel = CModel::GetModel(nIdx);

	// �ԍ��L��
	m_nIdxModel = nIdx;

	// ���a�ݒ�
	SetRadius();
}
//*****************************************************
//
// ���b�V���V�����_�[�̏���[meshcylinder.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "debugproc.h"
#include "universal.h"
#include "texture.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define TEX_PATH		"data\\TEXTURE\\EFFECT\\boost.png"				// �e�N�X�`���t�@�C����

//=====================================================
// �R���X�g���N�^
//=====================================================
CMeshCylinder::CMeshCylinder(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_meshCylinder, sizeof(m_meshCylinder));
	m_pIdxBuff = nullptr;
	m_pVtxBuff = nullptr;
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_nIdxTexture = -1;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMeshCylinder::~CMeshCylinder()
{

}

//=====================================================
// ��������
//=====================================================
CMeshCylinder *CMeshCylinder::Create(int nMeshU,int nMeshV,int nTexU,int nTexV)
{
	CMeshCylinder *pMeshCylinder = nullptr;

	if (pMeshCylinder == nullptr)
	{
		pMeshCylinder = new CMeshCylinder;

		if (pMeshCylinder != nullptr)
		{
			pMeshCylinder->m_meshCylinder.nMeshU = nMeshU;
			pMeshCylinder->m_meshCylinder.nMeshV = nMeshV;
			pMeshCylinder->m_meshCylinder.nTexU = nTexU;
			pMeshCylinder->m_meshCylinder.nTexV = nTexV;

			pMeshCylinder->Init();
		}
	}

	return pMeshCylinder;
}

//=====================================================
// ����������
//=====================================================
HRESULT CMeshCylinder::Init(void)
{
	m_meshCylinder.fRadius = meshCylinder::MESH_RADIUS;
	m_meshCylinder.fHeight = meshCylinder::MESH_HEIGHT;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	
	m_meshCylinder.nTexU = meshCylinder::SPLIT_TEX_U;
	m_meshCylinder.nTexV = meshCylinder::SPLIT_TEX_V;
	m_meshCylinder.fRadius = m_meshCylinder.fRadius;
	int nMeshU = m_meshCylinder.nMeshU;
	int nMeshV = m_meshCylinder.nMeshV;
	int nTexU = m_meshCylinder.nTexU;
	int nTexV = m_meshCylinder.nTexV;
	float fRadius = m_meshCylinder.fRadius;
	float fHeight = m_meshCylinder.fHeight;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nMeshU + 1) * (nMeshV + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((nMeshU + 1) * (nMeshV + 1) + ((nMeshV + 1) - 2) * (nMeshU + 1) + (((nMeshV + 1) - 2) * 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// �e�N�X�`���̓Ǎ�
	CTexture *pTexture = CTexture::GetInstance();

	if (pTexture != nullptr)
	{
		int nIdx = pTexture->Regist(TEX_PATH);
		SetIdxTexture(nIdx);
	}

	//���_���̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	WORD *pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//�ő咸�_���v�Z
	m_meshCylinder.nNumVtx = (nMeshU + 1) * (nMeshV + 1);

	//�v�Z�p�ϐ�
	float fRot;
	D3DXVECTOR3 VecRot;

	//���_���==================================================================================
	for (int nCountV = 0; nCountV < nMeshV + 1; nCountV++)
	{//���_���W�̐ݒ�
		for (int nCountU = 0; nCountU < nMeshU + 1; nCountU++)
		{
			//�p�x�Z�o
			fRot = nCountU * (D3DX_PI / nMeshU) * 2;

			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x = (float)sin(fRot) * fRadius;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.y = (nMeshV - nCountV) * fHeight;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z = cosf(fRot) * fRadius;

			//�e�N�X�`�����W
			pVtx[nCountV * (nMeshU + 1) + nCountU].tex = D3DXVECTOR2
			(
				((float)nTexU / nMeshU) * nCountU,
				((float)nTexV / nMeshV) * nCountV
			);

			VecRot = D3DXVECTOR3
			(
				pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x,
				0.0f,
				pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z
			);

			//�x�N�g�����K��
			D3DXVec3Normalize(&VecRot, &VecRot);

			//�@���x�N�g���̐ݒ�
			pVtx[nCountV * (nMeshU + 1) + nCountU].nor = VecRot;
		}
	}

	for (int nCnt = 0; nCnt < m_meshCylinder.nNumVtx; nCnt++)
	{
		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//�C���f�b�N�X==================================================================================
	m_meshCylinder.nNumIdx = (nMeshU + 1) * (nMeshV + 1) + ((nMeshV + 1) - 2) * (nMeshU + 1) + (((nMeshV + 1) - 2) * 2);

	for (int nCount = 0; nCount < m_meshCylinder.nNumIdx / 2; nCount++)
	{//�C���f�b�N�X����
		if (
			nCount % ((nMeshU + 1) + (nMeshU + 2) * (nCount / (nMeshU + 2))) == 0
			&& nCount != 0
			)
		{
			pIdx[nCount * 2 + 1] = ((nMeshU + 1) * 2) + (nMeshU + 1) * (nCount / (nMeshU + 2));

			pIdx[nCount * 2] = nMeshU + (nMeshU + 1) * (nCount / (nMeshU + 2));
		}
		else
		{
			pIdx[nCount * 2 + 1] = nCount - (nCount / (nMeshU + 2));

			pIdx[nCount * 2] = (nCount - (nCount / (nMeshU + 2))) + (nMeshU + 1);
		}
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CMeshCylinder::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{//���_�o�b�t�@�|�C���^�̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pIdxBuff != NULL)
	{//�C���f�b�N�X�o�b�t�@�|�C���^�̔j��
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CMeshCylinder::Update(void)
{

}

//=====================================================
// �X�V����
//=====================================================
void CMeshCylinder::SetVtx(void)
{
	int nMeshU = m_meshCylinder.nMeshU;
	int nMeshV = m_meshCylinder.nMeshV;
	int nTexU = m_meshCylinder.nTexU;
	int nTexV = m_meshCylinder.nTexV;
	float fRadius = m_meshCylinder.fRadius;
	float fHeight = m_meshCylinder.fHeight;

	//���_���̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCountV = 0; nCountV < nMeshV + 1; nCountV++)
	{//���_���W�̐ݒ�
		for (int nCountU = 0; nCountU < nMeshU + 1; nCountU++)
		{
			//�p�x�Z�o
			float fRot = nCountU * (D3DX_PI / nMeshU) * 2;

			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x = (float)sin(fRot) * fRadius;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.y = (nMeshV - nCountV) * fHeight;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z = cosf(fRot) * fRadius;

			//�e�N�X�`�����W
			pVtx[nCountV * (nMeshU + 1) + nCountU].tex = D3DXVECTOR2
			(
				((float)nTexU / nMeshU) * nCountU,
				((float)nTexV / nMeshV) * nCountV
			);

			D3DXVECTOR3 VecRot = D3DXVECTOR3
			(
				pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x,
				0.0f,
				pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z
			);

			//�x�N�g�����K��
			D3DXVec3Normalize(&VecRot, &VecRot);

			//�@���x�N�g���̐ݒ�
			pVtx[nCountV * (nMeshU + 1) + nCountU].nor = VecRot;
		}
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=====================================================
// �F�ݒ�
//=====================================================
void CMeshCylinder::SetCol(D3DXCOLOR col)
{
	if (m_pVtxBuff == nullptr)
	{
		return;
	}

	m_col = col;

	//���_���̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_meshCylinder.nNumVtx; nCnt++)
	{
		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = m_col;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=====================================================
// �`�揈��
//=====================================================
void CMeshCylinder::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;

	// �J�����O�𖳌���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_meshCylinder.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_meshCylinder.rot.y, m_meshCylinder.rot.x, m_meshCylinder.rot.z);
	D3DXMatrixMultiply(&m_meshCylinder.mtxWorld, &m_meshCylinder.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		m_meshCylinder.pos.x, m_meshCylinder.pos.y, m_meshCylinder.pos.z);
	D3DXMatrixMultiply(&m_meshCylinder.mtxWorld, &m_meshCylinder.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_meshCylinder.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���擾
	CTexture *pTextureManager = CTexture::GetInstance();

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	if (pTextureManager != nullptr)
	{
		pTexture = pTextureManager->GetAddress(m_nIdxTexture);
	}

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, pTexture);

	//�|���S���`��
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_meshCylinder.nNumVtx,								//���_��
		0,													//�ŏ��̃C���f�b�N�X
		m_meshCylinder.nNumIdx - 2							//�|���S����
	);

	// �J�����O��L����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=====================================================
// �`��̂ݍs��
//=====================================================
void CMeshCylinder::JustDraw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �J�����O�𖳌���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_meshCylinder.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���擾
	CTexture *pTextureManager = CTexture::GetInstance();

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	if (pTextureManager != nullptr)
	{
		pTexture = pTextureManager->GetAddress(m_nIdxTexture);
	}

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, pTexture);

	//�|���S���`��
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_meshCylinder.nNumVtx,								//���_��
		0,													//�ŏ��̃C���f�b�N�X
		m_meshCylinder.nNumIdx - 2							//�|���S����
	);

	// �J�����O��L����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
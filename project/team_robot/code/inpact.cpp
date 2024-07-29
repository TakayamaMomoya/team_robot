//*****************************************************
//
// �Ռ��g�̏���[inpact.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "inpact.h"
#include "manager.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int MESH_U = 32;	// ���̕�����
const int MESH_V = 1;	// �c�̕�����
const float MESH_HEIGHT = 50.0f;	// ���b�V���̍���
const float INITIAL_EXPAND = 15.0f;	// �����̖c��ޑ��x
const char* TEX_PATH = "data\\TEXTURE\\EFFECT\\inpact.png";	// �e�N�X�`���p�X
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CInpact::CInpact(int nPriority) : CMeshCylinder(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CInpact::~CInpact()
{

}

//=====================================================
// ��������
//=====================================================
CInpact *CInpact::Create(float nLife, D3DXMATRIX *pMtx)
{
	CInpact *pInpact = nullptr;

	if (pInpact == nullptr)
	{
		pInpact = new CInpact;

		if (pInpact != nullptr)
		{
			if (pMtx != nullptr)
			{
				pInpact->SetMtx(*pMtx);
			}

			pInpact->m_info.fLife = nLife;
			pInpact->m_info.fLifeInitial = nLife;

			pInpact->Init();
		}
	}

	return pInpact;
}

//=====================================================
// ����������
//=====================================================
HRESULT CInpact::Init(void)
{
	// �������ݒ�
	SetNumMeshU(MESH_U);
	SetNumMeshV(MESH_V);

	// �p���N���X�̏�����
	CMeshCylinder::Init();

	m_info.fRadiusDiff = 40.0f;
	m_info.fSpeedExpand = INITIAL_EXPAND;

	// ���_�ʒu�ݒ�
	SetVtx();

	// �e�N�X�`���̓Ǎ�
	CTexture *pTexture = CTexture::GetInstance();

	if (pTexture != nullptr)
	{
		int nIdx = pTexture->Regist(TEX_PATH);
		SetIdxTexture(nIdx);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CInpact::Uninit(void)
{
	CMeshCylinder::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CInpact::Update(void)
{
	CMeshCylinder::Update();

	// �̗͊Ǘ�
	ManageLife();

	// ���_�ݒ�
	SetVtx();
}

//=====================================================
// ���_�ʒu�̐ݒ�
//=====================================================
void CInpact::SetVtx(void)
{
	// ���擾
	MeshCylinder *pMesh = GetMeshCylinder();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	if (pMesh == nullptr || pVtxBuff == nullptr)
	{
		return;
	}

	int nMeshU = pMesh->nMeshU;
	int nMeshV = pMesh->nMeshV;
	float fRadius = pMesh->fRadius;
	float fHeight = pMesh->fHeight;

	//���_���̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�v�Z�p�ϐ�
	float fRot;
	D3DXVECTOR3 VecRot;

	//���_���==================================================================================
	for (int nCountV = 0; nCountV < nMeshV + 1; nCountV++)
	{//���_���W�̐ݒ�
		// ���a�̐ݒ�
		if (nCountV == 0)
		{
			fRadius -= m_info.fRadiusDiff;
		}
		else
		{
			fRadius += m_info.fRadiusDiff;
		}

		for (int nCountU = 0; nCountU < nMeshU + 1; nCountU++)
		{
			//�p�x�Z�o
			fRot = nCountU * (D3DX_PI / nMeshU) * 2;

			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x = (float)sin(fRot) * fRadius;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.y = (nMeshV - nCountV) * fHeight;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z = cosf(fRot) * fRadius;

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

	//���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();
}

//=====================================================
// ���C�t�̊Ǘ�
//=====================================================
void CInpact::ManageLife(void)
{
	// ���a�擾
	MeshCylinder *pMesh = GetMeshCylinder();

	if (pMesh == nullptr)
	{
		return;
	}

	float fRadius = pMesh->fRadius;

	// ���a�����Z����
	fRadius += m_info.fSpeedExpand;
	SetRadius(fRadius);

	// ���������炷
	float fDeltaTime = CManager::GetDeltaTime();

	m_info.fLife -= fDeltaTime;

	// �F�̐ݒ�
	float fRate = m_info.fLife / m_info.fLifeInitial;

	D3DXCOLOR col = GetCol();

	col.a = fRate;

	SetCol(col);

	if (m_info.fLife <= 0.0f)
	{
		Uninit();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CInpact::Draw(void)
{
	CMeshCylinder::JustDraw();
}
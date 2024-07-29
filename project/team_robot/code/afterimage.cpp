//*****************************************************
//
// �c���̏���[alterimage.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "afterimage.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CAfterImage::m_nNumAll = 0;	// ����

//=====================================================
// �R���X�g���N�^
//=====================================================
CAfterImage::CAfterImage(int nPriority)
{
	m_nLife = 0;
	m_fDecAlpha = 0.0f;

	m_nNumAll++;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CAfterImage::~CAfterImage()
{
	m_nNumAll--;
}

//=====================================================
// ��������
//=====================================================
CAfterImage *CAfterImage::Create(D3DXMATRIX mtx, int nIdxModel, D3DXCOLOR col, int nLife)
{
	CAfterImage *pAlterImage = nullptr;

	if (pAlterImage == nullptr)
	{// �C���X�^���X����
		pAlterImage = new CAfterImage;

		if (pAlterImage != nullptr)
		{
			// �}�g���b�N�X�K�p
			pAlterImage->SetMatrix(mtx);

			// ���f���ݒ�
			pAlterImage->SetIdxModel(nIdxModel);
			pAlterImage->BindModel(nIdxModel);

			// �p�����[�^�[�ݒ�
			pAlterImage->m_nLife = nLife;

			// ����������
			pAlterImage->Init();
			
			pAlterImage->SetEmissiveCol(col);
		}
	}

	return pAlterImage;
}

//=====================================================
// ����������
//=====================================================
HRESULT CAfterImage::Init(void)
{
	// �p���N���X�̏�����
	CObjectX::Init();

	m_fDecAlpha = 1.0f / m_nLife;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CAfterImage::Uninit(void)
{
	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CAfterImage::Update(void)
{
	// �p���N���X�̍X�V
	CObjectX::Update();

	D3DXCOLOR col = GetEmissiveCol();

	m_nLife--;

	col.a -= m_fDecAlpha;

	SetEmissiveCol(col);

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CAfterImage::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �p���N���X�̕`��
	CObjectX::JustDraw();

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
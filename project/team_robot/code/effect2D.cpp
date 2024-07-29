//*****************************************************
//
// 2D�G�t�F�N�g����[effect2D.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "effect2D.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define SPEED_MOVE	(7.0f)	// �ړ����x

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
LPDIRECT3DTEXTURE9 CEffect2D::m_pTexture = nullptr;	// �e�N�X�`���̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CEffect2D::CEffect2D(int nPriority) : CPolygon2D(nPriority)
{
	m_nLife = 0;
	m_fDecrease = 0.0f;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEffect2D::~CEffect2D()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEffect2D::Init(void)
{
	// �p���N���X�̏�����
	CPolygon2D::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEffect2D::Uninit(void)
{
	// �p���N���X�̏I��
	CPolygon2D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEffect2D::Update(void)
{
	// ��������
	m_nLife--;

	// �T�C�Y�k��
	SetSize(GetSize().x - m_fDecrease, GetSize().y - m_fDecrease);

	// �p���N���X�̍X�V
	CPolygon2D::Update();

	if (m_nLife < 0)
	{// �����̍폜
		Uninit();
	}

	// ���_���W�ݒ�
	SetVtx();
}

//=====================================================
// �`�揈��
//=====================================================
void CEffect2D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �p���N���X�̕`��
	CPolygon2D::Draw();

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=====================================================
// ��������
//=====================================================
CEffect2D *CEffect2D::Create(D3DXVECTOR3 pos, float fRadius, int nLife, D3DXCOLOR col, D3DXVECTOR3 move)
{
	CEffect2D *pEffect2D = nullptr;

	if (pEffect2D == nullptr)
	{// �C���X�^���X����
		pEffect2D = new CEffect2D;
	}
	
	// ����������
	pEffect2D->Init();

	pEffect2D->SetPosition(pos);
	pEffect2D->SetSize(fRadius, fRadius);

	pEffect2D->m_nLife = nLife;

	pEffect2D->m_fDecrease = fRadius / nLife;

	pEffect2D->SetMove(move);

	pEffect2D->SetCol(col);

	return pEffect2D;
}

//=====================================================
// �Ǎ�����
//=====================================================
HRESULT CEffect2D::Load(void)
{
	if (m_pTexture == nullptr)
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\EFFECT\\effect000.png",
			&m_pTexture
		);
	}

	return S_OK;
}

//=====================================================
// �e�N�X�`���j��
//=====================================================
void CEffect2D::Unload(void)
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}
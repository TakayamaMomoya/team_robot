//*****************************************************
//
// �RD�G�t�F�N�g����[effect3D.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "effect3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "slow.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define SPEED_MOVE	(7.0f)	// �ړ����x

//=====================================================
// �R���X�g���N�^
//=====================================================
CEffect3D::CEffect3D(int nPriority) : CPolygon3D(nPriority)
{
	m_nLife = 0;
	m_fDecreaseRadius = 0.0f;
	m_fDecreaseAlpha = 0.0f;
	m_move = { 0.0f,0.0f,0.0f };
	m_relPos = { 0.0f,0.0f,0.0f };
	m_bAdd = true;
	m_fGravity = 0.0f;
	m_pPosOwner = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEffect3D::~CEffect3D()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEffect3D::Init(void)
{
	// �p���N���X�̏�����
	CPolygon3D::Init();

	SetMode(CPolygon3D::MODE_BILLBOARD);
	EnableZtest(true);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEffect3D::Uninit(void)
{
	// �p���N���X�̏I��
	CPolygon3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEffect3D::Update(void)
{
	// �p���N���X�̍X�V
	CPolygon3D::Update();

	MODE mode = GetMode();

	if (mode == MODE_STRETCHBILLBOARD)
	{
		D3DXVECTOR3 rot = universal::VecToRot(m_move);

		rot.x += 1.57f;
		rot.y += 3.14f;

		SetRotation(rot);
	}

	// ��������
	m_nLife--;

	float fWidth = GetWidth();
	float fHeight = GetHeight();

	fWidth -= m_fDecreaseRadius;
	fHeight -= m_fDecreaseRadius;

	// �T�C�Y�k��
	SetSize(fWidth, fHeight);

	if (fWidth < 0.0f)
	{// �傫���̕␳
		SetSize(0.0f, 0.0f);
	}

	// �d�͉��Z
	m_move.y -= m_fGravity;

	// �F����
	SetColor(D3DXCOLOR(GetColor().r, GetColor().g, GetColor().b, GetColor().a - m_fDecreaseAlpha));

	if (m_pPosOwner != nullptr)
	{
		// ���Έʒu�X�V
		m_relPos += m_move;

		// �ʒu�X�V
		SetPosition(*m_pPosOwner + m_relPos + m_move);
	}
	else
	{
		D3DXVECTOR3 pos = GetPosition();

		CSlow *pSlow = CSlow::GetInstance();

		if (pSlow != nullptr)
		{
			float fScale = pSlow->GetScale();

			// �ʒu�̍X�V
			pos += m_move * fScale;
		}
		else
		{
			// �ʒu�̍X�V
			pos += m_move;
		}

		// �ʒu�X�V
		SetPosition(pos);
	}

	if (m_nLife < 0)
	{// �����̍폜
		Uninit();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CEffect3D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (m_bAdd)
	{
		//���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// �A���t�@�e�X�g�̖�����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

	MODE mode = GetMode();

	if (mode == MODE_STRETCHBILLBOARD)
	{
		SetVtx();
	}

	CPolygon3D::Draw();

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (m_bAdd)
	{
		//���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//=====================================================
// ��������
//=====================================================
CEffect3D *CEffect3D::Create(D3DXVECTOR3 pos, float fRadius, int nLife, D3DXCOLOR col, D3DXVECTOR3 move, float fGravity, bool bAdd,float fDecrease, D3DXVECTOR3 *pPosOwner, int nPriority, bool bTurn)
{
	CEffect3D *pEffect3D = nullptr;

	if (pEffect3D == nullptr)
	{// �C���X�^���X����
		pEffect3D = new CEffect3D(nPriority);

		if (pEffect3D != nullptr)
		{
			// ����������
			pEffect3D->Init();

			pEffect3D->SetPosition(pos);
			pEffect3D->SetSize(fRadius, fRadius);

			pEffect3D->SetColor(col);

			// �e�N�X�`���̓Ǎ�
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\EFFECT\\effect000.png");
			pEffect3D->SetIdxTexture(nIdx);

			pEffect3D->m_nLife = nLife;

			pEffect3D->m_move = move;

			pEffect3D->m_fDecreaseRadius = fDecrease;

			pEffect3D->m_fDecreaseAlpha = 1.0f / nLife;

			pEffect3D->m_fGravity = fGravity;

			pEffect3D->m_bAdd = bAdd;
			
			pEffect3D->m_pPosOwner = pPosOwner;

			if (pPosOwner != nullptr)
			{// �ʒu�̃��Z�b�g
				pEffect3D->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				if (bTurn)
				{
					pEffect3D->m_relPos = move * (float)nLife;

					pEffect3D->m_move *= -1;
				}
			}

			pEffect3D->SetVtx();
		}
	}
	
	return pEffect3D;
}

//=====================================================
// ��������(�I�[�o�[���[�h)
//=====================================================
CEffect3D* CEffect3D::Create(const char* pTexName, D3DXVECTOR3 pos, float fRadius, int nLife, D3DXCOLOR col, D3DXVECTOR3 move, float fGravity, bool bAdd, float fDecrease, D3DXVECTOR3* pPosOwner, int nPriority, bool bTurn)
{
	CEffect3D* pEffect3D = nullptr;

	if (pEffect3D == nullptr)
	{// �C���X�^���X����
		pEffect3D = new CEffect3D(nPriority);

		if (pEffect3D != nullptr)
		{
			// ����������
			pEffect3D->Init();

			pEffect3D->SetPosition(pos);
			pEffect3D->SetSize(fRadius, fRadius);
			pEffect3D->SetVtx();

			pEffect3D->SetColor(col);

			// �e�N�X�`���̓Ǎ�
			int nIdx = CTexture::GetInstance()->Regist(pTexName);
			pEffect3D->SetIdxTexture(nIdx);

			pEffect3D->m_nLife = nLife;

			pEffect3D->m_move = move;

			pEffect3D->m_fDecreaseRadius = fDecrease;

			pEffect3D->m_fDecreaseAlpha = 1.0f / nLife;

			pEffect3D->m_fGravity = fGravity;

			pEffect3D->m_bAdd = bAdd;

			pEffect3D->m_pPosOwner = pPosOwner;

			if (pPosOwner != nullptr)
			{// �ʒu�̃��Z�b�g
				pEffect3D->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				if (bTurn)
				{
					pEffect3D->m_relPos = move * (float)nLife;

					pEffect3D->m_move *= -1;
				}
			}
		}
	}

	return pEffect3D;
}

//=====================================================
// �Ǎ�����
//=====================================================
HRESULT CEffect3D::Load(void)
{
	return S_OK;
}

//=====================================================
// �e�N�X�`���j��
//=====================================================
void CEffect3D::Unload(void)
{

}
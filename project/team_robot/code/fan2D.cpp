//*****************************************************
//
// 2D�~�̏���[fan2D.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "fan2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "universal.h"
#include "debugproc.h"
#include "UIManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{

}

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CFan2D::CFan2D(int nPriority) : CFan(nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CFan2D::~CFan2D()
{

}

//=====================================================
// ��������
//=====================================================
CFan2D *CFan2D::Create(int nPriority, int nNumVtx)
{
	CFan2D *pFan2D = nullptr;

	// �C���X�^���X����
	pFan2D = new CFan2D(nPriority);

	if (pFan2D != nullptr)
	{
		pFan2D->SetNumVtx(nNumVtx);

		// ����������
		pFan2D->Init();
	}

	return pFan2D;
}

//=====================================================
// ������
//=====================================================
HRESULT CFan2D::Init(void)
{
	CFan::Init();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr)
	{
		LPDIRECT3DVERTEXBUFFER9 *pVtxBuff = GetVtxBuff();
		int nNumVtx = GetNumVtx();

		if (*pVtxBuff == nullptr)
		{
			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (nNumVtx + 2),
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				pVtxBuff,
				nullptr);
		}
	}

	// ���_�ݒ�
	SetVtx();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CFan2D::Uninit(void)
{
	CFan::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CFan2D::Update(void)
{
	CFan::Update();
}

//=====================================================
// ���_�ʒu�ݒ�
//=====================================================
void CFan2D::SetVtx(void)
{
	// ���_���̃|�C���^
	VERTEX_2D *pVtx;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();

	if (pVtxBuff != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		
		// ���S�̒��_�̐ݒ�
		pVtx[0].pos = pos;
		pVtx[0].tex = D3DXVECTOR2{ 0.5f,0.5f };

		int nNumVtx = GetNumVtx();
		float fAngleMax = GetAngleMax();
		float fRadius = GetRadius();
		float fRateAngle = GetRateAngle();
		D3DXVECTOR3 rot = GetRotation();

		for (int i = 1; i < nNumVtx + 2; i++)
		{// �~���̒��_�̐ݒ�
			// ���_�̂���p�x���v�Z
			float fAngle = (fAngleMax * fRateAngle) * ((float)(i - 1) / (float)nNumVtx) + rot.z;

			universal::LimitRot(&fAngle);

			pVtx[i].pos =
			{// ���_�ʒu�ݒ�
				pos.x + sinf(fAngle) * fRadius,
				pos.y - cosf(fAngle) * fRadius,
				0.0f,
			};

			D3DXVECTOR2 tex =
			{// �e�N�X�`�����W�̒��S����Ƃ���̂�0.5f���炷
				0.5f + sinf(fAngle) * 0.5f,
				0.5f - cosf(fAngle) * 0.5f,
			};

			pVtx[i].tex = tex;
		}

		// ���_�o�b�t�@�̃A�����b�N
		pVtxBuff->Unlock();
	}
}

//=====================================================
// �`��
//=====================================================
void CFan2D::Draw(void)
{
	bool bDisp = true;

	CUIManager *pUIManager = CUIManager::GetInstance();

	// ��\�����̃`�F�b�N
	if (pUIManager != nullptr)
		bDisp = pUIManager->IsDisp();

	if (!bDisp)
		return;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// ���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();

	if (pDevice != nullptr && pVtxBuff != nullptr)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		CFan::Draw();
	}
}
//*****************************************************
//
// 3D�~�̏���[fan3D.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "fan3D.h"
#include "renderer.h"
#include "texture.h"
#include "universal.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const float SWING_WIDTH = 10.0f;  // sin�J�[�u�̐U�ꕝ
}

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CFan3D::CFan3D(int nPriority) : CFan(nPriority)
{
	m_bBillboard = false;
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));
	m_nStartOffsetCount = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CFan3D::~CFan3D()
{

}

//=====================================================
// ��������
//=====================================================
CFan3D *CFan3D::Create(int nPriority, int nNumVtx)
{
	CFan3D *pFan3D = nullptr;

	// �C���X�^���X����
	pFan3D = new CFan3D(nPriority);

	if (pFan3D != nullptr)
	{
		pFan3D->SetNumVtx(nNumVtx);

		// ����������
		pFan3D->Init();
	}

	return pFan3D;
}

//=====================================================
// ������
//=====================================================
HRESULT CFan3D::Init(void)
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
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nNumVtx + 2),
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				pVtxBuff,
				nullptr);
		}
	}

	// ���_�ݒ�
	SetVtx();

	EnableLighting(false);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CFan3D::Uninit(void)
{
	CFan::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CFan3D::Update(void)
{

}

//=====================================================
// ���_�ʒu�ݒ�
//=====================================================
void CFan3D::SetVtx(void)
{
	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();
	int nNumVtx = GetNumVtx();
	float fAngleMax = GetAngleMax();
	float fRadius = GetRadius();
	float fRateAngle = GetRateAngle();

	if (pVtxBuff != nullptr)
	{
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���S�̒��_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pVtx[0].tex = D3DXVECTOR2{ 0.5f,0.5f };
		pVtx[0].col = GetCol();

		for (int i = 1;i < nNumVtx + 2;i++)
		{// �~���̒��_�̐ݒ�
			float fAngle = (fAngleMax * fRateAngle) * ((float)(i - 1) / (float)nNumVtx);

			universal::LimitRot(&fAngle);

			pVtx[i].pos =
			{
				sinf(fAngle) * fRadius,
				cosf(fAngle) * fRadius,
				0.0f,
			};

			float f = 0.1f * sinf((float)i * 0.01f);

			D3DXVECTOR2 tex =
			{
				0.5f + sinf(fAngle) * 0.5f,
				0.5f - cosf(fAngle) * 0.5f,
			};

			pVtx[i].tex = tex;

			pVtx[i].col = GetCol();
		}

		// ���_�o�b�t�@�̃A�����b�N
		pVtxBuff->Unlock();
	}
}

//=====================================================
// �e�N�X�`�����W�ݒ菈��
//=====================================================
void CFan3D::SetTex(float fScrollX, float fScrollY)
{
	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();
	int nNumVtx = GetNumVtx();
	float fAngleMax = GetAngleMax();
	float fRadius = GetRadius();
	float fRateAngle = GetRateAngle();

	if (pVtxBuff != nullptr)
	{
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < nNumVtx + 2; i++)
		{
			float fTexY = fScrollY * sinf((float)(m_nStartOffsetCount + i) / SWING_WIDTH);

			pVtx[i].tex.x = pVtx[i].tex.x + fScrollX;
			pVtx[i].tex.y = pVtx[i].tex.y + fTexY;
		}

		m_nStartOffsetCount++;
		
		// ���_�o�b�t�@�̃A�����b�N
		pVtxBuff->Unlock();
	}
}

//=====================================================
// �`��
//=====================================================
void CFan3D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// ���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();

	if (pDevice != nullptr && pVtxBuff != nullptr)
	{
		//���[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&m_mtxWorld);

		if (m_bBillboard)
		{// �r���{�[�h�`��
			DrawBillboard();
		}
		else
		{// �ʏ�̕`��
			DrawNormal();
		}

		//���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �J�����O�𖳌���
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// �|���S���̕`��
		CFan::Draw();

		// �J�����O��L����
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

//=====================================================
// �ʏ�`��
//=====================================================
void CFan3D::DrawNormal(void)
{
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}

//=====================================================
// �r���{�[�h�`��
//=====================================================
void CFan3D::DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();

	//�r���[�}�g���b�N�X�擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����Ɍ�����
	D3DXMatrixInverse(&m_mtxWorld, nullptr, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}
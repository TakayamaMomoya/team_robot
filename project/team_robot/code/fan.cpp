//*****************************************************
//
// ��|���S���̏���[fan.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "fan.h"
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
	const float INITIAL_RADIUS = 50.0f;	// �����̔��a
	const char* TEX_PATH = "data\\TEXTURE\\UI\\gauge.png";	// �e�N�X�`���̃p�X
}

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CFan::CFan(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_fRadius = 0.0f;
	m_nNumVtx = 0;
	m_fRateAngle = 1.0f;
	m_fAngleMax = 0.0f;
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_nIdxTexture = -1;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CFan::~CFan()
{

}

//=====================================================
// ������
//=====================================================
HRESULT CFan::Init(void)
{
	// �e�N�X�`���ǂݍ���
	int nIdx = CTexture::GetInstance()->Regist(TEX_PATH);
	SetIdxTexture(nIdx);

	m_fRadius = INITIAL_RADIUS;
	m_fAngleMax = D3DX_PI * 2;
	m_pos = D3DXVECTOR3{ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };

	// �F������
	SetCol(m_col);

	// ���_�ݒ�
	SetVtx();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CFan::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �������g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CFan::Update(void)
{

}

//=====================================================
// �`��
//=====================================================
void CFan::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr && m_pVtxBuff != nullptr)
	{
		// �e�N�X�`���ݒ�
		int nIdxTexture = GetIdxTexture();
		LPDIRECT3DTEXTURE9 pTexture = CTexture::GetInstance()->GetAddress(nIdxTexture);
		pDevice->SetTexture(0, pTexture);

		// �`��
		int nNumVtx = GetNumVtx();
		pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, nNumVtx);
	}
}

//=====================================================
// �ݒ菈��
//=====================================================
void CFan::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
// �����ݒ菈��
//=====================================================
void CFan::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;

	universal::LimitRot(&m_rot.x);
	universal::LimitRot(&m_rot.y);
	universal::LimitRot(&m_rot.z);
}

//=====================================================
// �F�ݒ菈��
//=====================================================
void CFan::SetCol(D3DXCOLOR col)
{
	m_col = col;

	// ���_���̃|�C���^
	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr)
	{
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < m_nNumVtx + 2; nCnt++)
		{
			pVtx[nCnt].col = (D3DCOLOR)m_col;

			pVtx[nCnt].rhw = 1.0;
		}

		// ���_�o�b�t�@�̃A�����b�N
		m_pVtxBuff->Unlock();
	}
}
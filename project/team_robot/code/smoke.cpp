//*****************************************************
//
// ���̏���[smoke.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "smoke.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SPEED_MOVE = 1.0f;	// �ړ����x
const float SPEED_AVOID = 0.01f;	// �����鑬�x
const float SPEED_APPER = 0.05f;	// �o�����x
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CSmoke::CSmoke(int nPriority) : CPolygon3D(nPriority)
{
	m_move = { 0.0f,0.0f,0.0f };
	m_bTurn = false;
}

//=====================================================
//	�f�X�g���N�^
//=====================================================
CSmoke::~CSmoke()
{

}

//=====================================================
// ��������
//=====================================================
CSmoke *CSmoke::Create(D3DXVECTOR3 pos)
{
	CSmoke *pSmoke = nullptr;

	pSmoke = new CSmoke;

	if (pSmoke != nullptr)
	{
		pSmoke->Init();

		pSmoke->SetPosition(pos);
	}

	return pSmoke;
}

//=====================================================
// ����������
//=====================================================
HRESULT CSmoke::Init(void)
{
	CPolygon3D::Init();

	// �e��ݒ�
	SetMode(MODE::MODE_BILLBOARD);

	int nIdx = Texture::GetIdx("data\\TEXTURE\\EFFECT\\smoke.png");
	SetIdxTexture(nIdx);

	// �ړ��ʐݒ�
	m_move =
	{
		(float)universal::RandRange(10,-10),
		0.0f,
		(float)universal::RandRange(10,-10),
	};

	D3DXVec3Normalize(&m_move, &m_move);

	m_move *= SPEED_MOVE;

	SetSize(500.f, 500.0f);

	SetVtx();

	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSmoke::Uninit(void)
{
	CPolygon3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CSmoke::Update(void)
{
	CPolygon3D::Update();

	// �ړ�
	D3DXVECTOR3 pos = GetPosition();

	pos += m_move;

	SetPosition(pos);

	// �i�X�����鏈��
	D3DXCOLOR col = GetColor();

	if (!m_bTurn)
	{
		col.a += SPEED_APPER;

		if (col.a >= 1.0f)
			m_bTurn = true;
	}
	else
	{
		col.a -= SPEED_AVOID;

		if (col.a <= 0.0f)
		{
			col.a = 0.0f;

			Uninit();

			return;
		}
	}

	SetColor(col);
}

//=====================================================
// �`�揈��
//=====================================================
void CSmoke::Draw(void)
{
	CPolygon3D::Draw();
}
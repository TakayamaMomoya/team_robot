//*****************************************************
//
// �^�C���̏���[timer.cpp]
// Author:�X��x��
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "meter.h"
#include "debugproc.h"
#include "texture.h"
#include "UI.h"
#include "number.h"
#include "player.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* PATH_TEX_BACKMETER = "data\\TEXTURE\\UI\\meter00.png";	// ���[�^�[�w�i�̃e�N�X�`���p�X
const char* PATH_TEX_NEEDLE = "data\\TEXTURE\\UI\\Needle.png";	// �j�̃e�N�X�`���p�X
const int PLACE = 3;		// ����
const D3DXVECTOR2 SIZE_METER = { SCREEN_WIDTH * 0.13f, SCREEN_WIDTH * 0.13f };	// ���[�^�[�w�i�̃T�C�Y
const D3DXVECTOR3 POS_METER = { SCREEN_WIDTH - SIZE_METER.x - 10.0f, SCREEN_HEIGHT - SIZE_METER.y - 10.0f, 0.0f };	// ���[�^�[�̈ʒu
const D3DXVECTOR2 SIZE_NUMBER = { SCREEN_WIDTH * 0.015f, SCREEN_WIDTH * 0.02f };	// ���[�^�[�̐����̃T�C�Y
const D3DXVECTOR2 SIZE_NEEDLE = { SIZE_METER.x * 0.2f, SIZE_METER.y * 0.9f };	// �j�̃T�C�Y
const float MIN_ANGLE_NEEDLE = D3DX_PI * 0.7f;	// �j�̍Œᑬ�x���̊p�x
const float MAX_ANGLE_NEEDLE = -D3DX_PI * 0.7f;	// �j�̍ō����x���̊p�x
}

//=====================================================
// �ÓI�����o�ϐ�
//=====================================================
CMeter* CMeter::m_pMeter = nullptr;

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CMeter::CMeter(int nPriority)
{
	m_pNumber = nullptr;	// �i���o�[�̃|�C���^
	m_pNeedle = nullptr;		// �j�̃|�C���^
	m_pBackMeter = nullptr;	// ���[�^�[�w�i�̃|�C���^
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMeter::~CMeter()
{

}

//=====================================================
// ��������
//=====================================================
CMeter* CMeter::Create(void)
{
	if (m_pMeter == nullptr)
	{// �C���X�^���X����
		m_pMeter = new CMeter;

		if (m_pMeter != nullptr)
		{// ������
			m_pMeter->Init();
		}
	}

	return m_pMeter;
}

//=====================================================
// ����������
//=====================================================
HRESULT CMeter::Init()
{
	if (m_pBackMeter == nullptr)
	{
		m_pBackMeter = CUI::Create();

		// �e�N�X�`���ݒ�
		int nIdx = Texture::GetIdx(PATH_TEX_BACKMETER);
		m_pBackMeter->SetIdxTexture(nIdx);

		// �g�����X�t�H�[���ݒ�
		m_pBackMeter->SetPosition(POS_METER);
		m_pBackMeter->SetSize(SIZE_METER.x, SIZE_METER.y);
		m_pBackMeter->SetVtx();
	}

	if (m_pNeedle == nullptr)
	{// �j�̐���
		// ����
		m_pNeedle = CUI::Create();

		// �e�N�X�`���ݒ�
		int nIdx = Texture::GetIdx(PATH_TEX_NEEDLE);
		m_pNeedle->SetIdxTexture(nIdx);

		// �g�����X�t�H�[���ݒ�
		m_pNeedle->SetPosition(POS_METER);
		m_pNeedle->SetSize(SIZE_NEEDLE.x, SIZE_NEEDLE.y);
		m_pNeedle->SetVtx();
	}

	if (m_pNumber == nullptr)
	{// ���[�^�[�l�̐���
		// ����
		m_pNumber = CNumber::Create(PLACE, 0);

		// �g�����X�t�H�[���ݒ�
		m_pNumber->SetPosition(D3DXVECTOR3(POS_METER.x - SIZE_NUMBER.x * 2, POS_METER.y - SIZE_NUMBER.y * 1.8f, 0.0f));
		m_pNumber->SetSizeAll(SIZE_NUMBER.x, SIZE_NUMBER.y);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CMeter::Uninit(void)
{
	if (m_pNumber != nullptr)
	{
		m_pNumber->Uninit();
		m_pNumber = nullptr;
	}

	if (m_pNeedle != nullptr)
	{
		m_pNeedle->Uninit();
		m_pNeedle = nullptr;
	}

	if (m_pBackMeter != nullptr)
	{
		m_pBackMeter->Uninit();
		m_pBackMeter = nullptr;
	}

	m_pMeter = nullptr;

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CMeter::Update(void)
{
	// ���[�^�[�l
	Acceleration();

	// �j�𓮂�������
	Needle();
}

//=====================================================
// ���[�^�[�l����
//=====================================================
void CMeter::Acceleration()
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// ���x�擾
	float fPlayerSpeed = pPlayer->GetSpeed();

	if (m_pNumber != nullptr)
	{// �l�\���̐���
		m_pNumber->SetValue((int)fPlayerSpeed, PLACE);
	}
}

//=====================================================
// ���[�^�[�j����
//=====================================================
void CMeter::Needle()
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (m_pNeedle == nullptr || pPlayer == nullptr)
		return;

	float fSpeed = pPlayer->GetSpeed();
	float fSpeedMax = pPlayer->GetParam().fSpeedMaxInitial;

	float fRate = fSpeed / fSpeedMax;
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	float fDiffAngle = MAX_ANGLE_NEEDLE - MIN_ANGLE_NEEDLE;

	float fRot = MIN_ANGLE_NEEDLE + fDiffAngle * fRate;

	m_pNeedle->SetRotation(fRot);
	m_pNeedle->SetVtx();
}
//*****************************************************
//
// ���b�V���t�B�[���h�G�f�B�^�[�̏���[editMeshfield.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "editMeshfield.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "inputmouse.h"
#include "inputkeyboard.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float DEFAULT_DEPTH = 100.0f;	// �f�t�H���g�̐[��
const float DEFAULT_RADIUS = 1000.0f;	// �f�t�H���g�͈̔�
const float HEIGHT_CYLINDER = 50000.0f;	// �V�����_�[�̍���
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEditMeshfield::CEditMeshfield() : m_fDepthDent(0.0f), m_fRadiusDent(0.0f), m_pMeshCylinder(nullptr)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEditMeshfield::~CEditMeshfield()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEditMeshfield::Init(void)
{
	CEdit::Init();

	m_fDepthDent = DEFAULT_DEPTH;
	m_fRadiusDent = DEFAULT_RADIUS;

	// ���b�V���V�����_�[����
	m_pMeshCylinder = CMeshCylinder::Create();

	if (m_pMeshCylinder != nullptr)
	{
		m_pMeshCylinder->SetIdxTexture(-1);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEditMeshfield::Uninit(void)
{
	if (m_pMeshCylinder != nullptr)
	{
		m_pMeshCylinder->Uninit();
		m_pMeshCylinder = nullptr;
	}

	CEdit::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEditMeshfield::Update(void)
{
	CEdit::Update();

	// �J�[�\���𓮂�������
	MoveCursor();

	// ���b�V���t�B�[���h���ւ��܂�����
	DentMeshField();
}

//=====================================================
// �J�[�\���𓮂�������
//=====================================================
void CEditMeshfield::MoveCursor(void)
{
	D3DXVECTOR3 posNear;
	D3DXVECTOR3 posFar;
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 posHit;

	universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiff);

	universal::CalcRayFlat(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), posNear, posFar, &posHit);

	SetPosition(posHit);

	if (m_pMeshCylinder != nullptr)
	{
		m_pMeshCylinder->SetPosition(posHit);
		m_pMeshCylinder->SetHeight(HEIGHT_CYLINDER);
		m_pMeshCylinder->SetRadius(m_fRadiusDent);
		m_pMeshCylinder->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_pMeshCylinder->SetVtx();
	}
}

//=====================================================
// ���b�V���t�B�[���h���ւ��܂�����
//=====================================================
void CEditMeshfield::DentMeshField(void)
{
	CMeshField *pMeshField = CMeshField::GetInstance();

	if (pMeshField == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	pMeshField->Dent(pos, m_fRadiusDent, m_fDepthDent);
}
//*****************************************************
//
// ���b�V�����̏���[meshRoad.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "meshCube.h"
#include "polygon3D.h"
#include "effect3D.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int NUM_POLYGON = 6;	// �|���S���̐�
	const float SIZE_DEFAULT = 500.0f;	// �f�t�H���g�T�C�Y
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CMeshCube::CMeshCube(int nPriority) : CObject3D(nPriority), m_size()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMeshCube::~CMeshCube()
{

}

//=====================================================
// ��������
//=====================================================
CMeshCube *CMeshCube::Create(void)
{
	CMeshCube *pMeshCube = nullptr;

	if (pMeshCube == nullptr)
	{
		// �C���X�^���X����
		pMeshCube = new CMeshCube;

		if (pMeshCube != nullptr)
		{
			// ����������
			pMeshCube->Init();
		}
	}

	return pMeshCube;
}

//=====================================================
// ����������
//=====================================================
HRESULT CMeshCube::Init(void)
{
	CObject3D::Init();

	m_size = { SIZE_DEFAULT,SIZE_DEFAULT,SIZE_DEFAULT };

	// �z��̃��T�C�Y
	m_apPolygon3D.resize(E_Rot::ROT_MAX);

	D3DXVECTOR3 aPos[E_Rot::ROT_MAX] =
	{
		{ 0.0f, SIZE_DEFAULT, 0.0f },
		{ 0.0f, -SIZE_DEFAULT, 0.0f },
		{ SIZE_DEFAULT, 0.0f, 0.0f },
		{ -SIZE_DEFAULT, 0.0f, 0.0f },
		{ 0.0f, 0.0f, -SIZE_DEFAULT },
		{ 0.0f, 0.0f, SIZE_DEFAULT },
	};

	D3DXVECTOR3 aRot[E_Rot::ROT_MAX] =
	{
		{ 0.0f, 0.0f, 0.0f },
		{ D3DX_PI, 0.0f, 0.0f },
		{ D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f },
		{ D3DX_PI * 0.5f, D3DX_PI * -0.5f, 0.0f },
		{ D3DX_PI * 0.5f, -D3DX_PI, 0.0f },
		{ D3DX_PI * -0.5f, D3DX_PI, 0.0f }
	};

	for ( int i = 0; i < E_Rot::ROT_MAX; i++ )
	{
		m_apPolygon3D[i] = CPolygon3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		m_apPolygon3D[i]->SetSize(SIZE_DEFAULT, SIZE_DEFAULT);
		m_apPolygon3D[i]->SetPosition(aPos[i]);
		m_apPolygon3D[i]->SetRotation(aRot[i]);
	}

	// �@���̐ݒ�
	SetNormal();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CMeshCube::Uninit(void)
{
	for (int i = 0; i < (int)m_apPolygon3D.size(); i++)
	{
		if (m_apPolygon3D[i] != nullptr)
		{
			m_apPolygon3D[i]->Uninit();
		}
	}

	m_apPolygon3D.clear();

	CObject3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CMeshCube::Update(void)
{
	D3DXMATRIX mtx = GetMatrix();

	for (auto it : m_apPolygon3D)
	{
		it->SetMatrixParent(mtx);
	}

	// �@���̐ݒ�
	SetNormal();
}

//=====================================================
// �`�揈��
//=====================================================
void CMeshCube::Draw(void)
{
	CObject3D::Draw();
}

//=====================================================
// �@���̐ݒ�
//=====================================================
void CMeshCube::SetNormal(void)
{
	for (auto it : m_apPolygon3D)
	{
		D3DXVECTOR3 nor = it->GetUp();
		D3DXVECTOR3 pos = { GetMatrix()._41,GetMatrix()._42 ,GetMatrix()._43 };

		CDebugProc::GetInstance()->Print("\nnor[%f,%f,%f]", nor.x, nor.y, nor.z);
		CDebugProc::GetInstance()->Print("\npos[%f,%f,%f]", pos.x, pos.y, pos.z);

		CEffect3D::Create(pos + nor * 100.0f, 50.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		it->SetNormal(nor);
	}
}
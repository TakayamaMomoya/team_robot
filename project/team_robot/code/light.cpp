//*****************************************************
//
// ���C�g�̏���[light.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "inputkeyboard.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SPEED_CHANGE_COL = 0.1f;	// �F��ς��鑬�x
const D3DXCOLOR INTIIAL_COLOR[MAX_LIGHT] =
{// �����̐F
	D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f),
	D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f),
	D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f)
};
}

//====================================================
//����������
//====================================================
HRESULT CLight::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXVECTOR3 vecDir;

	//���C�g�P�̐ݒ�============================================
	//���C�g�̎�ސݒ�
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U���̐ݒ�
	m_aLight[0].Diffuse = INTIIAL_COLOR[0];

	//���C�g�̕����ݒ�
	vecDir = D3DXVECTOR3(-1.4f, 0.24f, -2.21f);
	D3DXVec3Normalize(&vecDir, &vecDir);		//�x�N�g�����K��
	m_aLight[0].Direction = vecDir;

	//���C�g�ݒ�
	pDevice->SetLight(0, &m_aLight[0]);

	//���C�g�L����
	pDevice->LightEnable(0, TRUE);

	//���C�g�Q�̐ݒ�============================================
	//���C�g�̎�ސݒ�
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U���̐ݒ�
	m_aLight[1].Diffuse = INTIIAL_COLOR[1];

	//���C�g�̕����ݒ�
	vecDir = D3DXVECTOR3(1.42f, -0.8f, 0.08f);
	D3DXVec3Normalize(&vecDir, &vecDir);		//�x�N�g�����K��
	m_aLight[1].Direction = vecDir;

	//���C�g�ݒ�
	pDevice->SetLight(1, &m_aLight[1]);

	//���C�g�L����
	pDevice->LightEnable(1, TRUE);

	//���C�g�R�̐ݒ�============================================
	//���C�g�̎�ސݒ�
	m_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U���̐ݒ�
	m_aLight[2].Diffuse = INTIIAL_COLOR[2];

	//���C�g�̕����ݒ�
	vecDir = D3DXVECTOR3(-0.29f, -0.8f, 0.55f);
	D3DXVec3Normalize(&vecDir, &vecDir);		//�x�N�g�����K��
	m_aLight[2].Direction = vecDir;

	//���C�g�ݒ�
	pDevice->SetLight(2, &m_aLight[2]);

	//���C�g�L����
	pDevice->LightEnable(2, TRUE);

	// �F�̏�����
	ResetColDest();

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		m_aInfo[i].col = INTIIAL_COLOR[i];
	}

	return S_OK;
}

//====================================================
//�I������
//====================================================
void CLight::Uninit(void)
{

}

//====================================================
//�X�V����
//====================================================
void CLight::Update(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		// �F�̕␳
		m_aInfo[i].col.r += (m_aInfo[i].colDest.r - m_aInfo[i].col.r) * SPEED_CHANGE_COL;
		m_aInfo[i].col.g += (m_aInfo[i].colDest.g - m_aInfo[i].col.g) * SPEED_CHANGE_COL;
		m_aInfo[i].col.b += (m_aInfo[i].colDest.b - m_aInfo[i].col.b) * SPEED_CHANGE_COL;
		m_aInfo[i].col.a += (m_aInfo[i].colDest.a - m_aInfo[i].col.a) * SPEED_CHANGE_COL;

		// ���C�g�̊g�U���̐ݒ�
		m_aLight[i].Diffuse = m_aInfo[i].col;

		// ���C�g�ݒ�
		pDevice->SetLight(i, &m_aLight[i]);
	}
}

//====================================================
// �F�̐ݒ�
//====================================================
void CLight::SetColDest(int nIdx, D3DXCOLOR col)
{
	m_aInfo[nIdx].colDest = col;
}

//====================================================
// �F�̃��Z�b�g
//====================================================
void CLight::ResetColDest(void)
{
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		m_aInfo[i].colDest = INTIIAL_COLOR[i];
	}
}
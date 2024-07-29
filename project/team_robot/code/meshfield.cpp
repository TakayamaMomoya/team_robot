//*****************************************************
//
// ���b�V���t�B�[���h�̏���[meshfield.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "inputkeyboard.h"
#include "player.h"
#include "debugproc.h"
#include "game.h"
#include "effect3D.h"
#include "texture.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MESHFIELD_TEX_FILE			"data\\TEXTURE\\BG\\field00.jpg"				// �e�N�X�`���t�@�C����
#define MOVE_SPEED					(1.0f)										// �ړ����x
#define MESH_LENGTH					(800.0f)									// ���b�V���̈�ӂ̒���
#define MESH_U						(254)											// ���̃u���b�N��
#define MESH_V						(254)											// �c�̃u���b�N��
#define SPLIT_TEX					(10)										// �e�N�X�`��������
#define CHENGE_LENGTH	(10000)	// ����ł��钸�_�܂ł̋���
#define ANGLE_SLIP	(0.7f)	// �������p�x
#define CMP_LENGTH	(1000.0f)	// ���肷�锼�a

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CMeshField *CMeshField::m_pMeshField = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CMeshField::CMeshField()
{
	ZeroMemory(&m_MeshField, sizeof(m_MeshField));
	m_pIdxBuff = nullptr;
	m_nIdxTexture = -1;
	m_pVtxBuff = nullptr;
	m_fLengthMesh = 0.0f;
	m_nDivNumU = 0;
	m_nDivNumV = 0;
	m_col = { 0.0f,0.0f,0.0f,0.0f };
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMeshField::~CMeshField()
{

}

//=====================================================
// ��������
//=====================================================
CMeshField *CMeshField::Create(void)
{
	if (m_pMeshField == nullptr)
	{
		m_pMeshField = new CMeshField;

		if (m_pMeshField != nullptr)
		{
			m_pMeshField->Init();
		}
	}

	return m_pMeshField;
}

//=====================================================
// ����������
//=====================================================
HRESULT CMeshField::Init(void)
{
	CObject3D::Init();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �l�̏����ݒ�
	m_fLengthMesh = MESH_LENGTH;
	m_nDivNumU = MESH_U;
	m_nDivNumV = MESH_V;
	m_col = { 1.0f,1.0f,1.0f,1.0f };

	// �Ǎ�����
	//Load("data\\MAP\\field00.bin");

	//Reset();

	m_nDivTex = SPLIT_TEX;

	// �e�N�X�`���̓Ǎ�
	m_nIdxTexture = CTexture::GetInstance()->Regist(MESHFIELD_TEX_FILE);

	// ���_���v�Z
	m_MeshField.nNumVtx = (m_nDivNumU + 1) * (m_nDivNumV + 1);

	if (m_pVtxBuff == nullptr)
	{
		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshField.nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr);
	}

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCountV, nCountU;
	int nCntVtx = 0;

	for (nCountV = 0;nCountV < m_nDivNumV + 1; nCountV++)
	{// ���_���W�̐ݒ�
		for (nCountU = 0; nCountU < m_nDivNumU + 1; nCountU++)
		{
			// ���_���W
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.x = (nCountU - m_nDivNumU * 0.5f) * m_fLengthMesh;
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.z = (((m_nDivNumU) * 0.5f) - nCountV) * m_fLengthMesh;

			// �e�N�X�`�����W
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].tex = D3DXVECTOR2
			(
			    ((float)m_nDivTex / (float)m_nDivNumU) * nCountU,
				((float)m_nDivTex / (float)m_nDivNumU) * nCountV
			);
		}
	}

	for (int nCnt = 0;nCnt < m_MeshField.nNumVtx;nCnt++)
	{
		// ���_�J���[�̐ݒ�
		pVtx[nCnt].col = m_col;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	// �@���̐ݒ�
	SetNormal();

	m_MeshField.nNumIdx = (m_nDivNumU + 1) * (m_nDivNumV + 1) + ((m_nDivNumV + 1) - 2) * (m_nDivNumU + 1) + (((m_nDivNumV + 1) - 2) * 2);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_MeshField.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCount = 0; nCount < m_MeshField.nNumIdx / 2; nCount++)
	{// �C���f�b�N�X����
		if (
			nCount % ((m_nDivNumU + 1) + (m_nDivNumU + 2) * (nCount / (m_nDivNumU + 2))) == 0
			&& nCount != 0
			)
		{
			pIdx[nCount * 2 + 1] = ((m_nDivNumU + 1) * 2) + (m_nDivNumU + 1) * (nCount / (m_nDivNumU + 2));

			pIdx[nCount * 2] = m_nDivNumU + (m_nDivNumU + 1) * (nCount / (m_nDivNumU + 2));
		}
		else
		{
			pIdx[nCount * 2 + 1] = nCount - (nCount / (m_nDivNumU + 2));

			pIdx[nCount * 2] = (nCount - (nCount / (m_nDivNumU + 2))) + (m_nDivNumU + 1);
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CMeshField::Uninit(void)
{
	m_pMeshField = nullptr;

	if (m_pVtxBuff != nullptr)
	{// ���_�o�b�t�@�|�C���^�̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pIdxBuff != nullptr)
	{// �C���f�b�N�X�o�b�t�@�|�C���^�̔j��
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	CObject3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CMeshField::Update(void)
{
	Edit();
}

//=====================================================
// �G�f�B�b�g�̏���
//=====================================================
void CMeshField::Edit(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	float fLength;
	D3DXVECTOR3 vecDiff;

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���͏�����
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard->GetPress(DIK_UP))
	{
		for (int nCntVtx = 0; nCntVtx < m_MeshField.nNumVtx; nCntVtx++)
		{
			vecDiff = pPlayer->GetPosition() - pVtx[nCntVtx].pos;

			fLength = D3DXVec3Length(&vecDiff);

			if (fLength < CHENGE_LENGTH)
			{
				pVtx[nCntVtx].pos.y += 20.0f * (1.0f - (fLength / CHENGE_LENGTH));
			}
		}
	}

	if (pKeyboard->GetPress(DIK_DOWN))
	{
		for (int nCntVtx = 0; nCntVtx < m_MeshField.nNumVtx; nCntVtx++)
		{
			vecDiff = pPlayer->GetPosition() - pVtx[nCntVtx].pos;

			fLength = D3DXVec3Length(&vecDiff);

			if (fLength < CHENGE_LENGTH)
			{
				pVtx[nCntVtx].pos.y -= 20.0f * (1.0f - (fLength / CHENGE_LENGTH));
			}
		}
	}

	/*if (pKeyboard->GetPress(DIK_F))
	{
		m_col.r += 0.01f;
	}
	if (pKeyboard->GetPress(DIK_V))
	{
		m_col.r -= 0.01f;
	}

	if (pKeyboard->GetPress(DIK_G))
	{
		m_col.g += 0.01f;
	}
	if (pKeyboard->GetPress(DIK_B))
	{
		m_col.g -= 0.01f;
	}

	if (pKeyboard->GetPress(DIK_H))
	{
		m_col.b += 0.01f;
	}
	if (pKeyboard->GetPress(DIK_N))
	{
		m_col.b -= 0.01f;
	}*/

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	if (pKeyboard->GetTrigger(DIK_RIGHT))
	{
		// �@���̐ݒ�
		SetNormal();
	}

	if (pKeyboard->GetTrigger(DIK_F8))
	{// �ۑ�����
		Save();
	}

	if (pKeyboard->GetTrigger(DIK_F7))
	{// �Ǎ�����
		Reset();
	}
}

//=====================================================
// �@���̐ݒ�
//=====================================================
void CMeshField::SetNormal(void)
{
	D3DXVECTOR3 vec1, vec2, nor1, nor2, nor3, nor4, nor5, nor6, nor7;
	int nCountU, nCountV, nCntVtx = 0;

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// ���_�J���[�̐ݒ�
		pVtx[nCnt].col = m_col;
	}

	for (nCountV = 0; nCountV < m_nDivNumV + 1; nCountV++)
	{// �@���̐ݒ�
		for (nCountU = 0; nCountU < m_nDivNumU + 1; nCountU++, nCntVtx++)
		{
			if (nCntVtx == 0)
			{// �ŏ��̏ꍇ================================================
			 // ���ƉE�̖@�������߁A�����Ċ���
				// �����̖@�������߂�
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// �E�̖@�������߂�
				vec1 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx + 1].pos;

				D3DXVec3Cross(&nor2, &vec1, &vec2);

				D3DXVec3Normalize(&nor2, &nor2);

				// �@�������߂�
				pVtx[nCntVtx].nor = (nor1 + nor2) * 0.5f;
			}
			else if (nCntVtx == m_MeshField.nNumVtx - 1)
			{// �Ō�̏ꍇ================================================
			 // ���Ə�̖@�������߁A�����Ċ���
			 // �����̖@�������߂�
				vec1 = pVtx[nCntVtx - m_nDivNumU - 2].pos - pVtx[nCntVtx - 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx - 1].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// �E�̖@�������߂�
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx - m_nDivNumU - 1].pos;
				vec2 = pVtx[nCntVtx - m_nDivNumU - 2].pos - pVtx[nCntVtx - m_nDivNumU - 1].pos;

				D3DXVec3Cross(&nor2, &vec1, &vec2);

				D3DXVec3Normalize(&nor2, &nor2);

				// �@�������߂�
				pVtx[nCntVtx].nor = (nor1 + nor2) * 0.5f;
			}
			else if (nCntVtx == m_nDivNumU + 1)
			{// �E��̖ʈ��================================================
				vec1 = pVtx[nCntVtx + m_nDivNumU + 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx - 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// �@�������߂�
				pVtx[nCntVtx].nor = nor1;
			}
			else if (nCntVtx == m_nDivNumU * m_nDivNumV + m_nDivNumU)
			{// �����̖ʈ��================================================
				vec1 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// �@�������߂�
				pVtx[nCntVtx].nor = nor1;
			}
			else if (nCntVtx < m_nDivNumU)
			{// ��̕ӂŖʂ��O��================================================
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// �E�̖@�������߂�
				vec1 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx + 1].pos;

				D3DXVec3Cross(&nor2, &vec1, &vec2);

				D3DXVec3Normalize(&nor2, &nor2);

				// �ׂ̖@�������炤
				nor3 = pVtx[nCntVtx - 1].nor;

				// �@�������߂�
				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3) / 3;
			}
			else if (nCntVtx % m_nDivNumU + 1 == 0)
			{// ���̕ӂŖʂ��O��================================================
			 // ��̖@�������炤
				nor1 = pVtx[nCntVtx - m_nDivNumU - 1].nor;
				// �E�̖@���͏�Ɠ���
				nor2 = nor1;

				// ���̖@�������߂�
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor3, &vec1, &vec2);

				D3DXVec3Normalize(&nor3, &nor3);

				// �E���̖@�������߂�
				nor4 = (nor3 + nor2) / 2;

				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3 + nor4) / 4;
			}
			else if (nCntVtx == m_nDivNumU + nCountV * (m_nDivNumU + 1))
			{// �E�̕ӂŖʎO��================================================
			 // ����̖@�������炤
				nor1 = pVtx[nCntVtx - m_nDivNumU - 2].nor;
				// ��̖@�������炤
				nor2 = pVtx[nCntVtx - m_nDivNumU - 1].nor;
				// ���̖@�������炤
				nor3 = pVtx[nCntVtx - 1].nor;

				// �E��̖@�������߂�
				vec1 = pVtx[nCntVtx + m_nDivNumU + 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx - 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// �����̖@�������߂�
				vec1 = pVtx[nCntVtx - 1].pos - pVtx[nCntVtx + m_nDivNumU].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				// ���̖@�������߂�
				nor6 = (nor4 + nor5) / 2;

				// �@�������߂�
				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3 + nor6) / 4;
			}
			else if (nCntVtx > m_nDivNumU * m_nDivNumV + m_nDivNumU)
			{// ���̕ӂŖʎO��================================================
			 // ���̖@�������炤
				nor1 = pVtx[nCntVtx - 1].nor;
				// ����̖@�������炤
				nor2 = pVtx[nCntVtx - m_nDivNumU - 2].nor;

				// �E��̖@�������߂�
				vec1 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;
				vec2 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// �����̖@�������߂�
				vec1 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				nor3 = (nor4 + nor5) / 2;

				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3) / 3;
			}
			else
			{// �ʂ��Z�̏ꍇ================================================
			 // ���̖@�������炤
				nor1 = pVtx[nCntVtx - 1].nor;
				// ����̖@�������炤
				nor2 = pVtx[nCntVtx - m_nDivNumU - 2].nor;
				// ��̖@�������炤
				nor3 = pVtx[nCntVtx - m_nDivNumU - 1].nor;

				// �E��̎l�p�`�̖@�������߂�
				// �E��̖@�������߂�
				vec1 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;
				vec2 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// �����̖@�������߂�
				vec1 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				nor6 = (nor5 + nor4) / 2;

				// �����̎l�p�`�̖@�������߂�
				// �E��̖@�������߂�
				vec1 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx + 1].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// �����̖@�������߂�
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				nor7 = (nor4 + nor5) / 2;

				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3 + nor4 + nor5 + nor7) / 6;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=====================================================
// �����̎擾
//=====================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pNor)
{
	if (m_pVtxBuff == nullptr)
	{
		return pos.y;
	}

	// �ϐ��錾
	D3DXVECTOR3 vec1, vec2, vec3, vec4, vec5, nor, vec6;
	D3DXVECTOR3 vecDiff;
	int nRu, nRd, nLu, nLd;
	float fHeight = pos.y;
	int nCntV, nCntU;

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	nCntU = (int)(m_fLengthMesh * m_nDivNumU / 2 + pos.x) / (int)m_fLengthMesh;
	nCntV = (int)(m_fLengthMesh * m_nDivNumV / 2 - pos.z) / (int)m_fLengthMesh;

	// ���_�ԍ��v�Z
	nLd = (m_nDivNumU + 1) * (nCntV + 1) + nCntU;
	nLu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU + 1) + nCntU;
	nRd = (m_nDivNumU + 1) * (nCntV + 1) + 1 + nCntU;
	nRu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU)+nCntU;

	if (pos.x < -m_fLengthMesh * m_nDivNumU / 2 || 
		pos.x > m_fLengthMesh * m_nDivNumU / 2 ||
		pos.z < -m_fLengthMesh * m_nDivNumU / 2 ||
		pos.z > m_fLengthMesh * m_nDivNumU / 2)
	{
		return 0.0f;
	}

	// �������擾
	vecDiff = pVtx[nLd].pos - pos;

	// ����ւ̃x�N�g���Z�o
	vec1 = pVtx[nLu].pos - pVtx[nLd].pos;
	// �E���ւ̃x�N�g���Z�o
	vec2 = pVtx[nRd].pos - pVtx[nLd].pos;

	// �E���ւ̃x�N�g���Z�o
	vec3 = pVtx[nRd].pos - pVtx[nRu].pos;
	// ����ւ̃x�N�g���Z�o
	vec4 = pVtx[nLu].pos - pVtx[nRu].pos;

	// ���S�̃x�N�g���Z�o
	vec5 = pVtx[nLu].pos - pVtx[nRd].pos;

	if ((vec5.z * (pos.x - pVtx[nLu].pos.x)) - (vec5.x * (pos.z - pVtx[nLu].pos.z)) < 0)
	{// �����̃|���S���̏�ɂ���
	 // �@���̎Z�o
		D3DXVec3Cross(&nor, &vec1, &vec2);

		// �@�����K��
		D3DXVec3Normalize(&nor, &nor);

		// ���όv�Z
		D3DXVECTOR3 vec = { -(pos.x - pVtx[nLd].pos.x) * nor.x,0.0f,-(pos.z - pVtx[nLd].pos.z) * nor.z };

		if (nor.y != 0)
		{// �����̎擾
			fHeight = (vec.x + vec.z + pVtx[nLd].pos.y * nor.y) / nor.y;

			if (pNor != nullptr)
			{
				*pNor = nor;
			}
		}
	}
	else
	{// �E��̃|���S���̏�ɂ���
		// �@���̎Z�o
		D3DXVec3Cross(&nor, &vec3, &vec4);

		// �@�����K��
		D3DXVec3Normalize(&nor, &nor);

		// ���όv�Z
		D3DXVECTOR3 vec = { -(pos.x - pVtx[nRu].pos.x) * nor.x,0.0f,-(pos.z - pVtx[nRu].pos.z) * nor.z };

		if (nor.y != 0)
		{// �����̎擾
			fHeight = (vec.x + vec.z + pVtx[nRu].pos.y * nor.y) / nor.y;

			if (pNor != nullptr)
			{
				*pNor = nor;
			}
		}
	}

	if (pMove != nullptr)
	{
		if (nor.y * nor.y < ANGLE_SLIP * ANGLE_SLIP)
		{
			*pMove += nor * 0.5f;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return fHeight;
}

//=====================================================
// �ւ��ޏ���
//=====================================================
void CMeshField::Dent(D3DXVECTOR3 pos,float fRadius,float fDepth)
{
	// �v�Z�p�ϐ�
	D3DXVECTOR3 vecDiff;
	float fLength;

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// �����̒������擾
		vecDiff = pos - (pVtx[nCnt].pos + GetPosition());
		fLength = D3DXVec3Length(&vecDiff);

		float fRate = fLength / fRadius;

		if (fLength < fRadius)
		{
			pVtx[nCnt].pos.y += fDepth - (fDepth * (fLength / fRadius));
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=====================================================
// ���C�œ������Ă��邩�̃`�F�b�N
//=====================================================
bool CMeshField::RayCheck(D3DXVECTOR3 pos, D3DXVECTOR3 vecDir, D3DXVECTOR3 posOwn)
{
	int nCntV, nCntU;
	int nCntVStart, nCntUStart;
	int nCntVEnd, nCntUEnd;
	int nLd, nLu, nRd, nRu;
	int nTemp;
	float fLength = 0.0f;
	BOOL bLd,bRu;
	D3DXVECTOR3 vecDiff = vecDir;

	bool bHit = false;
	pos.y += 20;

	// �v�Z����͈͂̐ݒ�
	nCntUStart = (int)(m_fLengthMesh * m_nDivNumU / 2 + pos.x) / (int)m_fLengthMesh;
	nCntVStart = (int)(m_fLengthMesh * m_nDivNumV / 2 - pos.z) / (int)m_fLengthMesh;

	nCntUEnd = (int)(m_fLengthMesh * m_nDivNumU / 2 + pos.x + vecDir.x) / (int)m_fLengthMesh;
	nCntVEnd = (int)(m_fLengthMesh * m_nDivNumV / 2 - pos.z - vecDir.z) / (int)m_fLengthMesh;

	if (nCntUEnd < nCntUStart)
	{
		nTemp = nCntUEnd;

		nCntUEnd = nCntUStart;

		nCntUStart = nTemp;
	}

	if (nCntVEnd < nCntVStart)
	{
		nTemp = nCntVEnd;

		nCntVEnd = nCntVStart;

		nCntVStart = nTemp;
	}

	// �����̐��K��
	D3DXVec3Normalize(&vecDir, &vecDir);

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntV = nCntVStart; nCntV < nCntVEnd; nCntV++)
	{// �S�|���S���̃`�F�b�N
		for (nCntU = nCntUStart; nCntU < nCntUEnd; nCntU++)
		{
			// ���_�ԍ��v�Z
			nLd = (m_nDivNumU + 1) * (nCntV + 1) + nCntU;
			nLu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU + 1) + nCntU;
			nRd = (m_nDivNumU + 1) * (nCntV + 1) + 1 + nCntU;
			nRu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU) + nCntU;

			bLd = D3DXIntersectTri(&pVtx[nLd].pos, &pVtx[nLu].pos, &pVtx[nRd].pos, &pos, &vecDir, nullptr, nullptr, &fLength);
			bRu = D3DXIntersectTri(&pVtx[nLu].pos, &pVtx[nRu].pos, &pVtx[nRd].pos, &pos, &vecDir, nullptr, nullptr, &fLength);

			// �������Ă��邩�`�F�b�N
			if (bLd || bRu)
			{
				if (D3DXVec3Length(&vecDiff) > fLength)
				{
					return true;
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return bHit;
}

//=====================================================
// �F�̕ύX
//=====================================================
void CMeshField::SetCol(D3DXCOLOR col)
{
	m_col = col;

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �F�̕ύX
	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// ���_�J���[�̐ݒ�
		pVtx[nCnt].col = m_col;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=====================================================
// �����̃��Z�b�g
//=====================================================
void CMeshField::Reset(void)
{
	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �F�̕ύX
	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// ���_�J���[�̐ݒ�
		pVtx[nCnt].pos.y = 0.0f;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=====================================================
// �`�揈��
//=====================================================
void CMeshField::Draw(void)
{
	CObject3D::Draw();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	
	// �e�N�X�`���ݒ�
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetInstance()->GetAddress(m_nIdxTexture);
	pDevice->SetTexture(0, pTexture);

	//�|���S���`��
	pDevice->DrawIndexedPrimitive (D3DPT_TRIANGLESTRIP, 0, 0, m_MeshField.nNumVtx, 0, m_MeshField.nNumIdx - 2);
}

//=====================================================
// �Ǎ�����
//=====================================================
void CMeshField::Load(std::string path)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//�|�C���^�錾
	FILE *pFile;

	//�t�@�C�����J��
	pFile = fopen((const char*)&path[0], "rb");

	if (pFile != nullptr)
	{
		fread(&m_nDivNumU, sizeof(int), 1, pFile);
		fread(&m_nDivNumV, sizeof(int), 1, pFile);
		fread(&m_nDivTex, sizeof(int), 1, pFile);
		fread(&m_fLengthMesh, sizeof(float), 1, pFile);

		// ���_���̃|�C���^
		VERTEX_3D *pVtx;

		m_MeshField.nNumVtx = (m_nDivNumU + 1) * (m_nDivNumV + 1);

		if (m_pVtxBuff == nullptr)
		{
			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshField.nNumVtx * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				nullptr);
		}

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�̈ʒu����ۑ�
		fread(&pVtx[0].pos, sizeof(D3DXVECTOR3), m_MeshField.nNumVtx * 4, pFile);

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		// �t�@�C�������
		fclose(pFile);
	}

}

//=====================================================
// �ۑ�����
//=====================================================
void CMeshField::Save(void)
{
	//�|�C���^�錾
	FILE *pFile;

	//�t�@�C�����J��
	pFile = fopen("data\\MAP\\field00.bin", "wb");

	if (pFile != nullptr)
	{
		fwrite(&m_nDivNumU, sizeof(int), 1, pFile);
		fwrite(&m_nDivNumV, sizeof(int), 1, pFile);
		fwrite(&m_nDivTex, sizeof(int), 1, pFile);
		fwrite(&m_fLengthMesh, sizeof(float), 1, pFile);

		// ���_���̃|�C���^
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�̈ʒu����ۑ�
		fwrite(&pVtx[0].pos, sizeof(D3DXVECTOR3),(m_nDivNumU + 1) * (m_nDivNumV + 1) * 4, pFile);

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		// �t�@�C�������
		fclose(pFile);
	}
}
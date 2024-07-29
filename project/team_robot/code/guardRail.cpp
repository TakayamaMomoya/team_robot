//*****************************************************
//
// �K�[�h���[���̏���[guardRail.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "guardRail.h"
#include "texture.h"
#include "effect3D.h"
#include "renderer.h"
#include "MyEffekseer.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* TEX_PATH = "data\\TEXTURE\\MATERIAL\\wall00.jpeg";	// �e�N�X�`���p�X
const int NUM_VTX_ON_POINT = 2;	// ���b�V�����[�h�̒��_��ɂ��钸�_��
const float HEIGHT_GR = 600.0f;	// �K�[�h���[���̍���
const int NUMVTX_NOTDRAW = 4;	// ���̒��_�������̏ꍇ�A�`�悵�Ȃ�
const float DECELERATION_MAG = 0.985f;	// �Փˎ��̌����{��
const float MOVE_MAG = 0.15f;	// �Փˎ��̈ړ��ʔ��˔{��
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CGuardRail::CGuardRail(int nPriority) : CPolygon3D(nPriority), m_nNumVtx(0), m_fHeight(0.0f), m_bLeft(false)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CGuardRail::~CGuardRail()
{

}

//=====================================================
// ��������
//=====================================================
CGuardRail *CGuardRail::Create(std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart, std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd, bool bLeft, float fHeight)
{
	CGuardRail *pGuardRail = nullptr;

	pGuardRail = new CGuardRail;

	if (pGuardRail != nullptr)
	{
		pGuardRail->m_fHeight = fHeight;
		pGuardRail->m_bLeft = bLeft;
		pGuardRail->m_itStart = itStart;
		pGuardRail->m_itEnd = itEnd;

		pGuardRail->Init();
	}

	return pGuardRail;
}

//=====================================================
// ����������
//=====================================================
HRESULT CGuardRail::Init(void)
{
	// ���_����
	CreateVtx();

	// ���_�𓹂ɉ��킹��
	VtxFollowRoad();

#ifdef _DEBUG
	//EnableWire(true);
#endif // _DEBUG

	return S_OK;
}

//=====================================================
// ���_�̐���
//=====================================================
void CGuardRail::CreateVtx(void)
{
	int nDistIt = std::distance(m_itStart, m_itEnd);
	m_nNumVtx = nDistIt * NUM_VTX_ON_POINT * MeshRoad::NUM_EDGE_IN_ROADPOINT;

	// ���_����
	CreateVtxBuff(m_nNumVtx);

	// �e�N�X�`���̓Ǎ�
	CTexture *pTexture = CTexture::GetInstance();

	if (pTexture != nullptr)
	{
		int nIdx = pTexture->Regist(TEX_PATH);
		SetIdxTexture(nIdx);
	}
}

//=====================================================
// ���_�𓹂ɉ��킹��
//=====================================================
void CGuardRail::VtxFollowRoad(void)
{
	// �J�n�ƏI���܂ł̒��_�ʒu��ۑ�============================
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	std::vector<D3DXVECTOR3> vectorPosEdge;

	if (pMesh != nullptr)
	{
		// �J�n�n�_���Z�o
		std::vector<CMeshRoad::SInfoRoadPoint> *pList = pMesh->GetArrayRP();

		int nDistStart = std::distance(pList->begin(), m_itStart) + 1;

		LPDIRECT3DVERTEXBUFFER9 pVtxBuffRoad = pMesh->GetVtxBuff();

		// ���_���̃|�C���^
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pVtxBuffRoad->Lock(0, 0, (void**)&pVtx, 0);

		// �J�n�n�_�܂Ń|�C���^��i�߂�
		pVtx += nDistStart * MeshRoad::NUM_EDGE_IN_ROADPOINT * MeshRoad::NUM_VTX_IN_EDGE;

		// ���_���̌v�Z
		int nDistBetween = std::distance(m_itStart, m_itEnd);
		nDistBetween *= MeshRoad::NUM_EDGE_IN_ROADPOINT;

		for (int i = 0; i < nDistBetween; i++)
		{
			CEffect3D::Create(pVtx[m_bLeft].pos, 20.0f, 3, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

			vectorPosEdge.push_back(pVtx[m_bLeft].pos);

			pVtx += 2;
		}

		// ���_�o�b�t�@���A�����b�N
		pVtxBuffRoad->Unlock();
	}
	else
	{
		return;
	}

	// ���_�𓹂ɉ��킹��========================================
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (size_t i = 0; i < vectorPosEdge.size(); i++)
	{
		// �ʒu�̌���
		pVtx[0].pos = vectorPosEdge[i];
		pVtx[1].pos = pVtx[0].pos;

		pVtx[1].pos.y += m_fHeight;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �@���̐ݒ�
		if (i > 0)
		{
			SetNormal(pVtx);
		}

		if (i == 1)
		{
			pVtx[-2].nor = pVtx[0].nor;
			pVtx[-1].nor = pVtx[1].nor;
		}

		// �e�N�X�`�����W�̐ݒ�
		if (i % 2 == 0)
		{
			pVtx[0].tex = { 0.0f,0.0f };
			pVtx[1].tex = { 1.0f,0.0f };
		}
		else
		{
			pVtx[0].tex = { 0.0f,1.0f };
			pVtx[1].tex = { 1.0f,1.0f };
		}

		pVtx += NUM_VTX_ON_POINT;
	}

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();
}

//=====================================================
// �@���̐ݒ�
//=====================================================
void CGuardRail::SetNormal(VERTEX_3D *pVtx)
{
	if (pVtx == nullptr)
		return;

	// ���_�ʒu
	D3DXVECTOR3 vtxLu = pVtx[-MeshRoad::NUM_VTX_IN_EDGE].pos;
	D3DXVECTOR3 vtxRu = pVtx[0].pos;
	D3DXVECTOR3 vtxRd = pVtx[1].pos;

	// ���_�ǂ����̍����x�N�g������ӂ��Z�o
	D3DXVECTOR3 edge1 = vtxLu - vtxRu;
	D3DXVECTOR3 edge2 = vtxRd - vtxRu;

	// ��ӂ̊O�ς���@�����Z�o
	D3DXVECTOR3 nor;
	D3DXVec3Cross(&nor, &edge1, &edge2);

	D3DXVec3Normalize(&nor, &nor);	// �@���𐳋K��

	// �@����K�p
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
}

//=====================================================
// �I������
//=====================================================
void CGuardRail::Uninit(void)
{
	CPolygon3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CGuardRail::Update(void)
{
	CPolygon3D::Update();
}

//=====================================================
// �`�揈��
//=====================================================
void CGuardRail::Draw(void)
{
	if (m_nNumVtx < NUMVTX_NOTDRAW)	// �`�搧��
		return;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX *pMtx = GetMatrix();

	// ���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(pMtx);

	D3DXMATRIX mtxRot, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);

	// ���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, pMtx);

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	int nIdxTexture = GetIdxTexture();
	LPDIRECT3DTEXTURE9 pTexture = Texture::GetTexture(nIdxTexture);
	pDevice->SetTexture(0, pTexture);

	// �J�����O�𖳌���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

	// �J�����O��L����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=====================================================
// �����蔻�菈��
//=====================================================
bool CGuardRail::CollideGuardRail(D3DXVECTOR3* pos, D3DXVECTOR3* move, D3DXVECTOR3 vecAxial, float* fSpeed)
{
	bool bCollision = false;

	// ���_�o�b�t�@�擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̌v�Z
	int size = CMeshRoad::GetInstance()->GetArrayRP()->size() * MeshRoad::NUM_EDGE_IN_ROADPOINT;
	for (int i = 0; i < m_nNumVtx - 2; i += 2)
	{
		if(i != 0)
			pVtx += NUM_VTX_ON_POINT;

		// �@���̑傫����0�̎��͔��肵�Ȃ�
		if (D3DXVec3Length(&pVtx[0].nor) == 0.0f)
			continue;

		// �K�[�h���[���̍����ȓ��Ŕ��肷��
		if (m_fHeight < pos->y - pVtx[0].pos.y || pos->y < pVtx[0].pos.y - m_fHeight)
			continue;

		// �K�[�h���[���͈͓̔��ɑ��݂���
		if (D3DXVec3Dot(&(*pos - pVtx[0].pos), &(pVtx[2].pos - pVtx[0].pos)) < 0.0f ||
			D3DXVec3Dot(&(*pos - pVtx[2].pos), &(pVtx[0].pos - pVtx[2].pos)) < 0.0f)
			continue;

		// �����߂��ꂽ���W���v�Z����
		D3DXVECTOR3 vecReturn;
		if (m_bLeft)
			vecReturn = universal::CollideOBBToPlane(pos, vecAxial, pVtx[0].pos, pVtx[2].nor);
		else
			vecReturn = universal::CollideOBBToPlane(pos, vecAxial, pVtx[0].pos, -pVtx[2].nor);

		if (D3DXVec3Length(&vecReturn) == 0.0f)
			continue;

		// �v�Z���������������߂�
		*pos += vecReturn;

		// �����߂���������ړ������Ƌ������v�Z����
		D3DXVec3Normalize(&vecReturn, &vecReturn);
		*move -= vecReturn * D3DXVec3Dot(&vecReturn, move) * MOVE_MAG;
		*fSpeed *= DECELERATION_MAG;
		bCollision = true;

		// �Փ˃G�t�F�N�g���o���ʒu���v�Z����
		D3DXVECTOR3 vecWall = pVtx[2].pos - pVtx[0].pos;
		D3DXVec3Normalize(&vecWall, &vecWall);
		D3DXVECTOR3 posEffect = pVtx[0].pos + (vecWall * D3DXVec3Dot(&(*pos - pVtx[0].pos), &vecWall));

		// �Փ˃G�t�F�N�g���o���p�x���v�Z����
		float rotEffect;
		if (m_bLeft)
			rotEffect = atan2f(-pVtx[2].nor.x, -pVtx[2].nor.z);
		else
			rotEffect = atan2f(pVtx[2].nor.x, pVtx[2].nor.z);

		// �G�t�F�N�g�̍Đ�
		MyEffekseer::CreateEffect(CEffekseer::TYPE_SPARK, D3DXVECTOR3(posEffect.x, posEffect.y + 100.0f, posEffect.z), D3DXVECTOR3(0.0f, rotEffect, 0.0f));

		break;
	}

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	return bCollision;
}
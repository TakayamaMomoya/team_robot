//*****************************************************
//
// ���b�V�����̏���[meshRoad.h]
// Author:���R����
//
//*****************************************************

#ifndef _MESHROAD_H_
#define _MESHROAD_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "polygon3D.h"
#include <vector>

//*****************************************************
// �O����`
//*****************************************************
class CCutMullSpline;
class CTunnel;
class CGuardRail;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMeshRoad : public CPolygon3D
{
public:
	struct SInfoRoadPoint
	{// �ӂɕK�v�ȏ��
		D3DXVECTOR3 pos;
		float fWidth;
		SInfoRoadPoint() : pos({ 0.0f,0.0f,0.0f }) {}
	};

	CMeshRoad(int nPriority = 3);	// �R���X�g���N�^
	~CMeshRoad();	// �f�X�g���N�^

	static CMeshRoad *Create(const char* pPath);
	static CMeshRoad *GetInstance(void) { return m_pMeshRoad; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddRoadPoint(D3DXVECTOR3 pos,bool bReCreateVtx = false);
	void CreateVtxBuffEdge(void);	// �G�b�W���ɉ����Ē��_�𐶐�����

	bool CollideRoad(D3DXVECTOR3* pPos, D3DXVECTOR3 posOld);

	void Save(const char* pPath);	// �G�f�B�^�[�̂݁B�Q�[���ł͏����Ă�
	void Load(const char* pPath);	// �G�f�B�^�[�̂݁B�Q�[���ł͏����Ă�

	std::vector<CMeshRoad::SInfoRoadPoint>::iterator SelectRoadPoint(void);
	std::vector<SInfoRoadPoint> *GetArrayRP(void) { return &m_aRoadPoint; }
	std::vector<CTunnel*> *GetArrayTunnnel(void) { return &m_aTunnel; }
	std::vector<CGuardRail*> *GetArrayGR(void) { return &m_aGR; }
	size_t GetIdxRoad(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it);
	void DeleteRoadPoint(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it);
	void ResetIterator(void);
	void AjustRoadEvent(void);	// ���C�x���g�̒���
	CCutMullSpline *GetCenterSpline(void);
	void DeleteAllRoadEvent(void);	// �S���[�h�C�x���g�̍폜

	// �ÓI�����o�֐�
	static std::list<CMeshRoad*> GetArray(void) { return s_aRoad; }	// �R���e�i�̎擾
	static void SelectMeshRoad(void);	// ���b�V�����[�h�̑I��

private:
	void SetNormal(VERTEX_3D *pVtx);	// �@���̐ݒ�
	void CreateSpline(void);	// �X�v���C���̐���
	void CreateSideSpline(void);	// ���ʂ̃X�v���C������
	void CreateVtxBetweenRoadPoint(SInfoRoadPoint infoRoadPoint, VERTEX_3D *pVtx, SInfoRoadPoint *infoRoadPointOld = nullptr, int nIdx = 0);	// ���[�h�|�C���g�Ԃ̒��_�̐ݒ�
	void SetEdgeAngle(VERTEX_3D *pVtx, D3DXVECTOR3 posEdge, D3DXVECTOR3 posEdgeOld);	// �ӂ̊p�x�̐ݒ�
	D3DXVECTOR3 GetPosEdge(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2);

	UINT m_nNumVtx;
	std::vector<SInfoRoadPoint> m_aRoadPoint;
	std::vector<SInfoRoadPoint>::iterator m_it;
	CCutMullSpline *m_pSpline;	// �X�v���C��
	CCutMullSpline *m_pSplineL;	// �����̃X�v���C��
	CCutMullSpline *m_pSplineR;	// �E���̃X�v���C��
	std::vector<CTunnel*> m_aTunnel;	// �g���l���̃|�C���^�̔z��
	std::vector<CGuardRail*> m_aGR;	// �K�[�h���[���̃|�C���^�̔z��

	static CMeshRoad *m_pMeshRoad;
	static std::list<CMeshRoad*> s_aRoad;	// ���b�V�����[�h�̃R���e�i
};

namespace MeshRoad
{
const int NUM_EDGE_IN_ROADPOINT = 10;	// ���[�h�|�C���g��ɂ��A����ӂ̐�
const int NUM_VTX_IN_EDGE = 2;	// ��ӂɂ��钸�_��

CMeshRoad *GetInstance(void);
}

#endif
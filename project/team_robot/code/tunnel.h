//*****************************************************
//
// �g���l���̏���[tunnel.h]
// Author:���R����
//
//*****************************************************
#ifndef _TUNNEL_H_	// ��d�C���N���[�h�h�~
#define _TUNNEL_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "meshCylinder.h"
#include "meshRoad.h"

//*****************************************************
// �O���錾
//*****************************************************
class CFan3D;

//*****************************************************
// �N���X��`
//*****************************************************
class CTunnel : public CMeshCylinder
{
public:
	CTunnel(int nPriority = 5);	// �R���X�g���N�^
	~CTunnel();	// �f�X�g���N�^

	// �����o�ϐ�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ϐ��擾�E�ݒ�֐�
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator GetItStart(void) { return m_itStart; }
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator GetItEnd(void) { return m_itEnd; }
	void SetStart(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it) { m_itStart = it; }
	void SetEnd(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it) { m_itEnd = it; }

	// �ÓI�����o�֐�
	static CTunnel *Create(std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart, std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd);

private:
	// �����o�֐�
	void CreateVtx(void);	// ���_�̐���
	void VtxFollowRoad(void);	// ���_�𓹂ɉ��킹��
	void CreateFan(void);	// ��|���S���̐���
	void DetectionPlayer(void);	// �v���C���[�̌��m
	void CollidePlayerEnter(VERTEX_3D *pVtx);	// �v���C���[�����Ă��锻��
	void CollidePlayerExit(VERTEX_3D *pVtx);	// �v���C���[�o�Ă�������
	void EnterPlayer(void);	// �v���C���[���������Ƃ��̏���
	void ExitPlayer(void);	// �v���C���[���o���Ƃ��̏���
	void StayPlayer(void);	// �v���C���[�����ɂ���Ƃ��̏���

	// �����o�ϐ�
	bool m_bInPlayer;	// �v���C���[�������Ă��邩�ǂ���
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itStart;	// �J�n�C�e���C�^�[
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itEnd;	// �I���C�e���C�^�[
	CFan3D *m_pFanEnter;	// �����̐�|���S��
};
#endif
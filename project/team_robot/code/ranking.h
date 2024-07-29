//*****************************************************
//
// �����L���O�̏���[ranking.h]
// Author:���R����
//
//*****************************************************
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "scene.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace Result
{
const int BASE_REWARD = 100000;	// ��{��V
}

//*****************************************************
// �O���錾
//*****************************************************
class CMeshCylinder;
class CResultBehavior;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CRanking : public CScene
{
public:
	struct SInfoRanking
	{// �����L���O�̏��
		int nRank;	// �����N
		std::string name;	// ���O
		int nReward;	// ��V
	};

	CRanking();	// �R���X�g���N�^
	~CRanking();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeBehavior(CResultBehavior *pBehavior);
	std::list<SInfoRanking> GetListRanking(void) { return m_listRanking; }

private:
	void LoadRanking(void);

	std::list<SInfoRanking> m_listRanking;	// �����L���O�̃��X�g
	CResultBehavior *m_pBehavior;
	CMeshCylinder *m_pCylinder;	// �w�i�̃V�����_�[
};

#endif
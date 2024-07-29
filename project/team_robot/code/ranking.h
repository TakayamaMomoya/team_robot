//*****************************************************
//
// ランキングの処理[ranking.h]
// Author:高山桃也
//
//*****************************************************
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************
// インクルード
//*****************************************************
#include "scene.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace Result
{
const int BASE_REWARD = 100000;	// 基本報酬
}

//*****************************************************
// 前方宣言
//*****************************************************
class CMeshCylinder;
class CResultBehavior;

//*****************************************************
// クラスの定義
//*****************************************************
class CRanking : public CScene
{
public:
	struct SInfoRanking
	{// ランキングの情報
		int nRank;	// ランク
		std::string name;	// 名前
		int nReward;	// 報酬
	};

	CRanking();	// コンストラクタ
	~CRanking();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeBehavior(CResultBehavior *pBehavior);
	std::list<SInfoRanking> GetListRanking(void) { return m_listRanking; }

private:
	void LoadRanking(void);

	std::list<SInfoRanking> m_listRanking;	// ランキングのリスト
	CResultBehavior *m_pBehavior;
	CMeshCylinder *m_pCylinder;	// 背景のシリンダー
};

#endif
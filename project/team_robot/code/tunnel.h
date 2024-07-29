//*****************************************************
//
// トンネルの処理[tunnel.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _TUNNEL_H_	// 二重インクルード防止
#define _TUNNEL_H_

//*****************************************************
// インクルード
//*****************************************************
#include "meshCylinder.h"
#include "meshRoad.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CFan3D;

//*****************************************************
// クラス定義
//*****************************************************
class CTunnel : public CMeshCylinder
{
public:
	CTunnel(int nPriority = 5);	// コンストラクタ
	~CTunnel();	// デストラクタ

	// メンバ変数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 変数取得・設定関数
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator GetItStart(void) { return m_itStart; }
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator GetItEnd(void) { return m_itEnd; }
	void SetStart(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it) { m_itStart = it; }
	void SetEnd(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it) { m_itEnd = it; }

	// 静的メンバ関数
	static CTunnel *Create(std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart, std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd);

private:
	// メンバ関数
	void CreateVtx(void);	// 頂点の生成
	void VtxFollowRoad(void);	// 頂点を道に沿わせる
	void CreateFan(void);	// 扇ポリゴンの生成
	void DetectionPlayer(void);	// プレイヤーの検知
	void CollidePlayerEnter(VERTEX_3D *pVtx);	// プレイヤー入ってくる判定
	void CollidePlayerExit(VERTEX_3D *pVtx);	// プレイヤー出ていく判定
	void EnterPlayer(void);	// プレイヤーが入ったときの処理
	void ExitPlayer(void);	// プレイヤーが出たときの処理
	void StayPlayer(void);	// プレイヤーが内にいるときの処理

	// メンバ変数
	bool m_bInPlayer;	// プレイヤーが入っているかどうか
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itStart;	// 開始イテレイター
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itEnd;	// 終了イテレイター
	CFan3D *m_pFanEnter;	// 入口の扇ポリゴン
};
#endif
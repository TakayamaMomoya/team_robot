//*****************************************************
//
// メッシュエディターの処理[editMesh.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "edit.h"
#include "meshRoad.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPolygon3D;
class CObjectX;
class CStateEditMesh;
class CEnemySpawner;

//*****************************************************
// クラスの定義
//*****************************************************
class CEditMesh : public CEdit
{
public:
	CEditMesh();
	~CEditMesh();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void ChangeState(CStateEditMesh *pState);
	const char* GetSavePath(void) { return &m_aPathSave[0]; }
	const char* GetSavePathEnemy(void) { return &m_aPathSaveEnemy[0]; }
	void SaveEnemy(const char* pPath);
	void LoadEnemy(const char* pPath);
	void AddSpawner(CEnemySpawner* pSpawner) { m_listEnemySpawner.push_back(pSpawner); }

private:
	// メンバ関数
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator SelectIterator(void);	// イテレーターの選択
	void MoveCurrentRP(void);	// 選択ロードポイントの移動
	void CollideRPRay(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it, D3DXVECTOR3 posFar, D3DXVECTOR3 posNear, D3DXVECTOR3 vecDiff);	// ロードポイントとレイの判定

	// メンバ変数
	CPolygon3D *m_pMesh;	// メッシュのポインタ
	CObjectX *m_pManipulator;	// マニピュレーター
	CStateEditMesh *m_pState;	// ステイト
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itCurrent;	// 選択してるイテレイター
	bool m_bCurrent;	// 選択フラグ
	D3DXVECTOR3 m_posCurrent;	// 選択ブロック位置
	char m_aPathSave[256] = {};	// 保存パス
	char m_aPathSaveEnemy[256] = {};	// 敵保存パス
	std::list<CEnemySpawner*> m_listEnemySpawner;	// 敵スポナーのリスト
};

//*****************************************************
// ステイトクラスの定義
//*****************************************************
class CStateEditMesh
{// メッシュエディットのステイト基底クラス
public:
	CStateEditMesh() {};
	virtual ~CStateEditMesh() {};

	virtual void Update(CEditMesh *pEdit) = 0;
};

class CStateEditMeshCreateMesh : public CStateEditMesh
{// メッシュの生成
public:
	CStateEditMeshCreateMesh() {};
	virtual ~CStateEditMeshCreateMesh() {};

	void Update(CEditMesh *pEdit) override;

private:
};

class CStateEditMeshCreateTunnel : public CStateEditMesh
{// トンネルの生成
public:
	CStateEditMeshCreateTunnel();	// コンストラクタ
	virtual ~CStateEditMeshCreateTunnel() {};	// デストラクタ

	// メンバ関数
	void Update(CEditMesh *pEdit) override;	// 更新処理

private:
	// メンバ関数
	void SetTunnel(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it);

	// メンバ変数
	bool m_bEnd;	// スタートの選択かどうか
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itStart;	// トンネルの開始イテレイター
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itEnd;	// トンネルの終了イテレイター
};

class CStateEditMeshCreateGR : public CStateEditMesh
{// ガードレールの生成
public:
	CStateEditMeshCreateGR();	// コンストラクタ
	virtual ~CStateEditMeshCreateGR() {};	// デストラクタ

	// メンバ関数
	void Update(CEditMesh *pEdit) override;	// 更新処理

private:
	// メンバ関数
	void SetGR(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it);

	// メンバ変数
	bool m_bEnd;	// スタートの選択かどうか
	float m_fHeight;	// 高さ
	float m_bLeft;	// 左側にするかどうか
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itStart;	// 開始イテレイター
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itEnd;	// 終了イテレイター
};

class CStateEditMeshSelectGR : public CStateEditMesh
{// ガードレールの選択
public:
	CStateEditMeshSelectGR();	// コンストラクタ
	virtual ~CStateEditMeshSelectGR() {};	// デストラクタ

	// メンバ関数
	void Update(CEditMesh *pEdit) override;	// 更新処理

private:
	// メンバ変数
	std::vector<CGuardRail*>::iterator m_it;	// 選択イテレーター
};

class CStateEditMeshAdjustRoadPoint : public CStateEditMesh
{// ロードポイントの調節
public:
	CStateEditMeshAdjustRoadPoint();
	virtual ~CStateEditMeshAdjustRoadPoint() {};

	void Update(CEditMesh *pEdit) override;
};

class CStateEditMeshDeleteRoadPoint : public CStateEditMesh
{// ロードポイントの削除
public:
	CStateEditMeshDeleteRoadPoint();
	virtual ~CStateEditMeshDeleteRoadPoint() {};

	void Update(CEditMesh *pEdit) override;
};
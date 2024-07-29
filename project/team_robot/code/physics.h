//*****************************************************
//
// 物理[Myphysics.h]
// Author 大原怜将
//
//*****************************************************
#ifndef _MYPHYSICS_H_             //このマクロ定義がされてなかったら
#define _MYPHYSICS_H_             //2重インクルード防止のマクロ定義をする

//*****************************************************
// インクルード
//*****************************************************
#include "btBulletDynamicsCommon.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObjectX;

//*****************************************************
// クラス定義
//*****************************************************
class CPhysics
{
public:
	CPhysics();
	~CPhysics();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	btDiscreteDynamicsWorld* GetDynamicsWorld(void) { return m_pDynamicsWorld; }

private:

	btDiscreteDynamicsWorld* m_pDynamicsWorld;  //ダイナミクスワールド
	btAxisSweep3* m_pBroadphase;  // 
	btDefaultCollisionConfiguration* m_pCollisionConfiguration;  // デフォルトの衝突判定
	btCollisionDispatcher* m_pDispatcher;  // ディスパッチ
	btSequentialImpulseConstraintSolver* m_pSolver;  // 衝突解決ソルバ
	int allFrame;
};

#endif
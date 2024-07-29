//*****************************************************
//
// ブロックの処理[block.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "ObjectX.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CFan3D;
class CPolygon3D;

//*****************************************************
// クラスの定義
//*****************************************************
//=====================================================
// 通常ブロッククラス
//=====================================================
class CBlock : public CObjectX
{
public:
	typedef enum
	{// 種類
		TYPE_WALL = 0,	// 壁
		TYPE_MAX
	}TYPE;

	typedef enum
	{// 行動
		BEHAVIOUR_NORMAL = 0,	// 通常
		BEHAVIOUR_GRAB,	// 掴めるもの
		BEHAVIOUR_MAX
	}BEHAVIOUR;

	typedef struct
	{// 保存するときの情報
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		TYPE type;
	}MemBlock;

	CBlock(int nPriority = 3);	// コンストラクタ
	~CBlock();	// デストラクタ

	static CBlock *Create(BEHAVIOUR behaviour = BEHAVIOUR_NORMAL);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int GetIdx(void) { return m_nIdx; }
	void SetIdx(int nIdx) { m_nIdx = nIdx; }
	CBlock *GetNext(void) { return m_pNext; }
	CBlock *GetPrev(void) { return m_pPrev; }
	static int GetNumAll(void) { return m_nNumAll; }
	void SetPosition(D3DXVECTOR3 pos);
	virtual void Save(FILE *pFile);	// 保存処理
	virtual void Load(FILE *pFile, char* pTemp);	// 読込処理

	bool Collide(D3DXVECTOR3* pPos, D3DXVECTOR3 posOld);

private:
	static int m_nNumAll;	// 総数
	float m_fLife;	// 体力
	int m_nIdx;	// 種類のインデックス

	CBlock *m_pPrev;	// 前のアドレス
	CBlock *m_pNext;	// 次のアドレス
};

//=====================================================
// 掴むブロッククラス
//=====================================================
class CBlockGrab : public CBlock
{
public:
	CBlockGrab();	// コンストラクタ
	~CBlockGrab();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool CanGrab(D3DXVECTOR3 pos);
	void Save(FILE *pFile) override;	// 保存処理
	void Load(FILE *pFile, char* pTemp) override;	// 読込処理
	void SetFan(void);	// 扇ポリゴンの設定

	// 変数取得・設定
	void SetPosition(D3DXVECTOR3 pos);	// 位置の設定
	void SetRotation(D3DXVECTOR3 rot);	// 向きの設定
	void EnableCurrent(bool bCurrent) { m_bCurrent = bCurrent; }	// 選択フラグ
	bool IsCurrent(void) { return m_bCurrent; }
	float GetRadiusOffset(void) { return m_fRadiusOffset; }	// オフセット半径
	void SetRadiusOffset(float fRadius) { m_fRadiusOffset = fRadius; }
	float GetAngleOffset(int nIdx) { return m_afAngleOffset[nIdx]; }	// オフセット角度
	void SetAngleOffset(float fAngle, int nIdx) { m_afAngleOffset[nIdx] = fAngle; }

private:

	// メンバ関数
	void GrabPointUI(void);  // 鉤縄ポイントUI表示

	// 定数
	static const int NUM_OFFSET = 2;	// オフセットの数

	// メンバ変数
	float m_afAngleOffset[NUM_OFFSET];	// オフセットの角度
	float m_fRadiusOffset;	// オフセットの半径
	bool m_bCurrent;	// 選択されているかどうか
	CFan3D *m_pFan;	// 判定可視化用の扇ポリゴン
	CPolygon3D *m_pGrabPointUI;  // 鉤縄ポイントUI表示用ポリゴン
	float m_fColA = 0.0f;  // 鉤縄ポイントUI表示用ポリゴンのα値

};

#endif

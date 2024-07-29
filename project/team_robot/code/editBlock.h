//*****************************************************
//
// ブロックエディターの処理[editBlock.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "block.h"
#include "edit.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObjectX;
class CStateEditBlock;
class CPolygon3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CEditBlock : public CEdit
{
public:
	CEditBlock();	// コンストラクタ
	~CEditBlock();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Save(void);
	void ChangeState(CStateEditBlock *pState);
	void AddBlockToMap(CBlock *pBlock);
	void SetMoveBlock(CBlock *pBlock) { m_pMoveBlock = pBlock; }

private:
	void CreateBlockIconAll(void);
	void DeleteAllIcon(void);
	void RaySelectBlock(void);
	void CollideBlockRay(CBlock *pBlock,D3DXVECTOR3 posFar, D3DXVECTOR3 posNear, D3DXVECTOR3 vecDiff);
	void MoveCurrentBlock(D3DXVECTOR3 posFar, D3DXVECTOR3 posNear);

	char m_aPath[256];
	CStateEditBlock *m_pState;	// ステイトのポインタ
	D3DXVECTOR3 m_posCurrent;
	CBlock *m_pCurrentBlock;	// 選択ブロック
	CBlock *m_pMoveBlock;	// 動かすブロック
	std::map<CBlock*, CPolygon3D*> m_aIcon;	// アイコンのポインタ
};

//*****************************************************
// ステイトクラスの定義
//*****************************************************
class CStateEditBlock
{// ブロックエディットのステイト基底クラス
public:
	CStateEditBlock() {};
	virtual ~CStateEditBlock() {};

	virtual void Init(CEditBlock *pEdit) = 0;
	virtual void Uninit(CEditBlock *pEdit) = 0;
	virtual void Update(CEditBlock *pEdit) = 0;
};

class CStateCreateBlockNormal : public CStateEditBlock
{// 通常ブロックの生成
public:
	CStateCreateBlockNormal() : m_pObjectCursor(nullptr), m_nIdxObject(-1), m_type(CBlock::TYPE::TYPE_WALL), m_behaviour(CBlock::BEHAVIOUR::BEHAVIOUR_NORMAL) {};
	virtual ~CStateCreateBlockNormal() {};

	void Init(CEditBlock *pEdit) override;
	void Uninit(CEditBlock *pEdit) override;
	void Update(CEditBlock *pEdit) override;

private:
	void ChangeBlockBehaviour(void);
	void CreateBlock(D3DXVECTOR3 pos, CEditBlock *pEdit);
	CBlock *CheckDelete(void);
	void SelectBlock(void);

	int m_nIdxObject;
	CBlock::TYPE m_type;
	CObjectX *m_pObjectCursor;	// カーソルの仮オブジェクト
	CBlock::BEHAVIOUR m_behaviour;	// ブロックのビヘイビア
	CBlock *m_pBlockCurrent;	// 選択しているブロックのポインタ
};

class CStateEditGrabBlock : public CStateEditBlock
{// 掴むブロックの編集
public:
	CStateEditGrabBlock() {};
	virtual ~CStateEditGrabBlock() {};

	// メンバ関数
	void Init(CEditBlock *pEdit) override;
	void Uninit(CEditBlock *pEdit) override;
	void Update(CEditBlock *pEdit) override;

private:
	// メンバ関数
	void SelectGrabBlock(void);
	void EditGrabBlock(void);

	// メンバ変数
	std::list<CBlockGrab*>::iterator m_it;	// イテレーター
};
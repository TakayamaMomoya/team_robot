//*****************************************************
//
// メッシュフィールドエディターの処理[editMeshfield.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "edit.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMeshCylinder;

//*****************************************************
// クラスの定義
//*****************************************************
class CEditMeshfield : public CEdit
{
public:
	CEditMeshfield();	// コンストラクタ
	~CEditMeshfield();	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;

private:
	// メンバ関数
	void MoveCursor(void);	// カーソルを動かす処理
	void DentMeshField(void);	// メッシュフィールドをへこます処理

	// メンバ変数
	float m_fDepthDent;	// 1フレーム間にへこます深さ
	float m_fRadiusDent;	// へこます範囲
	CMeshCylinder *m_pMeshCylinder;	// 範囲表示のメッシュシリンダー
};
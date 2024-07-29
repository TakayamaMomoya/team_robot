//*****************************************************
//
// エディターの処理[edit.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _EDIT_H_
#define _EDIT_H_

//*****************************************************
// クラスの定義
//*****************************************************
class CEdit
{
public:
	CEdit();	// コンストラクタ
	~CEdit();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void TeleportPlayer(void);

private:
	// メンバ関数

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
};

#endif
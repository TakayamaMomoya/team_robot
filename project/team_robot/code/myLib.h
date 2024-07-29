//*****************************************************
//
// オリジナルライブラリ[myLib.h.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MYLIB_H_
#define _MYLIB_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CSpline
{// 自然スプライン
public:
    CSpline() {};
    void Init(const std::vector<double>& x, const std::vector<double>& y);

    double Interpolate(double xi,int nIdx = -1);

private:
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> d;
};

class CCutMullSpline
{// カットマル・スプライン
public:
    CCutMullSpline() {};    // コンストラクタ
    ~CCutMullSpline();    // デストラクタ

    // メンバ関数
    void Init(const std::vector<D3DXVECTOR3> vPos); // 初期化処理
    D3DXVECTOR3 Interpolate(float t, int nIdx);   // 補間処理
    bool IsEmpty(void) { return m_vPos.size() == 0; }; // 空かどうかの判定
    float GetLength(int nIdx,int nDiv);   // 曲線の長さの取得

private:
    // メンバ変数
    std::vector<D3DXVECTOR3> m_vPos;    // 点の配列
};
#endif
//*****************************************
//
// いろいろ汎用処理詰め込み
// Author:髙山桃也
//
//*****************************************

//*****************************************
// インクルード
//*****************************************
#include "manager.h"
#include "renderer.h"
#include "universal.h"
#include "debugproc.h"

namespace universal
{
//=========================================
// float値の制限
//=========================================
void LimitValuefloat(float *pValue, float fMax, float fMin)
{
	if (fMax < *pValue)
	{
		*pValue = fMax;
	}

	if (fMin > *pValue)
	{
		*pValue = fMin;
	}
}

//=========================================
// int値の制限
//=========================================
void LimitValueInt(int *pValue, int nMax, int nMin)
{
	if (nMax < *pValue)
	{
		*pValue = nMax;
	}

	if (nMin > *pValue)
	{
		*pValue = nMin;
	}
}

//=========================================
// 回転制御処理
//=========================================
void LimitRot(float *fRot)
{
	if (*fRot > D3DX_PI)
	{
		*fRot -= 6.28f;
	}
	if (*fRot < -D3DX_PI)
	{
		*fRot += 6.28f;
	}
}

//=========================================
// 速度の制限
//=========================================
void LimitSpeed(D3DXVECTOR3 *pVec, float fSpeedMax)
{
	if (pVec == nullptr)
	{
		return;
	}

	// 速度の取得
	float fSpeed = D3DXVec3Length(pVec);

	if (fSpeed > fSpeedMax)
	{// 速度の制限
		D3DXVECTOR3 vecNew = *pVec;

		D3DXVec3Normalize(&vecNew, &vecNew);

		vecNew *= fSpeedMax;

		*pVec = vecNew;
	}
}

//========================================
// 矩形内の位置制限
//========================================
void LimitPosInSq(float fWidth, float fHeight, D3DXVECTOR3 *pPos)
{
	if (pPos == nullptr)
		return;
	
	LimitValuefloat(&pPos->x, fWidth, -fWidth);
	LimitValuefloat(&pPos->z, fHeight, -fHeight);
}

//========================================
// 円柱の距離制限
//========================================
float LimitDistCylinder(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget)
{
	if (pPos == nullptr)
		return 0.0f;

	posTarget.y = pPos->y;

	D3DXVECTOR3 vecDiff = posTarget - *pPos;

	float fDistDiff = D3DXVec3Length(&vecDiff);

	if (fLength >= fDistDiff)
	{
		fDistDiff = fLength;

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= fLength;

		*pPos = posTarget - vecDiff;
	}

	return fDistDiff;
}

//========================================
// 球の距離制限（内側）
//========================================
float LimitDistSphereInSide(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget)
{
	if (pPos == nullptr)
		return 0.0f;

	D3DXVECTOR3 vecDiff = posTarget - *pPos;

	float fDistDiff = D3DXVec3Length(&vecDiff);

	if (fLength <= fDistDiff)
	{
		fDistDiff = fLength;

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= fLength;

		*pPos = posTarget - vecDiff;
	}

	return fDistDiff;
}

//========================================
// ホーミング
//========================================
void Horming(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, float fSpeedChase, D3DXVECTOR3 *pMove)
{
	if (pMove == nullptr)
		return;

	D3DXVECTOR3 vecDiff = posTarget - pos;

	VecConvertLength(&vecDiff, fSpeedChase);

	*pMove += vecDiff;
}

//========================================
// ベクトルを長さで補正する処理
//========================================
void VecConvertLength(D3DXVECTOR3 *pVec, float fLength)
{
	D3DXVec3Normalize(pVec, pVec);

	*pVec *= fLength;
}

//========================================
// 反対の相対座標を出す処理
//========================================
D3DXVECTOR3 RelativeInversPos(D3DXVECTOR3 pos, D3DXVECTOR3 posCenter, float fRate)
{
	D3DXVECTOR3 posInvers = { 0.0f,0.0f,0.0f };

	D3DXVECTOR3 vecDiff = posCenter - pos;

	vecDiff *= fRate;

	posInvers = posCenter + vecDiff;

	return posInvers;
}

//========================================
// 目標位置に向かう処理
//========================================
void MoveToDest(D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget, float fFact)
{
	if (pPos == nullptr)
		return;

	D3DXVECTOR3 vecDiff = posTarget - *pPos;

	*pPos += vecDiff * fFact;
}

//========================================
// オフセットへのベクトル
//========================================
D3DXVECTOR3 VecToOffset(D3DXMATRIX mtx, D3DXVECTOR3 posOffset)
{
	D3DXVECTOR3 pos =
	{
		mtx._41,
		mtx._42,
		mtx._43,
	};

	D3DXMATRIX mtxOffset;

	SetOffSet(&mtxOffset, mtx, posOffset);

	D3DXVECTOR3 offset =
	{
		mtxOffset._41,
		mtxOffset._42,
		mtxOffset._43,
	};

	D3DXVECTOR3 vecDiff = offset - pos;

	return vecDiff;
}

//========================================
// 極座標の計算
//========================================
D3DXVECTOR3 PolarCoordinates(D3DXVECTOR3 rot)
{
	D3DXVECTOR3 vec;

	vec =
	{
		sinf(rot.x) * sinf(rot.y),
		cosf(rot.x),
		sinf(rot.x) * cosf(rot.y),
	};

	return vec;
}

//========================================
// 3次元ベクトルの内積
//========================================
float Vec3Dot(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2)
{
	float fDot = 0.0f;

	fDot = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;

	return fDot;
}

//========================================
// レイと球の判定
//========================================
bool CalcRaySphere(D3DXVECTOR3 posStartRay, D3DXVECTOR3 vecRay, D3DXVECTOR3 posSphere, float fRadius, D3DXVECTOR3 *pPosStartHit, D3DXVECTOR3 *pPosEndHit)
{
	// 球の座標をレイの開始地点分戻す
	posSphere -= posStartRay;

	float A = vecRay.x * vecRay.x + vecRay.y * vecRay.y + vecRay.z * vecRay.z;
	float B = vecRay.x * posSphere.x + vecRay.y * posSphere.y + vecRay.z * posSphere.z;
	float C = posSphere.x * posSphere.x + posSphere.y * posSphere.y + posSphere.z * posSphere.z - fRadius * fRadius;

	if (A == 0.0f)
		return false; // レイの長さが0

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // 衝突していない

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
		return false; // レイの反対で衝突

	if (pPosStartHit != nullptr)
	{// 衝突しはじめたところ
		*pPosStartHit = posStartRay + a1 * vecRay;
	}

	if (pPosEndHit != nullptr)
	{// 衝突が終わったところ
		*pPosStartHit = posStartRay + a2 * vecRay;
	}

	return true;
}

//========================================
// レイと平面の判定
//========================================
bool CalcRayFlat(D3DXVECTOR3 posPlane, D3DXVECTOR3 nor, D3DXVECTOR3 srcRay, D3DXVECTOR3 endRay, D3DXVECTOR3 *pPosHit)
{
	D3DXVECTOR3 vecToSrc = srcRay - posPlane;
	D3DXVECTOR3 vecToEnd = endRay - posPlane;

	float fDotSrc = Vec3Dot(vecToSrc, nor);
	float fDotEnd = Vec3Dot(vecToEnd, nor);

	if (pPosHit != nullptr)
	{// 衝突した座標の設定
		D3DXVECTOR3 posHit;

		if (fDotEnd < 0)
			fDotEnd *= -1;
		if (fDotSrc < 0)
			fDotSrc *= -1;

		float fDotMax = fDotSrc + fDotEnd;
		float fRate = fDotSrc / fDotMax;

		D3DXVECTOR3 vecDiff = endRay - srcRay;
		posHit = srcRay + vecDiff * fRate;

		*pPosHit = posHit;
	}

	return fDotSrc * fDotEnd <= 0;
}

//========================================
// スクリーン座標をワールド座標に変換
//========================================
D3DXVECTOR3 CalcScreenToWorld(D3DXVECTOR3 posScreen)
{
	HWND hWnd = FindWindow(CLASS_NAME, WINDOW_NAME);

	RECT rect;
	GetClientRect(hWnd, &rect);
	float fWidthScreen = static_cast<float>(rect.right - rect.left);
	float fHeightScreen = static_cast<float>(rect.bottom - rect.top);

	// 各行列の逆行列を算出
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX InvView, InvPrj, VP, InvViewport, View, Prj;

	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Prj);

	D3DXMatrixInverse(&InvView, NULL, &View);
	D3DXMatrixInverse(&InvPrj, NULL, &Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = fWidthScreen / 2.0f; VP._22 = -fHeightScreen / 2.0f;
	VP._41 = fWidthScreen / 2.0f; VP._42 = fHeightScreen / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVECTOR3 pOut;
	D3DXVec3TransformCoord(&pOut, &D3DXVECTOR3(posScreen.x, posScreen.y, posScreen.z), &tmp);

	return pOut;
}

//========================================
// スクリーン座標を3Dに変換
//========================================
void ConvertScreenPosTo3D(D3DXVECTOR3 *pPosNear, D3DXVECTOR3 *pPosFar, D3DXVECTOR3 *pVecDiff)
{
	HWND hWnd = FindWindow(CLASS_NAME, WINDOW_NAME);

	if (!hWnd)
	{
		assert(("ウィンドウ見つからないよ！", false));
	}

	POINT posCursor;

	GetCursorPos(&posCursor);

	CDebugProc::GetInstance()->Print("\n変換前[%d,%d]", posCursor.x, posCursor.y);

	ScreenToClient(hWnd, &posCursor);

	CDebugProc::GetInstance()->Print("\n変換後[%d,%d]", posCursor.x, posCursor.y);

	*pPosNear = universal::CalcScreenToWorld(D3DXVECTOR3((float)posCursor.x, (float)posCursor.y, 0.0f));
	*pPosFar = universal::CalcScreenToWorld(D3DXVECTOR3((float)posCursor.x, (float)posCursor.y, 1.0f));

	if (pVecDiff != nullptr)
	{
		*pVecDiff = *pPosFar - *pPosNear;
		D3DXVec3Normalize(pVecDiff, pVecDiff);
	}
}

//========================================
// 外積の作成
//========================================
D3DXVECTOR3 Vec3Cross(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2)
{
	D3DXVECTOR3 vecCross;

	vecCross =
	{
		vec1.y * vec2.z - vec1.z * vec2.y,
		vec1.z * vec2.x - vec1.x * vec2.z,
		vec1.x * vec2.y - vec1.y * vec2.x
	};

	return vecCross;
}

//========================================
// オフセット設定処理
//========================================
void SetOffSet(D3DXMATRIX *pMtxWorldOffset, D3DXMATRIX mtxWorldOwner, D3DXVECTOR3 posOffset, D3DXVECTOR3 rot)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(pMtxWorldOffset);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtxWorldOffset, pMtxWorldOffset, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, posOffset.x, posOffset.y, posOffset.z);
	D3DXMatrixMultiply(pMtxWorldOffset, pMtxWorldOffset, &mtxTrans);

	//マトリックスをかけ合わせる
	D3DXMatrixMultiply(pMtxWorldOffset, pMtxWorldOffset, &mtxWorldOwner);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, pMtxWorldOffset);
}

//========================================
// 向き補正処理
//========================================
void FactingRot(float *pfRot, float fRotDest, float rotateFact)
{
	// 引数の角度の補正
	LimitRot(pfRot);
	LimitRot(&fRotDest);

	//差分角度を取得
	float fRotDiff = fRotDest - *pfRot;

	//角度の修正
	LimitRot(&fRotDiff);

	//角度補正
	*pfRot += fRotDiff * rotateFact;

	LimitRot(pfRot);
}

//========================================
// 目標方向を向く処理
//========================================
void FactingRotTarget(D3DXVECTOR3 *pRot, D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fFact)
{
	if (pRot == nullptr)
	{
		return;
	}

	// 差分角度の取得
	D3DXVECTOR3 vecDiff = posTarget - posOwn;

	D3DXVECTOR3 rotDest = VecToRot(vecDiff);

	FactingRot(&pRot->x, rotDest.x, fFact);
	FactingRot(&pRot->y, rotDest.y, fFact);
}

//========================================
// 距離の比較
//========================================
D3DXVECTOR3 VecToRot(D3DXVECTOR3 vec)
{
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	rot.y = atan2f(vec.x, vec.z);

	float fLength = sqrtf(vec.x * vec.x + vec.z * vec.z);

	rot.x = atan2f(fLength, -vec.y);

	return rot;
}

//========================================
// マトリックスから位置の取得
//========================================
D3DXVECTOR3 GetMtxPos(D3DXMATRIX mtx)
{
	D3DXVECTOR3 pos =
	{
		mtx._41,
		mtx._42,
		mtx._43
	};

	return pos;
}

//========================================
// 距離の比較
//========================================
bool DistCmp(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff)
{
	D3DXVECTOR3 vecDiff = posTarget - posOwn;
	float fLength = D3DXVec3Length(&vecDiff);

	if (fLength < fLengthMax)
	{
		if (fDiff != nullptr)
		{
			*fDiff = fLength;
		}

		return true;
	}
	else
	{
		return false;
	}
}

//========================================
// 距離の比較平面
//========================================
bool DistCmpFlat(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff)
{
	D3DXVECTOR3 vecDiff = posTarget - posOwn;
	float fLength = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

	if (fLength < fLengthMax)
	{
		if (fDiff != nullptr)
		{
			*fDiff = fLength;
		}

		return true;
	}
	else
	{
		return false;
	}
}

//========================================
// 外積の計算
//========================================
float CrossProduct(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	return pos1.z * pos2.x - pos1.x * pos2.z;
}

//========================================
// 外積の当たり判定計算
//========================================
bool IsCross(D3DXVECTOR3 posTarget, D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest, float *pRate, D3DXVECTOR3 move)
{

	D3DXVECTOR3 vecLine = D3DXVECTOR3
	(//一旦Y座標は平面
		vecDest.x - vecSorce.x,
		0.0f,
		vecDest.z - vecSorce.z
	);

	D3DXVECTOR3 vecToPos = D3DXVECTOR3
	(
		posTarget.x - vecSorce.x,
		0.0f,
		posTarget.z - vecSorce.z
	);

	//計算用変数
	D3DXVECTOR3 vecTemp;
	float fAngle = atan2f(vecLine.x, vecLine.z);
	float fAreaMax = D3DXVec3Cross(&vecTemp, &vecLine, &move)->y;
	float fArea = D3DXVec3Cross(&vecTemp, &vecToPos, &move)->y;
	float fRate = fArea / fAreaMax;

	if (pRate != nullptr)
	{
		*pRate = fRate;
	}

	if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) > 0)
	{//線分の左にめり込んでいる

		float fLength = sqrtf(vecLine.x * vecLine.x + vecLine.z * vecLine.z) * fRate;

		D3DXVECTOR3 posCross =
		{//線分の交点
			vecSorce.x + sinf(fAngle) * fLength,
			posTarget.y,
			vecSorce.z + cosf(fAngle) * fLength
		};

		return true;
	}
	else
	{
		return false;
	}
}

//========================================
// 三角形の中にいるかの判定
//========================================
bool IsInTriangle(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 posTarget)
{
	int nHit = 0;

	if(IsCross(posTarget, vtx1, vtx2, nullptr) == false)
		nHit++;
	if (IsCross(posTarget, vtx2, vtx3, nullptr) == false)
		nHit++;
	if (IsCross(posTarget, vtx3, vtx1, nullptr) == false)
		nHit++;

	if (nHit == 3)
	{
		return true;
	}

	return false;
}

//========================================
// 三角形の下にいるかの判定
//========================================
bool IsOnTrianglePolygon(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtxNor, D3DXVECTOR3 posTarget, float& rHeight)
{
	D3DXVECTOR3 vecP, vecTemp;
	float fHeight, fDot;

	// ポリゴンの外周ベクトルを設定
	D3DXVECTOR3 vecVtx[3];
	vecVtx[0] = vtx2 - vtx1;
	vecVtx[1] = vtx3 - vtx2;
	vecVtx[2] = vtx1 - vtx3;

	// ポリゴンの各点から目標へのベクトルを設定
	D3DXVECTOR3 vecVtxTarget[3];
	vecVtxTarget[0] = posTarget - vtx1;
	vecVtxTarget[1] = posTarget - vtx2;
	vecVtxTarget[2] = posTarget - vtx3;

	// 目標位置がポリゴンの内側にいるか判定
	if (D3DXVec3Cross(&vecTemp, &vecVtxTarget[0], &vecVtx[0])->y < 0 &&
		D3DXVec3Cross(&vecTemp, &vecVtxTarget[1], &vecVtx[1])->y < 0 &&
		D3DXVec3Cross(&vecTemp, &vecVtxTarget[2], &vecVtx[2])->y < 0)
	{
		// y軸法線が0ではないか判定
		if (!vtxNor.y)
			return false;

		// 内積を計算
		fDot = (vtxNor.x * vecVtxTarget[0].x) + (vtxNor.z * vecVtxTarget[0].z);

		// 内積を用いて高さを計算
		fHeight = -(fDot / vtxNor.y) + vtx1.y;

		// 高さが目標位置より高いか判定
		if (fHeight >= rHeight)
		{
			// 高さ代入
			rHeight = fHeight;
			return true;
		}
	}

	return false;
}

//========================================
// ポリゴンの下にいるかの判定
//========================================
bool IsOnSquarePolygon(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 vtxNor1, D3DXVECTOR3 vtxNor2, D3DXVECTOR3 posTarget, D3DXVECTOR3 posOldTarget, float& rHeight)
{
	bool bColllision = false;

	// 1つ目のポリゴンと内外判定
	if (IsOnTrianglePolygon(vtx1, vtx2, vtx3, vtxNor1, posTarget, rHeight))
		bColllision = true;

	// 2つ目のポリゴンと内外判定
	if (IsOnTrianglePolygon(vtx4, vtx3, vtx2, vtxNor2, posTarget, rHeight))
		bColllision = true;
	
	return bColllision;
}

//========================================
// 四角の下にいるかの判定
// 1から時計回りに代入してください
//========================================
bool IsOnSquare(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 vtxNor, D3DXVECTOR3 posTarget, D3DXVECTOR3 posOldTarget, float& rHeight)
{
	D3DXVECTOR3 vecP, vecTemp;
	float fHeight, fDot;

	// ポリゴンの外周ベクトルを設定
	D3DXVECTOR3 vecVtx[4];
	vecVtx[0] = vtx2 - vtx1;
	vecVtx[1] = vtx3 - vtx2;
	vecVtx[2] = vtx4 - vtx3;
	vecVtx[3] = vtx1 - vtx4;

	// ポリゴンの各点から目標へのベクトルを設定
	D3DXVECTOR3 vecVtxTarget[4];
	vecVtxTarget[0] = posTarget - vtx1;
	vecVtxTarget[1] = posTarget - vtx2;
	vecVtxTarget[2] = posTarget - vtx3;
	vecVtxTarget[3] = posTarget - vtx4;

	// 目標位置がポリゴンの内側にいるか判定
	if (D3DXVec3Cross(&vecTemp, &vecVtxTarget[0], &vecVtx[0])->y < 0 &&
		D3DXVec3Cross(&vecTemp, &vecVtxTarget[1], &vecVtx[1])->y < 0 &&
		D3DXVec3Cross(&vecTemp, &vecVtxTarget[2], &vecVtx[2])->y < 0 &&
		D3DXVec3Cross(&vecTemp, &vecVtxTarget[3], &vecVtx[3])->y < 0)
	{
		// y軸法線が0ではないか判定
		if (!vtxNor.y)
			return false;

		// 内積を計算
		fDot = (vtxNor.x * vecVtxTarget[0].x) + (vtxNor.z * vecVtxTarget[0].z);

		// 内積を用いて高さを計算
		fHeight = -(fDot / vtxNor.y) + vtx1.y;

		// 高さが目標位置より高いか判定
		if (fHeight > posTarget.y)
		{
			// 高さ代入
			rHeight = fHeight;
			return true;
		}
	}

	return false;
}

//========================================
// 線に対する外積の押し戻し判定
//========================================
bool LineCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3* pos, D3DXVECTOR3 posOld)
{
	D3DXVECTOR3 vecMove, vecLine, vecToPos, vecToPosOld;
	float fRate, fOutUnit, fOutRate;

	// 角ベクトル計算
	vecMove = *pos - posOld;
	vecLine = vtx1 - vtx2;
	vecToPos = *pos - vtx2;
	vecToPosOld = posOld - vtx2;

	// y方向を0にする
	vecMove.y = 0.0f;
	vecLine.y = 0.0f;
	vecToPos.y = 0.0f;
	vecToPosOld.y = 0.0f;

	// 交点倍率計算
	fOutUnit = CrossProduct(vecLine, vecMove);
	fOutRate = CrossProduct(vecToPos, vecMove);
	fRate = fOutRate / fOutUnit;

	// 線分上をまたいでいるか判定
	if (CrossProduct(vecLine, vecToPos) > 0.0f && 
		CrossProduct(vecLine, vecToPosOld) <= 0.0f && 
		fRate >= 0.0f && fRate < 1.0f)
	{
		D3DXVECTOR3 vecNor, posCross;
		float fDot;

		//法線ベクトル計算
		vecNor.x = -vecLine.z;
		vecNor.y = 0.0f;
		vecNor.z = vecLine.x;

		//法線ベクトル単位化
		D3DXVec3Normalize(&vecNor, &vecNor);

		//交点計算
		posCross = vtx2 + (vecLine * fRate);
		vecMove = *pos - posCross;
		vecMove.y = 0.0f;
		posCross.y = 0.0f;

		//水平方向大きさ
		fDot = D3DXVec3Dot(&-vecMove, &vecNor);

		*pos = posCross + (vecMove + (vecNor * fDot));

		return true;
	}

	return false;
}

//========================================
// OBBの平面に対する押し戻し判定処理
//========================================
D3DXVECTOR3 CollideOBBToPlane(D3DXVECTOR3* posOBB, D3DXVECTOR3 vecAxial, D3DXVECTOR3 posPlane, D3DXVECTOR3 vecNorPlane)
{
	// 各方向軸ベクトル計算
	D3DXVECTOR3 axis1 = D3DXVECTOR3(vecAxial.x, 0.0f, 0.0f);
	D3DXVECTOR3 axis2 = D3DXVECTOR3(0.0f, vecAxial.y, 0.0f);
	D3DXVECTOR3 axis3 = D3DXVECTOR3(0.0f, 0.0f, vecAxial.z);

	// 射影線の長さを計算
	float lenProjection = lengthAxis(vecNorPlane, axis1, axis2, axis3);

	// 線分とターゲットの位置関係を計算
	float lenPos = D3DXVec3Dot(&(*posOBB - posPlane), &vecNorPlane);

	// めり込んでいる
	if (lenProjection < fabs(lenPos))
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vecNorPlane.y = 0.0f;

	// めり込み具合で戻す距離を変える
	if (lenPos > 0.0f)
		return vecNorPlane * (lenProjection - fabs(lenPos));
	else
		return vecNorPlane * (lenProjection + fabs(lenPos));
}

//========================================
// 矩形の中にいるかどうかの計算
//========================================
bool CubeCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 pos)
{
	bool bHit = false;
	int nHit = 0;

	D3DXVECTOR3 aVec[4] =
	{// 配列に格納
		vtx1,
		vtx2,
		vtx3,
		vtx4,
	};

	// ４辺のなかにいるかのチェック
	for (int i = 0; i < 4; i++)
	{
		int nNext = (i + 1) % 4;

		// 線分の向こうにいるかの判定
		bool bCross = IsCross(pos, aVec[i], aVec[nNext], nullptr);

		if (bCross)
		{
			nHit++;
		}
	}

	if (nHit >= 4)
	{
		bHit = true;
	}

	return bHit;
}

//========================================
// 画面内判定
//========================================
bool IsInScreen(const D3DXVECTOR3 pos, D3DXMATRIX mtx, D3DXVECTOR3 *pPos)
{
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer == nullptr)
	{
		return false;
	}

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DVIEWPORT9 viewport;
	pDevice->GetViewport(&viewport);

	D3DXMATRIX projectionMatrix;
	pDevice->GetTransform(D3DTS_PROJECTION, &projectionMatrix);

	D3DXMATRIX viewMatrix;
	pDevice->GetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXVECTOR3 screenPosition;
	D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	if (pPos != nullptr)
	{
		*pPos = screenPosition;
	}

	if (screenPosition.x >= viewport.X && 
		screenPosition.x <= viewport.X + viewport.Width &&
		screenPosition.y >= viewport.Y && 
		screenPosition.y <= viewport.Y + viewport.Height &&
		screenPosition.z >= 0.0f && screenPosition.z <= 1.0f)
	{
		return true;
	}

	return false;
}

//=====================================================
// 偏差撃ちの処理
//=====================================================
D3DXVECTOR3 LinePridiction(D3DXVECTOR3 pos, float fSpeedBullet, D3DXVECTOR3 posTarget, D3DXVECTOR3 moveTarget)
{
	// 変数宣言
	D3DXVECTOR3 vecDiff;
	float fSpeedTarget;
	float fDataA, fDataB, fDataC, fDataD;
	int nFrame1, nFrame2;

	// 目標の速度を計算
	fSpeedTarget = D3DXVec3Length(&moveTarget);

	// 目標との位置の差分を取得
	vecDiff = posTarget - pos;

	fDataA = (moveTarget.x * moveTarget.x + moveTarget.y * moveTarget.y + moveTarget.z * moveTarget.z) - fSpeedBullet * fSpeedBullet;

	fDataB = 2 * (posTarget.x * moveTarget.x + posTarget.y * moveTarget.y + posTarget.z * moveTarget.z);

	fDataC = (posTarget.x * posTarget.x + posTarget.y * posTarget.y + posTarget.z * posTarget.z);

	if (fDataA == 0)
	{// 0割り防止
		if (fDataB == 0)
		{
			return posTarget;
		}
		else
		{
			return posTarget + moveTarget * (-fDataC / fDataB);
		}
	}

	//二次方程式の解の公式の判別式で分類
	fDataD = fDataB * fDataB - 4 * fDataA * fDataC;

	if (fDataD > 0)
	{
		float fDataE = (float)sqrt(fDataD);
		nFrame1 = (int)((-fDataB - fDataE) / (2 * fDataA));
		nFrame2 = (int)((-fDataB + fDataE) / (2 * fDataA));
		//解は2つなので正の数の最小値を使う
		nFrame1 = (int)PlusMin((float)nFrame1, (float)nFrame2);
	}
	else
	{
		//虚数解
		//当たらないので今の位置を狙う
		nFrame1 = 0;
	}

	//予想位置を返す
	return posTarget + moveTarget * (float)nFrame1;
}

//=====================================================
// プラスの最小値を返す処理
//=====================================================
float PlusMin(float fData1, float fData2)
{
	if (fData1 < 0 && fData2 < 0)
	{// どちらも負の値なら０を返す
		return 0;
	}
	else if (fData1 < 0)
	{
		return fData2;
	}
	else if (fData2 < 0)
	{
		return fData1;
	}

	return fData1 < fData2 ? fData1 : fData2;
}

//========================================
// 範囲内のランダム数値を返す処理
//========================================
int RandRange(int nMax, int nMin)
{
	int nRange = nMax - nMin;
	int nRand = rand() % nRange + nMin;

	return nRand;
}

//========================================
// RGBをD3DXCOLORに変換する処理
//========================================
D3DXCOLOR ConvertRGB(BYTE r, BYTE g, BYTE b, BYTE a)
{
	D3DXCOLOR col;

	col =
	{
		(float)r / 255.0f,
		(float)g / 255.0f,
		(float)b / 255.0f,
		(float)a / 255.0f,
	};

	return col;
}

//========================================
// 3D座標の線形補完
//========================================
D3DXVECTOR3 Lerp(D3DXVECTOR3 start, D3DXVECTOR3 end, float fTime)
{
	D3DXVECTOR3 pos = start;

	D3DXVECTOR3 vecDiff = end - start;

	pos += vecDiff * fTime;

	return pos;
}

//========================================
// クォータニオンのラープ
//========================================
D3DXQUATERNION QuaternionLerp(D3DXQUATERNION *pQuat1, D3DXQUATERNION *pQuat2, float t)
{
	// クォータニオンを線形補間する
	D3DXQUATERNION result(0.0f, 0.0f, 0.0f, 1.0f);

	if (pQuat1 == nullptr || pQuat2 == nullptr)
		return result;

	// 補間
	result.x = (1 - t) * pQuat1->x + t * pQuat2->x;
	result.y = (1 - t) * pQuat1->y + t * pQuat2->y;
	result.z = (1 - t) * pQuat1->z + t * pQuat2->z;
	result.w = (1 - t) * pQuat1->w + t * pQuat2->w;

	// 結果を正規化
	D3DXQuaternionNormalize(&result, &result);

	return result;
}

//========================================
// ターゲットの方を向く処理(クォータニオン)
//========================================
D3DXQUATERNION SmoothFaceTowardsTarget(D3DXQUATERNION quatCurrent, D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, D3DXVECTOR3 vecForward, D3DXVECTOR3 vecUp, float fSpeed)
{
	// ターゲット方向を計算
	D3DXVECTOR3 targetDirection = posTarget - posOwn;
	D3DXVec3Normalize(&targetDirection, &targetDirection);  // 正規化して単位ベクトルにする

	// 現在の前方向ベクトル
	D3DXVECTOR3 currentForward(0, 0, 1);

	// 現在の向きからターゲット方向へのクォータニオンを計算
	D3DXQUATERNION targetRotationQuat;
	D3DXQUATERNION identityQuat;
	D3DXQuaternionIdentity(&identityQuat);

	float dotProduct = D3DXVec3Dot(&currentForward, &targetDirection);

	if (dotProduct < -0.999f)
	{// 反対を向いていたら真上のベクトルで回転
		D3DXQuaternionRotationAxis(&targetRotationQuat, &vecUp, D3DX_PI);
	}
	else 
	{
		// 正面ベクトルとターゲットの間の回転軸を計算
		D3DXVECTOR3 rotationAxis;
		D3DXVec3Cross(&rotationAxis, &currentForward, &targetDirection);
		D3DXVec3Normalize(&rotationAxis, &rotationAxis);

		float angle = acosf(dotProduct);
		D3DXQuaternionRotationAxis(&targetRotationQuat, &rotationAxis, angle);

		CDebugProc::GetInstance()->Print("\nrotationAxis[%f,%f,%f]", rotationAxis.x, rotationAxis.y, rotationAxis.z);
	}

	// クォータニオンを補間して、スムーズに回転
	float t = fSpeed;
	D3DXQUATERNION smoothedRotation = QuaternionLerp(&quatCurrent, &targetRotationQuat, t);

	// 新しい回転を設定
	return smoothedRotation;
}

//========================================
// 線分に対するの射影変換
//========================================
float lengthAxis(D3DXVECTOR3 separationAxis, D3DXVECTOR3 e1, D3DXVECTOR3 e2, D3DXVECTOR3 e3)
{
	float length1, length2, length3;

	length1 = fabs(D3DXVec3Dot(&separationAxis, &e1));
	length2 = fabs(D3DXVec3Dot(&separationAxis, &e2));
	length3 = fabs(D3DXVec3Dot(&separationAxis, &e3));

	return length1 + length2 + length3;
}

//========================================
//3次元空間での行列による回転座標変換関数
//(任意の点からのオフセット位置を角度と距離で変換)
//========================================
D3DXVECTOR3 PosRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offset)
{
	D3DXVECTOR3 posAnswer;
	D3DXMATRIX mtxO, mtxAnswer;
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//計算用マトリックス

	//パーツのワールドマトリックス初期化
	D3DXMatrixIdentity(&mtxO);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel,
		posO.x, posO.y, posO.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxTransModel);

	mtxPalent = mtxO;

	//パーツのワールドマトリックス初期化
	D3DXMatrixIdentity(&mtxAnswer);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		3.14f, 3.14f, 3.14f);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel,
		offset.x, offset.y, offset.z);
	D3DXMatrixMultiply(&mtxAnswer, &mtxAnswer, &mtxTransModel);

	//算出したパーツのワールドマトリックスと親のマトリックスをかけ合わせる
	D3DXMatrixMultiply(&mtxAnswer,
		&mtxAnswer,
		&mtxPalent);

	posAnswer = GetMtxPos(mtxAnswer);

	return posAnswer;
}

//========================================
//3次元空間での行列による回転座標変換からの法線計算関数
//========================================
D3DXVECTOR3 NorRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offsetMax, D3DXVECTOR3 offsetMin)
{
	D3DXVECTOR3 posMax, posMin;
	D3DXVECTOR3 vecNor;

	posMax = PosRelativeMtx(posO, rot, D3DXVECTOR3(offsetMax.x, offsetMax.y, offsetMax.z));
	posMin = PosRelativeMtx(posO, rot, D3DXVECTOR3(offsetMax.x, offsetMin.y, offsetMax.z));

	vecNor = posMax - posMin;

	D3DXVec3Normalize(&vecNor, &vecNor);

	return vecNor;
}
}	// namespace universal
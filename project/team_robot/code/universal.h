//*****************************************
//
// îƒópèàóùÉZÉbÉg
// Author:˚¸éRìçñÁ
//
//*****************************************
#ifndef _UNIVERSAL_H_
#define _UNIVERSAL_H_

#include "main.h"

//*****************************************
// ÉNÉâÉXÇÃíËã`
//*****************************************
namespace universal
{
//======================================
// êßå¿ån
//======================================
void LimitValuefloat(float *pValue, float fMax, float fMin);
void LimitValueInt(int *pValue, int nMax, int nMin);
void LimitRot(float *fRot);	//âÒì]êßå‰èàóù
void LimitSpeed(D3DXVECTOR3 *pVec, float fSpeedMax);
void LimitPosInSq(float fWidth, float fHeight, D3DXVECTOR3 *pPos);
float LimitDistCylinder(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget);
float LimitDistSphereInSide(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget);

//======================================
// åvéZån
//======================================
void Horming(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, float fSpeedChase, D3DXVECTOR3 *pMove = nullptr);
D3DXVECTOR3 RelativeInversPos(D3DXVECTOR3 pos, D3DXVECTOR3 posCenter, float fRate = 1.0f);
D3DXVECTOR3 LinePridiction(D3DXVECTOR3 pos, float fSpeedBullet, D3DXVECTOR3 posTarget, D3DXVECTOR3 moveTarget);
void FactingRot(float *pfRot, float fRotDest, float rotateFact);
void FactingRotTarget(D3DXVECTOR3 *pRot, D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fFact);
void MoveToDest(D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget, float fFact);
void SetOffSet(D3DXMATRIX *pMtxWorldOffset, D3DXMATRIX mtxWorldOwner, D3DXVECTOR3 posOffset, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });
D3DXVECTOR3 VecToOffset(D3DXMATRIX mtx, D3DXVECTOR3 posOffset);
D3DXVECTOR3 PolarCoordinates(D3DXVECTOR3 rot);
float Vec3Dot(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2);
bool CalcRaySphere(D3DXVECTOR3 posStartRay, D3DXVECTOR3 vecRay, D3DXVECTOR3 posSphere, float fRadius, D3DXVECTOR3 *pPosStartHit = nullptr, D3DXVECTOR3 *pPosEndHit = nullptr);
bool CalcRayFlat(D3DXVECTOR3 posPlane, D3DXVECTOR3 nor, D3DXVECTOR3 srcRay, D3DXVECTOR3 endRay, D3DXVECTOR3 *pPosHit = nullptr);
D3DXVECTOR3 CalcScreenToWorld(D3DXVECTOR3 posScreen);
void ConvertScreenPosTo3D(D3DXVECTOR3 *pPosNear, D3DXVECTOR3 *pPosFar, D3DXVECTOR3 *pVecDiff);
D3DXVECTOR3 Vec3Cross(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2);

//======================================
// ïœä∑ån
//======================================
D3DXCOLOR ConvertRGB(BYTE r, BYTE g, BYTE b, BYTE a);
D3DXVECTOR3 VecToRot(D3DXVECTOR3 vec);
D3DXVECTOR3 GetMtxPos(D3DXMATRIX mtx);
int RandRange(int nMax, int nMin);
void VecConvertLength(D3DXVECTOR3 *pVec, float fLength);
float lengthAxis(D3DXVECTOR3 separationAxis, D3DXVECTOR3 e1, D3DXVECTOR3 e2, D3DXVECTOR3 e3);
D3DXVECTOR3 PosRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offset);
D3DXVECTOR3 NorRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offsetMax, D3DXVECTOR3 offsetMin);

//======================================
// îªíËån
//======================================
float PlusMin(float fData1, float fData2);
float CrossProduct(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
bool IsInTriangle(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 posTarget);
bool IsOnTrianglePolygon(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtxNor, D3DXVECTOR3 posTarget, float& rHeight);
bool IsOnSquarePolygon(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 vtxNor1, D3DXVECTOR3 vtxNor2, D3DXVECTOR3 posTarget, D3DXVECTOR3 posOldTarget, float& rHeight);
bool IsOnSquare(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 vtxNor, D3DXVECTOR3 posTarget, D3DXVECTOR3 posOldTarget, float& rHeight);
bool IsCross(D3DXVECTOR3 posTarget, D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest, float *pRate, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f });
bool LineCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3* pos, D3DXVECTOR3 posOld);
bool CubeCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 pos);
bool IsInScreen(const D3DXVECTOR3 pos, D3DXMATRIX mtx, D3DXVECTOR3 *pPos = nullptr);
bool DistCmp(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff);
bool DistCmpFlat(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff);
D3DXVECTOR3 CollideOBBToPlane(D3DXVECTOR3* posOBB, D3DXVECTOR3 vecAxial, D3DXVECTOR3 posPlane, D3DXVECTOR3 vecNorPlane);

//======================================
// ï‚äÆån
//======================================
D3DXVECTOR3 Lerp(D3DXVECTOR3 start, D3DXVECTOR3 end, float fTime);
D3DXQUATERNION QuaternionLerp(D3DXQUATERNION *pQuat1, D3DXQUATERNION *pQuat2, float t);
D3DXQUATERNION SmoothFaceTowardsTarget(D3DXQUATERNION quatCurrent, D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, D3DXVECTOR3 vecForward, D3DXVECTOR3 vecUp, float fSpeed);
}

#endif
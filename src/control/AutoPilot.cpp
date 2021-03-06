#include "common.h"
#include "patcher.h"
#include "AutoPilot.h"

#include "CarCtrl.h"
#include "Curves.h"
#include "PathFind.h"

void CAutoPilot::ModifySpeed(float speed)
{
	m_fMaxTrafficSpeed = max(0.01f, speed);
	float positionBetweenNodes = (float)(CTimer::GetTimeInMilliseconds() - m_nTimeEnteredCurve) / m_nTimeToSpendOnCurrentCurve;
	CCarPathLink* pCurrentLink = &ThePaths.m_carPathLinks[m_nCurrentPathNodeInfo];
	CCarPathLink* pNextLink = &ThePaths.m_carPathLinks[m_nNextPathNodeInfo];
	float currentPathLinkForwardX = m_nCurrentDirection * ThePaths.m_carPathLinks[m_nCurrentPathNodeInfo].dirX;
	float currentPathLinkForwardY = m_nCurrentDirection * ThePaths.m_carPathLinks[m_nCurrentPathNodeInfo].dirY;
	float nextPathLinkForwardX = m_nNextDirection * ThePaths.m_carPathLinks[m_nNextPathNodeInfo].dirX;
	float nextPathLinkForwardY = m_nNextDirection * ThePaths.m_carPathLinks[m_nNextPathNodeInfo].dirY;
	CVector positionOnCurrentLinkIncludingLane(
		pCurrentLink->posX + ((m_nCurrentLane + 0.5f) * LANE_WIDTH) * currentPathLinkForwardY,
		pCurrentLink->posY - ((m_nCurrentLane + 0.5f) * LANE_WIDTH) * currentPathLinkForwardX,
		0.0f);
	CVector positionOnNextLinkIncludingLane(
		pNextLink->posX + ((m_nNextLane + 0.5f) * LANE_WIDTH) * nextPathLinkForwardY,
		pNextLink->posY - ((m_nNextLane + 0.5f) * LANE_WIDTH) * nextPathLinkForwardX,
		0.0f);
	m_nTimeToSpendOnCurrentCurve = CCurves::CalcSpeedScaleFactor(
		&positionOnCurrentLinkIncludingLane,
		&positionOnNextLinkIncludingLane,
		currentPathLinkForwardX, currentPathLinkForwardY,
		nextPathLinkForwardX, nextPathLinkForwardY
	) * (1000.0f / m_fMaxTrafficSpeed);
#ifdef FIX_BUGS
	/* Casting timer to float is very unwanted, and in this case even causes crashes. */
	m_nTimeEnteredCurve = CTimer::GetTimeInMilliseconds() -
		(uint32)(positionBetweenNodes * m_nTimeToSpendOnCurrentCurve);
#else
	m_nTimeEnteredCurve = CTimer::GetTimeInMilliseconds() - positionBetweenNodes * m_nSpeedScaleFactor;
#endif
}

void CAutoPilot::RemoveOnePathNode()
{
	--m_nPathFindNodesCount;
	for (int i = 0; i < m_nPathFindNodesCount; i++)
		m_aPathFindNodesInfo[i] = m_aPathFindNodesInfo[i + 1];
}
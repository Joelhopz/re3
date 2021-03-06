#pragma once

#include "ClumpModelInfo.h"
#include "PedType.h"
#include "PedStats.h"

enum PedNode {
	PED_TORSO,
	PED_MID,	// Smid on PS2/PC, Storso on mobile/xbox
	PED_HEAD,
	PED_UPPERARML,
	PED_UPPERARMR,
	PED_HANDL,
	PED_HANDR,
	PED_UPPERLEGL,
	PED_UPPERLEGR,
	PED_FOOTL,
	PED_FOOTR,
	PED_LOWERLEGR,
	PED_NODE_MAX// Not valid: PED_LOWERLEGL
};

class CPedModelInfo : public CClumpModelInfo
{
public:
	uint32 m_animGroup;
	ePedType m_pedType;
	ePedStats m_pedStatType;
	uint32 m_carsCanDrive;
	CColModel *m_hitColModel;
	RpAtomic *m_head;
	RpAtomic *m_lhand;
	RpAtomic *m_rhand;

	static RwObjectNameIdAssocation m_pPedIds[12];

	CPedModelInfo(void) : CClumpModelInfo(MITYPE_PED) { }
	void DeleteRwObject(void);
	void SetClump(RpClump *);

	void SetLowDetailClump(RpClump*);
	void CreateHitColModel(void);
	CColModel *GetHitColModel(void) { return m_hitColModel; }
	static CColModel *AnimatePedColModel(CColModel* colmodel, RwFrame* frame);
};
static_assert(sizeof(CPedModelInfo) == 0x54, "CPedModelInfo: error");

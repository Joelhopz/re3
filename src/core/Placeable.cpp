#include "common.h"
#include "Placeable.h"
#include "patcher.h"

CPlaceable::CPlaceable(void)
{
	m_matrix.SetScale(1.0f);
}

CPlaceable::~CPlaceable(void) = default;

void
CPlaceable::SetHeading(float angle)
{
	CVector pos = GetPosition();
	m_matrix.SetRotateZ(angle);
	GetPosition() += pos;
}

bool
CPlaceable::IsWithinArea(float x1, float y1, float x2, float y2)
{
	float tmp;

	if(x1 > x2){
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if(y1 > y2){
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	return x1 <= GetPosition().x && GetPosition().x <= x2 &&
	       y1 <= GetPosition().y && GetPosition().y <= y2;
}

bool
CPlaceable::IsWithinArea(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float tmp;

	if(x1 > x2){
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if(y1 > y2){
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	if(z1 > z2){
		tmp = z1;
		z1 = z2;
		z2 = tmp;
	}

	return x1 <= GetPosition().x && GetPosition().x <= x2 &&
	       y1 <= GetPosition().y && GetPosition().y <= y2 &&
	       z1 <= GetPosition().z && GetPosition().z <= z2;
}

#include <new>

class CPlaceable_ : public CPlaceable
{
public:
	CPlaceable *ctor(void) { return ::new (this) CPlaceable(); }
	void dtor(void) { CPlaceable::~CPlaceable(); }
};

STARTPATCHES
	InjectHook(0x49F9A0, &CPlaceable_::ctor, PATCH_JUMP);
	InjectHook(0x49F9E0, &CPlaceable_::dtor, PATCH_JUMP);

	InjectHook(0x49FA00, &CPlaceable::SetHeading, PATCH_JUMP);
	InjectHook(0x49FA50, (bool (CPlaceable::*)(float, float, float, float))&CPlaceable::IsWithinArea, PATCH_JUMP);
	InjectHook(0x49FAF0, (bool (CPlaceable::*)(float, float, float, float, float, float))&CPlaceable::IsWithinArea, PATCH_JUMP);
ENDPATCHES

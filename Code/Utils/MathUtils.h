#pragma once

#include <CryEntitySystem\IEntity.h>

//Return true if the differenc between V1 and V2 is inside the range
static bool ClampRange(float V1, float  V2, float Range) { return (abs(V1 - V2) > Range); }
// Returns true if the difference between any corresponding elements (x, y) is in the range
static bool ClampRange(Vec2 V1, Vec2 V2, float Range) {
	return (abs(V1.x - V2.x) > Range ||
		abs(V1.y - V2.y) > Range);
}
// Returns true if the difference between any corresponding elements (x, y, z) is in the range
static bool ClampRange(Vec3 V1, Vec3 V2, float Range) {
	return (abs(V1.x - V2.x) > Range ||
		abs(V1.y - V2.y) > Range ||
		abs(V1.z - V2.z) > Range);
}

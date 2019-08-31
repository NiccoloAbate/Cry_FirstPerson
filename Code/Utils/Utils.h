#pragma once

#include <CryEntitySystem\IEntity.h>

struct Utils
{
	static void EntityFaceAt(IEntity *pEntity, IEntity *pEntity2)
	{
		const Vec3 vDirection = pEntity2->GetPos() - pEntity->GetPos();
		const Quat orientation = Quat::CreateRotationVDir(vDirection);
		pEntity->SetRotation(Quat(orientation));
	}
	static void EntitySilhouette(IEntity *pEntity, uint32 RGBA)
	{
		IRenderNode *pRenderNode = pEntity->GetRenderNode(-1);
		if (!pRenderNode)
			return;
		pRenderNode->m_nHUDSilhouettesParam = RGBA;
	}
	static void EntitySlotSilhouette(IEntity *pEntity, int nSlot, uint32 RGBA)
	{
		if (nSlot >= pEntity->GetSlotCount())
			return;
		IRenderNode *pRenderNode = pEntity->GetRenderNode(nSlot);
		if (!pRenderNode)
			return;
		pRenderNode->m_nHUDSilhouettesParam = RGBA;
	}
};

#ifndef VIGNETTE_HXX
# define VIGNETTE_HXX

template <class T>
inline void Manager::OnWorldObjectAppear(T const* p_Target)
{
    VignetteEntry const* l_VignetteEntry = GetVignetteEntryFromWorldObject(p_Target);
    if (l_VignetteEntry == nullptr)
        return;

    auto l_Type = GetDefaultVignetteTypeFromWorldObject(p_Target);
    auto l_TrackingQuest = GetTrackingQuestIdFromWorldObject(p_Target);

    if (l_TrackingQuest)
    {
        if (m_Owner->GetCompletedQuests().GetBit(GetQuestUniqueBitFlag(l_TrackingQuest) - 1))
            return;

        l_Type = GetTrackingVignetteTypeFromWorldObject(p_Target);
    }

    Vignette::Entity* l_Vignette = new Vignette::Entity(l_VignetteEntry, p_Target->GetMapId());
    l_Vignette->Create(l_Type, G3D::Vector3(p_Target->GetPositionX(), p_Target->GetPositionY(), p_Target->GetPositionZ()), p_Target->GetGUID());

    AddVignette(l_Vignette);
}

template <class T>
inline void Manager::OnWorldObjectDisappear(T const* p_Target)
{
    auto l_VignetteEntry = GetVignetteEntryFromWorldObject(p_Target);
    if (l_VignetteEntry == nullptr)
        return;

    DestroyAndRemoveVignettes([p_Target](Vignette::Entity const* p_Vignette) -> bool
    {
        if (p_Vignette->GeSourceGuid() == p_Target->GetGUID()
            && p_Vignette->GetVignetteType() != Vignette::Type::SourceScript)
            return true;

        return false;
    });
}
#endif VIGNETTE_HXX
// done
CellClass::CellClass()
{
    memset(this, 0, sizeof(CellClass));
    m_Smudge = SMUDGE_NONE;
    m_Overlay = OVERLAY_NONE;
    m_Template = TEMPLATE_NONE;
    m_OwningHouse = HOUSES_NONE;
    m_Owner = HOUSES_NONE;
    m_Smudge = SMUDGE_NONE;
}

// done
void CellClass::Shimmer()
{
    for (ObjectClass *optr = Cell_Occupier(); optr != nullptr; optr = optr->Get_Next()) {
        optr->Do_Shimmer();
    }
}

ObjectClass *CellClass::Cell_Occupier()
{
    for (ObjectClass *optr = m_OccupierPtr; optr; m_OccupierPtr = nullptr) {
        if (optr->Is_Active()) {
            break;
        }
        optr = optr->Get_Next();
    }
    return optr;
}

// done
BOOL CellClass::Flag_Place(HousesType house)
{
    if (!m_HasFlag && Is_Generally_Clear()) {
        m_HasFlag = true;
        m_OwningHouse = house;
        Redraw_Objects();
        return true;
    }
    return false;
}

// done
BOOL CellClass::Flag_Remove()
{
    if (m_HasFlag) {
        m_HasFlag = false;
        m_OwningHouse = HOUSES_NONE;
        Redraw_Objects();
        return true;
    }
    return false;
}

// done
InfantryClass *CellClass::Cell_Infantry() const
{
    return reinterpret_cast<InfantryClass *>(Cell_Find_Object(RTTI_INFANTRY));
}

// done
UnitClass *CellClass::Cell_Unit() const
{
    return reinterpret_cast<UnitClass *>(Cell_Find_Object(RTTI_UNIT));
}

// done
TerrainClass *CellClass::Cell_Terrain() const
{
    return reinterpret_cast<TerrainClass *>(Cell_Find_Object(RTTI_TERRAIN));
}

// done
BuildingClass *CellClass::Cell_Building() const
{
    return reinterpret_cast<BuildingClass *>(Cell_Find_Object(RTTI_BUILDING));
}

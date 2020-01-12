// done
ObjectClass::ObjectClass() :
    AbstractClass(),
    m_IsDown(false),
    m_ToDamage(false),
    m_ToDisplay(false),
    m_InLimbo(true),
    m_Selected(false),
    m_AnimAttached(false),
    m_IsFalling(false),
    m_OBit1_128(false),
    m_Next(nullptr),
    m_AttachedTrigger(nullptr),
    m_Health(255)
{
    m_Coord = -1;
}

// done
void ObjectClass::Mark_For_Redraw()
{
    if (!m_ToDisplay) {
        m_ToDisplay = true;
        g_Map.Flag_To_Redraw();
    }
}

// done
BOOL ObjectClass::Limbo()
{
    if (!g_GameActive || m_InLimbo) {
        return false;
    }

    Unselect();
    Detach_All(true);
    Mark(0);
    g_Map.Remove(this, In_Which_Layer());

    if (a1->a.vtable->Class_Of(a1)->ot.Bitfield[0] & 128) {
        g_Logic.Remove(this);
    }

    Hidden();
    m_InLimbo = true;
    m_ToDisplay = false;
    return true;
}

// done
BOOL ObjectClass::Revealed(HouseClass *house)
{
    return house != nullptr;
}

// done
int ObjectClass::Full_Name()
{
    return Class_Of()->Full_Name();
}

// done
BuildingClass *ObjectClass::Who_Can_Build_Me(BOOL a1, BOOL a2) const
{
    return Class_Of().Who_Can_Build_Me(a1, a2, Owner());
}

// done
const int16_t *ObjectClass::Occupy_List(BOOL a1) const
{
    return Class_Of().Occupy_List(a1);
}

// done
const int16_t *ObjectClass::Overlap_List(BOOL a1) const
{
    return Class_Of().Overlap_List();
}

// done
unsigned int ObjectClass::Health_Ratio()
{
    if (m_Health > 0) {
        return (m_Health * 256) / Class_Of().Get_Strength();
    }
    return 0;
}

// done
int ObjectClass::Init()
{
    CurrentObject.ActiveCount = 0;
    return VectorClass<ObjectClass *>::Clear(&CurrentObject);
}

// done
void ObjectClass::Detach_All(int a1)
{
    if (a1 || Owner() != g_PlayerPtr->What_Type()) {
        Unselect();
    }

    Detach_This_From_All(As_Target(), a1);
}

// done
void ObjectClass::Unselect()
{
    // If selected, process unselection.
    if (m_Selected) {
        // Remove this object from the currently selected objects array.
        CurrentObjects.Delete(this);

        if (!In_Which_Layer()) {
            Mark(4);
        }
        m_Selected = false;

        if (!In_Which_Layer()) {
            Mark(3);
        }
    }
}

// done
RadioMessageType ObjectClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
    switch (message) {
        case RADIO_WE_BUMPED:
            Mark(2);
            return RADIO_ROGER;
        default:
            break;
    };

    return RADIO_STATIC;
}

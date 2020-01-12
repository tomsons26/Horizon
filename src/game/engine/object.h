class ObjectClass : public AbstractClass
{
public:

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsDown : 1; // 1
    BOOL m_ToDamage : 1; // 2
    BOOL m_ToDisplay : 1; // 4
    BOOL m_InLimbo : 1; // 8
    BOOL m_Selected : 1; // 16
    BOOL m_AnimAttached : 1; // 32
    BOOL m_IsFalling : 1; // 64
    BOOL m_OBit1_128 : 1; // 128
#else
    bool m_IsDown;
    bool m_ToDamage;
    bool m_ToDisplay;
    bool m_InLimbo; // Is this object in limbo [state of nothing, thus not processed per frame tick]?
    bool m_Selected; // Has this object been selected by the player?
    bool m_AnimAttached;
    bool m_IsFalling; // Is this object falling from a height [as a paradropped object]?
    bool m_OBit1_128;
#endif
    ObjectClass *m_Next;
    ? m_AttachedTrigger;
    int16_t m_Health;
};

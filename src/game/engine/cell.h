class CellClass
{

public:

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Bit1 : 1; // 1
    BOOL m_Bit2 : 1; // 2
    BOOL m_Bit4 : 1; // 4
    BOOL m_Bit8 : 1; // 8
    BOOL m_Bit16 : 1; // 16
    BOOL m_Bit32 : 1; // 32
    BOOL m_Bit64 : 1; // 64
    BOOL m_HasFlag : 1; // 128
#else
    bool m_Bit1;
    bool m_Bit2;
    bool m_Bit4;
    bool m_Bit8;
    bool m_Bit16;
    bool m_Bit32;
    bool m_Bit64;
    bool m_HasFlag;
#endif

    TemplateType m_Template;
    uint8_t m_Icon;
    OverlayType m_Overlay;
    int8_t m_OverlayFrame;
    SmudgeType m_Smudge;
    int8_t m_SmudgeFrame;
    HousesType m_OwnerHouse;
    HousesType m_field_B;
    ObjectClass *m_OccupierPtr;
    ObjectClass *m_Overlapper[3];
    uint32_t m_OccupantBit;
    LandType m_Land;
};

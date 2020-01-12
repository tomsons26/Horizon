#include "keyboard.h"

KeyboardClass::KeyboardClass()
{
    //WWKeyboard = a1;
    memset(m_field_8, 0, sizeof(m_field_8));
    memset(m_field_108, 0, sizeof(m_field_108));

    for (unsigned short i = 0x1F; i < 0xFF; ++i) {
        if (isprint(i)) {
            unsigned short v4 = VkKeyScanA(i);
            if (v4 > 0 && v4 < 2048) {
                m_field_108[v4] = i;
                m_field_8[i] = v4;
            }
        }
    }
    
    memset(m_field_B08, 0, sizeof(m_field_B08));

    for (unsigned short j = 0; j < 0xFF; ++j) {
        if (isalpha(j) && isupper(j)) {
            m_field_B08[j] = 1;
        }
        if (j >= 0x60 && j <= 0x6F) {
            m_field_B08[j] = 2;
        }
    }

    memset(m_Elements, 0, sizeof(m_Elements));
    m_ElementGetPos = 0;
    m_ElementPutPos = 0;
    m_MouseQX = 0;
    m_MouseQY = 0;
    m_field_C10 = 0;
    m_field_C14 = 0;
    Cursor = 0;
}

// done
BOOL KeyboardClass::Is_Mouse_Key(uint16_t keycode)
{
#ifdef PLATFORM_WINDOWS
    return keycode == VK_LBUTTON || keycode == VK_MBUTTON || keycode == VK_RBUTTON;
#else
    return false;
#endif
}

// done
uint16_t KeyboardClass::Check()
{
    Fill_Buffer_From_System();
    return Peek_Element();
}

// done
uint16_t KeyboardClass::Get()
{
    // Wait until there is a valid key pressed.
    while (Check() == KN_NONE) {
        // empty
    }

    uint16_t key_val = Buff_Get();

    if (!(key_val & KEY_VK_BIT)) {
        key_val = m_field_108[key_val & 0x1FF] | (key_val & 0xFF00);
    }

    return key_val;
}

//done
BOOL KeyboardClass::Put(uint16_t keycode)
{
    return Put_Element(keycode);
}

//done
BOOL KeyboardClass::Put_Key_Message(uint16_t keycode, BOOL release, BOOL double_click)
{
    int state = 0;

    if (!Is_Mouse_Key(keycode)) {
        bool shift = GetKeyState(VK_SHIFT) >> 8 != 0;
        bool control = GetKeyState(VK_CONTROL) >> 8 != 0;
        bool alt = GetKeyState(VK_MENU) >> 8 != 0;
        bool capslock = GetKeyState(VK_CAPITAL) && m_field_B08[keycode] == 1;
        bool numlock = GetKeyState(KN_NUMLOCK) && m_field_B08[keycode] == 2;

        if (shift || capslock || numlock) {
            state |= KEY_SHIFT_BIT;
        }

        if (control) {
            state |= KEY_CTRL_BIT;
        }

        if (alt) {
            state |= KEY_ALT_BIT;
        }
    }

    if (!m_field_108[state | keycode]) {
        state |= KEY_VK_BIT;
    }

    if (release) {
        state |= KEY_RLS_BIT;
    }

    if (double_click) {
        state |= KEY_DBL_BIT;
    }

    return Put(state | keycode);
}

//done
void KeyboardClass::Put_Mouse_Message(uint16_t keycode, int mouse_x, int mouse_y, BOOL release, BOOL double_click)
{
        Put_Key_Message(keycode, release);

        Put(mouse_x);
        Put(mouse_y);
}

// done
BOOL KeyboardClass::Down(uint16_t keycode)
{
#if defined PLATFORM_WINDOWS
    return GetAsyncKeyState(keycode) != 0;
#else
    return true;
#endif
}

//done
uint16_t KeyboardClass::Peek_Element() const
{
    if (!Is_Buffer_Empty()) {
        return m_Elements[m_ElementGetPos];
    }

    return 0;
}

// done
BOOL KeyboardClass::Put_Element(uint16_t key)
{
    if (Is_Buffer_Empty()) {
        m_Elements[m_ElementPutPos] = key;
        m_ElementPutPos = m_ElementPutPos + 1;
        return true;
    }

    return false;
}

//TODO
BOOL KeyboardClass::Message_Handler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Has the message been handled?
    BOOL handled = false;

    // Get two shorts from lParam.
    // This gets mouse pos (for mouse events) relative to window.
    POINT point;
    point.x = lParam & 0xFFFF;
    point.y = (lParam >> 16) & 0xFFFF;

    switch (message) {

        case WM_LBUTTONDOWN:
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y);
            handled = true;
            break;

        case WM_LBUTTONUP:
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_LBUTTONDBLCLK:
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, true, true);
            handled = true;
            break;

        case WM_MBUTTONDOWN:
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y);
            handled = true;
            break;

        case WM_MBUTTONUP:
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_MBUTTONDBLCLK:
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, true, true);
            handled = true;
            break;

        case WM_RBUTTONDOWN:
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y);
            handled = true;
            break;

        case WM_RBUTTONUP:
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_RBUTTONDBLCLK:
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, true, true);
            handled = true;
            break;

        //dunno whats going on beyond this
        case WM_MOUSEMOVE:
            if (Cursor != nullptr) {
                SetCursor(Cursor);
            }


    }
    return 0;
}
// done
uint16_t KeyboardClass::Buff_Get()
{
    int pos = 0;

    // Wait until there is a valid key pressed.
    while (Check() == KN_NONE) {
        // empty
    }

    uint16_t key_val = m_Elements[m_ElementGetPos];

    if (Is_Mouse_Key(key_val)) {
        m_MouseQX = m_Elements[(m_ElementGetPos + 1)];
        m_MouseQY = m_Elements[(m_ElementGetPos + 2)];
        pos = m_ElementGetPos + 3;
    } else {
        pos = m_ElementGetPos + 1;
    }

    m_ElementGetPos = pos;
    return key_val;
}

// done
BOOL KeyboardClass::Is_Buffer_Empty() const
{
    return m_ElementGetPos == m_ElementPutPos;
}

// done
void KeyboardClass::Clear()
{
    m_ElementGetPos = m_ElementPutPos;
}

// done
void KeyboardClass::Fill_Buffer_From_System()
{
#if defined PLATFORM_WINDOWS
    MSG msg;

    // Check to see if any messages are waiting in the queue
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_NOREMOVE) && GetMessageA(&msg, 0, 0, 0)) {
        // Translate the message and dispatch it to WindowProc()
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
#endif
}

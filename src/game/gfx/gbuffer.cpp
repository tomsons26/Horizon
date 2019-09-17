/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Wrapper class for platform drawing engine.
 *
 * @copyright Horizon is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gbuffer.h"
#include "blitters.h"
#include "eventhandler.h"
#include "gamedebug.h"
#include "globals.h"
#include "initvideo.h"
#include "mouse.h"
#include "surfacemonitor.h"
#include <algorithm>

#ifndef GAME_DLL
BOOL GraphicViewPortClass::AllowHardwareBlitFills;
BOOL GraphicViewPortClass::AllowStretchBlits;
int GraphicViewPortClass::ScreenWidth = 640;
int GraphicViewPortClass::ScreenHeight = 400;
GraphicViewPortClass *g_logicPage = nullptr;
GraphicViewPortClass g_seenBuff;
GraphicViewPortClass g_hidPage;
GraphicBufferClass g_visiblePage;
GraphicBufferClass g_hiddenPage;
GraphicBufferClass g_sysMemPage;
GraphicBufferClass g_modeXBuff;
#endif

void Wait_Blit()
{
#ifdef BUILD_WITH_DDRAW
    DWORD result;
    do {
        result = g_paletteSurface->GetBltStatus(DDGBS_ISBLTDONE);
    } while (result != 0 && result != DDERR_SURFACELOST);
#endif
}

GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass *view)
{
    GraphicViewPortClass *old = g_logicPage;
    g_logicPage = view;

    return old;
}

GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass &view)
{
    GraphicViewPortClass *old = g_logicPage;
    g_logicPage = &view;

    return old;
}

GraphicViewPortClass::GraphicViewPortClass(GraphicBufferClass *buffer, int x, int y, int w, int h) :
    m_graphicBuff(nullptr), m_lockCount(0)
{
    Attach(buffer, x, y, w, h);
}

GraphicViewPortClass::~GraphicViewPortClass()
{
    m_offset = nullptr;
    m_width = 0;
    m_height = 0;
    m_xAdd = 0;
    m_xPos = 0;
    m_yPos = 0;
    m_pitch = 0;
    m_inVideoMemory = false;
    m_lockCount = 0;
    m_graphicBuff = nullptr;
}

void GraphicViewPortClass::Attach(GraphicBufferClass *buffer, int x, int y, int w, int h)
{
    if (buffer != this) {
        x = std::clamp(x, 0, buffer->Get_Width() - 1); // std::min(std::max(0, x), buffer->Get_Width() - 1);
        y = std::clamp(y, 0, buffer->Get_Height() - 1); // std::min(std::max(0, y), buffer->Get_Height() - 1);
        w = x + w > buffer->Get_Width() ? buffer->Get_Width() - x : w;
        h = y + h > buffer->Get_Height() ? buffer->Get_Height() - y : h;

        int scanline = y * (buffer->Get_Width() + buffer->Get_Pitch());

        m_xPos = x;
        m_yPos = y;

        m_offset = x + scanline + static_cast<char *>(buffer->Get_Offset());
        m_width = w;
        m_xAdd = buffer->Get_Width() - w;
        m_height = h;
        m_pitch = buffer->Get_Pitch();
        m_graphicBuff = buffer;
        m_inVideoMemory = buffer->In_Video_Memory();
    }
}

BOOL GraphicViewPortClass::Change(int x, int y, int w, int h)
{
    if (Get_Graphic_Buffer() == this) {
        DEBUG_LOG("GraphicBuff == this in GraphicViewPortClass::Change()!\n");
        return false;
    }

    Attach(Get_Graphic_Buffer(), x, y, w, h);

    return true;
}

BOOL GraphicViewPortClass::Lock()
{
    if (m_graphicBuff->Lock()) {
        if (m_graphicBuff != this) {
            Attach(m_graphicBuff, m_xPos, m_yPos, m_width, m_height);
        }

        return true;
    }

    return false;
}

BOOL GraphicViewPortClass::Unlock()
{
    if (m_graphicBuff->Unlock()) {
        if (m_graphicBuff != this && m_inVideoMemory && !m_graphicBuff->Get_LockCount()) {
            m_offset = nullptr;
        }

        return true;
    }

    return false;
}

void GraphicViewPortClass::Put_Pixel(int x, int y, unsigned char value)
{
    if (Lock()) {
        Buffer_Put_Pixel(*this, x, y, value);
    }

    Unlock();
}

void GraphicViewPortClass::Put_Fat_Pixel(int x, int y, int size, unsigned char value)
{
    if (Lock()) {
        if (size > 1) {
            int w = x + size;
            int h = y + size;
            for (int i = x; i < w; ++i) {
                for (int j = y; j < h; ++j) {
                    Buffer_Put_Pixel(*this, i, j, value);
                }
            }
        } else {
            Buffer_Put_Pixel(*this, x, y, value);
        }
    }

    Unlock();
}

unsigned GraphicViewPortClass::Get_Pixel(int x, int y)
{
    unsigned val = 0;

    if (Lock()) {
        val = Buffer_Get_Pixel(*this, x, y);
    }

    Unlock();

    return val;
}

void GraphicViewPortClass::Remap(int x, int y, int w, int h, unsigned char *fading_table)
{
    if (Lock()) {
        Buffer_Remap(*this, x, y, w, h, fading_table);
    }

    Unlock();
}

void GraphicViewPortClass::Draw_Rect(int x, int y, int w, int h, unsigned char color)
{
    if (Lock()) {
        Draw_Line(x, y, w, y, color);
        Draw_Line(x, h, w, h, color);
        Draw_Line(x, y, x, h, color);
        Draw_Line(w, y, w, h, color);
    }

    Unlock();
}

void GraphicViewPortClass::Draw_Line(int x1, int y1, int x2, int y2, unsigned char color)
{
    if (Lock()) {
        Buffer_Draw_Line(*this, x1, y1, x2, y2, color);
    }

    Unlock();
}

void GraphicViewPortClass::Fill_Rect(int x, int y, int w, int h, unsigned char color)
{
    if (Lock()) {
        Buffer_Fill_Rect(*this, x, y, w, h, color);
    }

    Unlock();
}

void GraphicViewPortClass::Clear(unsigned char color)
{
    if (Lock()) {
        Buffer_Clear(*this, color);
    }

    Unlock();
}

int GraphicViewPortClass::Blit(GraphicViewPortClass &view, BOOL use_keysrc)
{
#ifdef BUILD_WITH_DDRAW
    if (In_Video_Memory() && view.In_Video_Memory()) {
        return DD_Linear_Blit_To_Linear(
            view, Get_XPos(), Get_YPos(), view.Get_XPos(), view.Get_YPos(), view.Get_Width(), view.Get_Height(), use_keysrc);
    }
#endif
    if (Lock()) {
        if (view.Lock()) {
            Linear_Blit_To_Linear(*this, view, 0, 0, 0, 0, Get_Width(), Get_Height(), use_keysrc);
        }
        view.Unlock();
    }
    Unlock();

    return 0;
}

int GraphicViewPortClass::Blit(
    GraphicViewPortClass &view, int src_x, int src_y, int dst_x, int dst_y, int w, int h, BOOL use_keysrc)
{
#ifdef BUILD_WITH_DDRAW
    if (In_Video_Memory() && view.In_Video_Memory()) {
        return DD_Linear_Blit_To_Linear(view,
            src_x + Get_XPos(),
            src_y + Get_YPos(),
            dst_x + view.Get_XPos(),
            dst_y + view.Get_YPos(),
            w,
            h,
            use_keysrc);
    }
#endif
    if (Lock()) {
        if (view.Lock()) {
            Linear_Blit_To_Linear(*this, view, src_x, src_y, dst_x, dst_y, w, h, use_keysrc);
        }
        view.Unlock();
    }
    Unlock();

    return 0;
}

void GraphicViewPortClass::Draw_Stamp(
    void *tileset, int icon, int x, int y, const void *remapper, int left, int top, int right, int bottom)
{
    void(__cdecl * func)(GraphicViewPortClass &, void *, int, int, int, const void *, int, int, int, int) =
        reinterpret_cast<void(__cdecl *)(GraphicViewPortClass &, void *, int, int, int, const void *, int, int, int, int)>(
            0x004CF99E);
    if (Lock()) {
        func(*this, /*(IconControlType *)*/ tileset, icon, x, y, remapper, left, top, right, bottom);
    }

    Unlock();
}

void GraphicViewPortClass::Scale(GraphicViewPortClass &view, BOOL use_keysrc, void *fade)
{
    Scale(view,
        Get_XPos(),
        Get_YPos(),
        view.Get_XPos(),
        view.Get_YPos(),
        Get_Width(),
        Get_Height(),
        view.Get_Width(),
        view.Get_Height(),
        use_keysrc,
        fade);
}

void GraphicViewPortClass::Scale(GraphicViewPortClass &view, int src_x, int src_y, int dst_x, int dst_y, int src_w,
    int src_h, int dst_w, int dst_h, BOOL use_keysrc, void *fade)
{
    if (Lock()) {
        if (view.Lock()) {
            Linear_Scale_To_Linear(*this, view, src_x, src_y, dst_x, dst_y, src_w, src_h, dst_w, dst_h, use_keysrc, fade);
        }

        view.Unlock();
    }

    Unlock();
}

int GraphicViewPortClass::DD_Linear_Blit_To_Linear(
    GraphicViewPortClass &view, int src_x, int src_y, int dst_x, int dst_y, int w, int h, BOOL use_keysrc)
{
#ifdef BUILD_WITH_DDRAW
    int keysrcflag = 0;

    // Flags if we are keying based on source key colour
    if (use_keysrc) {
        keysrcflag = DDBLT_KEYSRC;
    }

    RECT srect;
    RECT drect;

    srect.left = src_x;
    srect.top = src_y;
    srect.right = src_x + w;
    srect.bottom = src_y + h;
    drect.left = dst_x;
    drect.top = dst_y;
    drect.right = dst_x + w;
    drect.bottom = dst_y + h;

    return view.Get_Graphic_Buffer()->Get_DD_Surface()->Blt(
        &drect, Get_Graphic_Buffer()->Get_DD_Surface(), &srect, keysrcflag | DDBLT_WAIT | DDCKEY_DESTBLT, nullptr);
#else
    return 0;
#endif
}

void GraphicViewPortClass::From_Buffer(int x, int y, int w, int h, void *buffer)
{
    if (Lock()) {
        Buffer_To_Page(x, y, w, h, buffer, *this);
    }

    Unlock();
}

void GraphicViewPortClass::To_Buffer(int x, int y, int w, int h, void *buffer, int size)
{
    if (Lock()) {
        Buffer_To_Buffer(*this, x, y, w, h, buffer, size);
    }

    Unlock();
}

unsigned GraphicViewPortClass::Print(const char *string, int x, int y, int fground, int bground)
{
    int(__cdecl * func)(GraphicViewPortClass &, const char *, int, int, int, int) =
        reinterpret_cast<int(__cdecl *)(GraphicViewPortClass &, const char *, int, int, int, int)>(0x004B6704);
    unsigned ret = 0;

    if (Lock()) {
        // ret = Buffer_Print(*this, string, x, y, fground, bground);
        ret = func(*this, string, x, y, fground, bground);
    }

    Unlock();

    return ret;
}

GraphicBufferClass::GraphicBufferClass()
#ifdef BUILD_WITH_DDRAW
    :
    m_videoSurface(nullptr)
#endif
{
#ifdef BUILD_WITH_DDRAW
    memset(&m_surfaceInfo, 0, sizeof(m_surfaceInfo));
#endif
}

GraphicBufferClass::GraphicBufferClass(int w, int h, void *buffer, int size)
{
    Init(w, h, buffer, size);
}

GraphicBufferClass::GraphicBufferClass(int w, int h, void *buffer)
{
    Init(w, h, buffer, w * h);
}

GraphicBufferClass::GraphicBufferClass(int w, int h, GBCEnum mode)
{
    Init(w, h, nullptr, w * h, mode);
}

GraphicBufferClass::~GraphicBufferClass()
{
    Un_Init();
}

void GraphicBufferClass::DD_Init(GBCEnum mode)
{
#ifdef BUILD_WITH_DDRAW
    memset(&m_surfaceInfo, 0, sizeof(m_surfaceInfo));
    m_surfaceInfo.dwSize = sizeof(m_surfaceInfo);
    m_surfaceInfo.dwFlags = DDSD_CAPS;
    m_surfaceInfo.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    if (!(mode & GBC_VISIBLE)) {
        m_surfaceInfo.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
        m_surfaceInfo.dwFlags |= DDSD_HEIGHT | DDSD_WIDTH;
        m_surfaceInfo.dwHeight = m_height;
        m_surfaceInfo.dwWidth = m_width;
    }

    // This relates to old DOS resolution which the game doesn't actually
    // support properly.
    if (m_width == 320) {
        m_surfaceInfo.ddsCaps.dwCaps |= DDSCAPS_MODEX;
    }

    g_directDrawObject->CreateSurface(&m_surfaceInfo, &m_videoSurface, NULL);
    g_allSurfaces.Add_Surface(m_videoSurface);

    if (mode & GBC_VISIBLE) {
        g_paletteSurface = m_videoSurface;
    }

    m_graphicBuffer.Set_Allocated(false);
    m_inVideoMemory = true;
    m_offset = 0;
    m_lockCount = 0;
#endif
}

void GraphicBufferClass::Attach_DD_Surface(GraphicBufferClass *buffer)
{
#ifdef BUILD_WITH_DDRAW
    m_videoSurface->AddAttachedSurface(buffer->m_videoSurface);
#endif
}

void GraphicBufferClass::Init(int width, int height, void *buffer, int size, GBCEnum mode)
{
    m_graphicBuffer.Set_Size(size);
    m_width = width;
    m_height = height;

    if (mode & (GBC_VIDEO_MEM | GBC_VISIBLE)) {
        DD_Init(mode);
    } else {
        if (buffer) {
            m_graphicBuffer.Set_Buffer(buffer);
            m_graphicBuffer.Set_Allocated(false);
        } else {
            if (!m_graphicBuffer.Get_Size()) {
                m_graphicBuffer.Set_Size(width * height);
            }

            m_graphicBuffer.Set_Buffer(new char[m_graphicBuffer.Get_Size()]);
            m_graphicBuffer.Set_Allocated(true);
        }

        m_offset = m_graphicBuffer.Get_Buffer();
        m_inVideoMemory = false;
    }

    m_pitch = 0;
    m_xAdd = 0;
    m_xPos = 0;
    m_yPos = 0;
    m_graphicBuff = this;
}

void GraphicBufferClass::Un_Init()
{
#ifdef BUILD_WITH_DDRAW
    if (!m_inVideoMemory || m_videoSurface == nullptr) {
        return;
    }

    while (m_lockCount > 0) {
        if (m_videoSurface->DeleteAttachedSurface(0, NULL) == DDERR_SURFACELOST) {
            if (GBufferFocusLoss != nullptr) {
                GBufferFocusLoss();
            }

            g_allSurfaces.Restore_Surfaces();
        }
    }

    g_allSurfaces.Remove_Surface(m_videoSurface);
    m_videoSurface->Release();
    m_videoSurface = nullptr;
#endif
}

BOOL GraphicBufferClass::Lock()
{
#ifdef BUILD_WITH_DDRAW
    if (!m_inVideoMemory) {
        return true;
    }

    if (m_videoSurface == nullptr) {
        return false;
    }

    if (!g_gameInFocus) {
        return false;
    }

    if (g_mouse != nullptr) {
        g_mouse->Block_Mouse(this);
    }

    if (m_lockCount != 0) {
        ++m_lockCount;

        if (g_mouse != nullptr) {
            g_mouse->Unblock_Mouse(this);
        }

        return true;
    } else if (m_videoSurface != nullptr) {
        for (int i = 0; m_lockCount == 0 && i < 2; ++i) {
            HRESULT locked = m_videoSurface->Lock(nullptr, &m_surfaceInfo, DDLOCK_WAIT, nullptr);

            if (locked == DD_OK) {
                m_offset = m_surfaceInfo.lpSurface;
                m_pitch = m_surfaceInfo.lPitch;
                m_pitch -= m_width;
                ++m_lockCount;
                // Original has a TotalLocks global that was probably for debugging.
                if (g_mouse != nullptr) {
                    g_mouse->Unblock_Mouse(this);
                }

                return 1;
            }

            if (locked == DDERR_SURFACELOST) {
                if (GBufferFocusLoss != nullptr) {
                    GBufferFocusLoss();
                }

                g_allSurfaces.Restore_Surfaces();
            } else {
                break;
            }
        }
    }

    if (g_mouse != nullptr) {
        g_mouse->Unblock_Mouse(this);
    }

    return false;
#else
    return false;
#endif
}

BOOL GraphicBufferClass::Unlock()
{
#ifdef BUILD_WITH_DDRAW
    if (m_lockCount == 0 || !m_inVideoMemory) {
        return true;
    }

    if (m_lockCount == 1 && m_videoSurface != nullptr) {
        if (g_mouse != nullptr) {
            g_mouse->Block_Mouse(this);
        }

        if (m_videoSurface->Unlock(nullptr) != DD_OK) {
            if (g_mouse != nullptr) {
                g_mouse->Unblock_Mouse(this);
            }

            return false;
        }

        m_offset = nullptr;
        --m_lockCount;

        if (g_mouse != nullptr) {
            g_mouse->Unblock_Mouse(this);
        }
    } else {
        --m_lockCount;
    }

    return true;
#else
    return 0;
#endif
}

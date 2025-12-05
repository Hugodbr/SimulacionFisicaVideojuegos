#pragma once

#include <cstdint>


enum class KeyCode : uint16_t {
    Unknown = 0,

    // Letters
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    // Numbers
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

    // Function keys
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    // Control keys
    Escape, Enter, Tab, Backspace, 
    Space, CapsLock,
    ShiftLeft, ShiftRight, 
    CtrlLeft, CtrlRight,
    AltLeft, AltRight, 

    // Arrows
    ArrowUp, ArrowDown, 
    ArrowLeft, ArrowRight,
};

enum class MouseButton : uint8_t {
    Left,
    Right,
    Middle
};

enum class KeyAction {
    Press,
    Release,
    Repeat
};

// Bitwise flags for key modifiers
enum KeyMod : uint8_t {
    None  = 0,
    Shift = 1 << 0,  // 00000001
    Ctrl  = 1 << 1,  // 00000010
    Alt   = 1 << 2,  // 00000100
};
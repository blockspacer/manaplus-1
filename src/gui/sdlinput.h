/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004, 2005, 2006, 2007 Olof Naess�n and Per Larsson
 * Copyright (C) 2007-2010  The Mana World Development Team
 * Copyright (C) 2011-2013  The ManaPlus Developers
 *
 *                                                         Js_./
 * Per Larsson a.k.a finalman                          _RqZ{a<^_aa
 * Olof Naess�n a.k.a jansem/yakslem                _asww7!uY`>  )\a//
 *                                                 _Qhm`] _f "'c  1!5m
 * Visit: http://guichan.darkbits.org             )Qk<P ` _: :+' .'  "{[
 *                                               .)j(] .d_/ '-(  P .   S
 * License: (BSD)                                <Td/Z <fP"5(\"??"\a.  .L
 * Redistribution and use in source and          _dV>ws?a-?'      ._/L  #'
 * binary forms, with or without                 )4d[#7r, .   '     )d`)[
 * modification, are permitted provided         _Q-5'5W..j/?'   -?!\)cam'
 * that the following conditions are met:       j<<WP+k/);.        _W=j f
 * 1. Redistributions of source code must       .$%w\/]Q  . ."'  .  mj$
 *    retain the above copyright notice,        ]E.pYY(Q]>.   a     J@\
 *    this list of conditions and the           j(]1u<sE"L,. .   ./^ ]{a
 *    following disclaimer.                     4'_uomm\.  )L);-4     (3=
 * 2. Redistributions in binary form must        )_]X{Z('a_"a7'<a"a,  ]"[
 *    reproduce the above copyright notice,       #}<]m7`Za??4,P-"'7. ).m
 *    this list of conditions and the            ]d2e)Q(<Q(  ?94   b-  LQ/
 *    following disclaimer in the                <B!</]C)d_, '(<' .f. =C+m
 *    documentation and/or other materials      .Z!=J ]e []('-4f _ ) -.)m]'
 *    provided with the distribution.          .w[5]' _[ /.)_-"+?   _/ <W"
 * 3. Neither the name of Guichan nor the      :$we` _! + _/ .        j?
 *    names of its contributors may be used     =3)= _f  (_yQmWW$#(    "
 *    to endorse or promote products derived     -   W,  sQQQQmZQ#Wwa]..
 *    from this software without specific        (js, \[QQW$QWW#?!V"".
 *    prior written permission.                    ]y:.<\..          .
 *                                                 -]n w/ '         [.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT       )/ )/           !
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY         <  (; sac    ,    '
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING,               ]^ .-  %
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF            c <   r
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR            aga<  <La
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          5%  )P'-3L
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR        _bQf` y`..)a
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          ,J?4P'.P"_(\?d'.,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES               _Pa,)!f/<[]/  ?"
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT      _2-..:. .r+_,.. .
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     ?a.<%"'  " -'.a_ _,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                     ^
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SDLINPUT_H
#define SDLINPUT_H

#include "keyinput.h"

#include <SDL/SDL.h>

#include <guichan/input.hpp>
#include <guichan/keyinput.hpp>
#include <guichan/mouseinput.hpp>

#include <queue>

class KeyInput;
class MouseInput;

namespace Key
{
    enum
    {
        SPACE              = ' ',
        TAB                = '\t',
        ENTER              = '\n',
        // Negative values, to avoid conflicts with higher character codes.
        LEFT_ALT           = -1000,
        RIGHT_ALT,
        LEFT_SHIFT,
        RIGHT_SHIFT,
        LEFT_CONTROL,
        RIGHT_CONTROL,
        LEFT_META,
        RIGHT_META,
        LEFT_SUPER,
        RIGHT_SUPER,
        INSERT,
        HOME,
        PAGE_UP,
        DELETE_,
        END,
        PAGE_DOWN,
        ESCAPE,
        CAPS_LOCK,
        BACKSPACE,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        PRINT_SCREEN,
        SCROLL_LOCK,
        PAUSE,
        NUM_LOCK,
        ALT_GR,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
}  // namespace Key

/**
 * SDL implementation of Input.
 */
class SDLInput final : public gcn::Input
{
public:
    /**
     * Constructor.
     */
    SDLInput();

    A_DELETE_COPY(SDLInput)

    /**
     * Pushes an SDL event. It should be called at least once per frame to
     * update input with user input.
     *
     * @param event an event from SDL.
     */
    virtual void pushInput(const SDL_Event &event);

    /**
     * Polls all input. It exists for input driver compatibility. If you
     * only use SDL and plan sticking with SDL you can safely ignore this
     * function as it in the SDL case does nothing.
     */
    virtual void _pollInput()
    { }

    virtual KeyInput dequeueKeyInput2() A_WARN_UNUSED;

    virtual gcn::KeyInput dequeueKeyInput() A_WARN_UNUSED
    { return gcn::KeyInput(); }

    // Inherited from Input

    virtual bool isKeyQueueEmpty() A_WARN_UNUSED;

    virtual bool isMouseQueueEmpty() A_WARN_UNUSED;

    virtual gcn::MouseInput dequeueMouseInput() A_WARN_UNUSED;

    virtual MouseInput dequeueMouseInput2() A_WARN_UNUSED;

protected:
    /**
     * Converts a mouse button from SDL to a Guichan mouse button
     * representation.
     *
     * @param button an SDL mouse button.
     * @return a Guichan mouse button.
     */
    static int convertMouseButton(const int button) A_WARN_UNUSED;

    /**
     * Converts an SDL event key to a key value.
     *
     * @param event an SDL event with a key to convert.
     * @return a key value.
     * @see Key
     */
    static int convertKeyCharacter(const SDL_Event &event) A_WARN_UNUSED;

    std::queue<KeyInput> mKeyInputQueue;
    std::queue<MouseInput> mMouseInputQueue;

    bool mMouseDown;
    bool mMouseInWindow;
};

#endif

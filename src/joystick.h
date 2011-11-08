/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <SDL.h>

class Joystick
{
    public:
        /**
         * Number of buttons we can handle.
         */
        enum
        {
            MAX_BUTTONS = 64
        };

        /**
         * Directions, to be used as bitmask values.
         */
        enum
        {
            UP    = 1,
            DOWN  = 2,
            LEFT  = 4,
            RIGHT = 8
        };

        /**
         * Initializes the joystick subsystem.
         */
        static void init();

        /**
         * Returns the number of available joysticks.
         */
        static int getNumberOfJoysticks()
        { return joystickCount; }

        /**
         * Constructor, pass the number of the joystick the new object
         * should access.
         */
        Joystick(int no);

        ~Joystick();

        bool open();

        void close();

        bool isEnabled() const
        { return mEnabled; }

        void setNumber(int n);

        static void setEnabled(bool enabled)
        { mEnabled = enabled; }

        static void getNames(std::vector <std::string> &names);

        /**
         * Updates the direction and button information.
         */
        void update();

        void startCalibration();

        void finishCalibration();

        bool isCalibrating() const
        { return mCalibrating; }

        bool buttonPressed(unsigned char no) const;

        bool isUp() const
        { return mEnabled && (mDirection & UP); };

        bool isDown() const
        { return mEnabled && (mDirection & DOWN); };

        bool isLeft() const
        { return mEnabled && (mDirection & LEFT); };

        bool isRight() const
        { return mEnabled && (mDirection & RIGHT); };

        int getNumber() const
        { return mNumber; }

    protected:
        unsigned char mDirection;
        bool mButtons[MAX_BUTTONS];
        SDL_Joystick *mJoystick;

        int mUpTolerance, mDownTolerance, mLeftTolerance, mRightTolerance;
        bool mCalibrating;
        int mNumber;
        bool mCalibrated;
        int mButtonsNumber;

        /**
         * Is joystick support enabled.
         */
        static bool mEnabled;
        static int joystickCount;

        void doCalibration();
};

extern Joystick *joystick;

#endif // JOYSTICK_H

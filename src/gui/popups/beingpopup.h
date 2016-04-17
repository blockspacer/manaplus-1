/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_POPUPS_BEINGPOPUP_H
#define GUI_POPUPS_BEINGPOPUP_H

#include "gui/widgets/popup.h"

class Being;
class Label;

/**
 * A popup that displays information about a being.
 */
class BeingPopup final : public Popup
{
    public:
        /**
         * Constructor. Initializes the being popup.
         */
        BeingPopup();

        A_DELETE_COPY(BeingPopup)

        /**
         * Destructor. Cleans up the being popup on deletion.
         */
        ~BeingPopup();

        /**
         * Sets the info to be displayed given a particular player.
         */
        void show(const int x, const int y, Being *const b);

        void postInit() final;

#ifdef USE_PROFILER
        void logic();
#endif

    private:
        Label *mBeingName A_NONNULLPOINTER;
        Label *mBeingParty A_NONNULLPOINTER;
        Label *mBeingGuild A_NONNULLPOINTER;
        Label *mBeingRank A_NONNULLPOINTER;
        Label *mBeingComment A_NONNULLPOINTER;
        Label *mBeingBuyBoard A_NONNULLPOINTER;
        Label *mBeingSellBoard A_NONNULLPOINTER;
        Label *mBeingRoom A_NONNULLPOINTER;
        Label *mBeingEffects A_NONNULLPOINTER;
};

extern BeingPopup *beingPopup;

#endif  // GUI_POPUPS_BEINGPOPUP_H

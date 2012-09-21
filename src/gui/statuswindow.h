/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef STATUS_H
#define STATUS_H

#include "listener.h"

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>

#include <map>

class AttrDisplay;
class ProgressBar;
class ScrollArea;
class VertContainer;

namespace gcn
{
    class Button;
    class Label;
}

/**
 * The player status dialog.
 *
 * \ingroup Interface
 */
class StatusWindow final : public Window,
                           public gcn::ActionListener,
                           public Listener
{
    public:
        /**
         * Constructor.
         */
        StatusWindow();

        void processEvent(Channels channel, const DepricatedEvent &event);

        void setPointsNeeded(const int id, const int needed);

        void addAttribute(const int id, const std::string &name,
                          const bool modifiable,
                          const std::string &description);

        static void updateHPBar(ProgressBar *const bar,
                                const bool showMax = false);
        static void updateMPBar(ProgressBar *bar, bool showMax = false);
        static void updateJobBar(ProgressBar *const bar,
                                 const bool percent = true);
        static void updateXPBar(ProgressBar *const bar,
                                const bool percent = true);
        static void updateWeightBar(ProgressBar *const bar);
        static void updateInvSlotsBar(ProgressBar *const bar);
        static void updateMoneyBar(ProgressBar *const bar);
        static void updateArrowsBar(ProgressBar *const bar);
        static void updateStatusBar(ProgressBar *const bar,
                                    const bool percent = true);
        static void updateProgressBar(ProgressBar *const bar, const int value,
                                      const int max, const bool percent);
        void updateProgressBar(ProgressBar *const bar, const int id,
                               const bool percent = true) const;

        void action(const gcn::ActionEvent &event);

        void clearAttributes();

    private:
        static std::string translateLetter(const char *const letters);

        static std::string translateLetter2(std::string letters);

        /**
         * Status Part
         */
        gcn::Label *mLvlLabel;
        gcn::Label *mMoneyLabel;
        gcn::Label *mHpLabel;
        gcn::Label *mMpLabel;
        gcn::Label *mXpLabel;
        ProgressBar *mHpBar;
        ProgressBar *mMpBar;
        ProgressBar *mXpBar;

        gcn::Label *mJobLvlLabel;
        gcn::Label *mJobLabel;
        ProgressBar *mJobBar;

        VertContainer *mAttrCont;
        ScrollArea *mAttrScroll;
        VertContainer *mDAttrCont;
        ScrollArea *mDAttrScroll;

        gcn::Label *mCharacterPointsLabel;
        gcn::Label *mCorrectionPointsLabel;
        gcn::Button *mCopyButton;

        typedef std::map<int, AttrDisplay*> Attrs;
        Attrs mAttrs;
};

extern StatusWindow *statusWindow;

#endif

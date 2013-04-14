/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef NET_TA_NPCHANDLER_H
#define NET_TA_NPCHANDLER_H

#include "net/net.h"
#include "net/npchandler.h"

#include "net/ea/npchandler.h"

#include "net/tmwa/messagehandler.h"

#include <map>

class NpcDialog;

namespace TmwAthena
{

class NpcHandler final : public MessageHandler, public Ea::NpcHandler
{
    public:
        NpcHandler();

        A_DELETE_COPY(NpcHandler)

        void handleMessage(Net::MessageIn &msg) override;

        void talk(const int npcId) const override;

        void nextDialog(const int npcId) const override;

        void closeDialog(const int npcId) override;

        void listInput(const int npcId,
                       const unsigned char value) const override;

        void integerInput(const int npcId, const int value) const override;

        void stringInput(const int npcId,
                         const std::string &value) const override;

        void buy(const int beingId) const override;

        void sell(const int beingId) const override;

        void buyItem(const int beingId, const int itemId,
                     const unsigned char color,
                     const int amount) const override;

        void sellItem(const int beingId, const int itemId,
                      const int amount) const override;

        int getNpc(Net::MessageIn &msg,
                   const bool haveLength) override A_WARN_UNUSED;

        void processNpcCommand(Net::MessageIn &msg, const int npcId);

        void processLangReuqest(Net::MessageIn &msg, const int npcId);

    private:
        bool mRequestLang;
};

}  // namespace TmwAthena

#endif  // NET_TA_NPCHANDLER_H

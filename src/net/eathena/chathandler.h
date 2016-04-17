/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_EATHENA_CHATHANDLER_H
#define NET_EATHENA_CHATHANDLER_H

#include "net/ea/chathandler.h"

namespace EAthena
{
class MessageOut;

class ChatHandler final : public Ea::ChatHandler
{
    public:
        ChatHandler();

        A_DELETE_COPY(ChatHandler)

        void talk(const std::string &restrict text,
                  const std::string &restrict channel) const final;

        void talkRaw(const std::string &text) const final;

        void privateMessage(const std::string &restrict recipient,
                            const std::string &restrict text) const
                            final;

        void channelMessage(const std::string &restrict channel,
                            const std::string &restrict text) const
                            final;

        void joinChannel(const std::string &channel) const final;

        void who() const final;

        void sendRaw(const std::string &args) const final;

        void ignoreAll() const final;

        void unIgnoreAll() const final;

        void createChatRoom(const std::string &title,
                            const std::string &password,
                            const int limit,
                            const bool isPublic) const final;

        void ignore(const std::string &nick) const final;

        void unIgnore(const std::string &nick) const final;

        void requestIgnoreList() const final;

        void battleTalk(const std::string &text) const final;

        void joinChat(const ChatObject *const chat,
                      const std::string &password) const final;

        void partChannel(const std::string &channel) const final;

        void talkPet(const std::string &restrict text,
                     const std::string &restrict channel) const final;

        void leaveChatRoom() const final;

        void setChatRoomOptions(const int limit,
                                const bool isPublic,
                                const std::string &password,
                                const std::string &title) const final;

        void setChatRoomOwner(const std::string &nick) const final;

        void kickFromChatRoom(const std::string &nick) const final;

    protected:
        static void processRaw(MessageOut &restrict outMsg,
                               const std::string &restrict line);
};

}  // namespace EAthena

#endif  // NET_EATHENA_CHATHANDLER_H

/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#ifndef NOTIFYCATIONS_H
#define NOTIFYCATIONS_H

#include "localconsts.h"

#include "utils/gettext.h"

namespace NotifyManager
{
    enum NotifyFlags
    {
        EMPTY = 0,
        INT,
        STRING,
        GUILD,
        GUILD_STRING,
        PARTY,
        PARTY_STRING,
        SPEECH
    };

    struct NotificationInfo final
    {
        const char *sound;
        const char *text;
        const NotifyFlags flags;
    };

    enum NotifyTypes
    {
        NONE = 0,
        BUY_DONE,
        BUY_FAILED,
        SELL_LIST_EMPTY,
        SOLD,
        SELL_FAILED,
        SELL_TRADE_FAILED,
        SELL_UNSELLABLE_FAILED,
        ONLINE_USERS,
        GUILD_CREATED,
        GUILD_ALREADY,
        GUILD_EMPERIUM_CHECK_FAILED,
        GUILD_ERROR,
        GUILD_LEFT,
        GUILD_INVITE_FAILED,
        GUILD_INVITE_REJECTED,
        GUILD_INVITE_JOINED,
        GUILD_INVITE_FULL,
        GUILD_INVITE_ERROR,
        GUILD_USER_LEFT,
        GUILD_KICKED,
        GUILD_USER_KICKED,
        USE_FAILED,
        EQUIP_FAILED,
        PARTY_CREATE_FAILED,
        PARTY_CREATED,
        PARTY_LEFT,
        PARTY_USER_JOINED,
        PARTY_INVITE_ALREADY_MEMBER,
        PARTY_INVITE_REFUSED,
        PARTY_INVITE_DONE,
        PARTY_INVITE_PARTY_FULL,
        PARTY_INVITE_ERROR,
        PARTY_EXP_SHARE_ON,
        PARTY_EXP_SHARE_OFF,
        PARTY_EXP_SHARE_ERROR,
        PARTY_ITEM_SHARE_ON,
        PARTY_ITEM_SHARE_OFF,
        PARTY_ITEM_SHARE_ERROR,
        PARTY_USER_LEFT,
        PARTY_UNKNOWN_USER_MSG,
        PARTY_USER_NOT_IN_PARTY,
        MONEY_GET,
        MONEY_SPENT,
        SKILL_RAISE_ERROR,
        ARROWS_EQUIP_NEEDED,
        TRADE_FAIL_FAR_AWAY,
        TRADE_FAIL_CHAR_NOT_EXISTS,
        TRADE_CANCELLED_ERROR,
        TRADE_CANCELLED_NAME,
        TRADE_ERROR_UNKNOWN,
        TRADE_ADD_PARTNER_OVER_WEIGHT,
        TRADE_ADD_PARTNER_NO_SLOTS,
        TRADE_ADD_UNTRADABLE_ITEM,
        TRADE_ADD_ERROR,
        TRADE_CANCELLED,
        TRADE_COMPLETE,
        KICK_FAIL,
        KICK_SUCCEED,
        MVP_PLAYER,
        WHISPERS_IGNORED,
        WHISPERS_IGNORE_FAILED,
        WHISPERS_UNIGNORED,
        WHISPERS_UNIGNORE_FAILED,
        SKILL_FAIL_MESSAGE,
        PVP_OFF_GVG_OFF,
        PVP_ON,
        GVG_ON,
        PVP_ON_GVG_ON,
        PVP_UNKNOWN,

        TYPE_END
    };

    static const NotificationInfo notifications[] =
    {
        {"",
            "", EMPTY},
        {"buy done",
            N_("Thanks for buying."), EMPTY},
        {"buy fail",
            N_("Unable to buy."), EMPTY},
        {"sell empty",
            N_("Nothing to sell."), EMPTY},
        {"sell done",
            N_("Thanks for selling."), EMPTY},
        {"sell fail",
            N_("Unable to sell."), EMPTY},
        {"sell trade fail",
            N_("Unable to sell while trading."), EMPTY},
        {"sell unsellable fail",
            N_("Unable to sell unsellable item."), EMPTY},
        {"online users",
            N_("Online users: %d"), INT},
        {"guild created",
            N_("Guild created."), EMPTY},
        {"guild already",
            N_("You are already in guild."), EMPTY},
        {"guild create fail",
            N_("Emperium check failed."), EMPTY},
        {"guild error",
            N_("Unknown server response."), EMPTY},
        {"guild left",
            N_("You have left the guild."), EMPTY},
        {"guild invite fail",
            N_("Could not invite user to guild."), GUILD},
        {"guild invite rejected",
            N_("User rejected guild invite."), GUILD},
        {"guild invite joined",
            N_("User is now part of your guild."), GUILD},
        {"guild invite full",
            N_("Your guild is full."), GUILD},
        {"guild invite error",
            N_("Unknown guild invite response."), GUILD},
        {"guild user left",
            N_("%s has left your guild."), GUILD_STRING},
        {"guild kicked",
            N_("You were kicked from guild."), EMPTY},
        {"guild user kicked",
            N_("%s has kicked from your guild."), GUILD_STRING},
        {"use failed",
            N_("Failed to use item."), EMPTY},
        {"equip failed",
            N_("Unable to equip."), EMPTY},
        {"party create failed",
            N_("Could not create party."), EMPTY},
        {"party created",
            N_("Party successfully created."), EMPTY},
        {"party left",
            N_("You have left the party."), EMPTY},
        {"party user joined",
            N_("%s has joined your party."), PARTY_STRING},
        {"party invite already member",
            N_("%s is already a member of a party."), PARTY_STRING},
        {"party invite refused",
            N_("%s refused your invitation."), PARTY_STRING},
        {"party invite done",
            N_("%s is now a member of your party."), PARTY_STRING},
        {"party invite full",
            N_("%s can't join your party because party is full."),
            PARTY_STRING},
        {"party invite error",
            N_("QQQ Unknown invite response for %s."), PARTY_STRING},
        {"party exp sharing on",
            N_("Experience sharing enabled."), PARTY},
        {"party exp sharing off",
            N_("Experience sharing disabled."), PARTY},
        {"party exp sharing error",
            N_("Experience sharing not possible."), PARTY},
        {"party item sharing on",
            N_("Item sharing enabled."), PARTY},
        {"party item sharing off",
            N_("Item sharing disabled."), PARTY},
        {"party item sharing error",
            N_("Item sharing not possible."), PARTY},
        {"party user left",
            N_("%s has left your party."), PARTY_STRING},
        {"party unknown user msg",
            N_("An unknown member tried to say: %s"), PARTY_STRING},
        {"party user not in party",
            N_("%s is not in your party!"), PARTY_STRING},
        {"money get",
            N_("You picked up %s."), STRING},
        {"money spend",
            N_("You spent %s."), STRING},
        {"skill raise error",
            N_("Cannot raise skill!"), EMPTY},
        {"arrow equip needed",
            N_("Equip arrows first."), EMPTY},
        {"trade fail far away",
            N_("Trading with %s isn't possible. Trade partner is "
            "too far away."), STRING},
        {"trade fail chat not exists",
            N_("Trading with %s isn't possible. Character doesn't exist."),
            STRING},
        {"trade cancelled error",
            N_("Trade cancelled due to an unknown reason."), EMPTY},
        {"trade cancelled user",
            N_("Trade with %s cancelled."), STRING},
        {"trade error unknown",
            N_("Unhandled trade cancel packet with %s"), STRING},
        {"trade add partner over weighted",
            N_("Failed adding item. Trade partner is over weighted."), EMPTY},
        {"trade add partned has no free slots",
            N_("Failed adding item. Trade partner has no free slot."), EMPTY},
        {"trade add untradable item",
            N_("Failed adding item. You can't trade this item."), EMPTY},
        {"trade add error",
            N_("Failed adding item for unknown reason."), EMPTY},
        {"trade cancelled",
            N_("Trade canceled."), EMPTY},
        {"trade complete",
            N_("Trade completed."), EMPTY},
        {"kick fail",
            N_("Kick failed!"), EMPTY},
        {"kick succeed",
            N_("Kick succeeded!"), EMPTY},
        {"mvp player",
            N_("MVP player: %s"), STRING},
        {"whispers ignored",
            N_("All whispers ignored."), EMPTY},
        {"whispers ignore failed",
            N_("All whispers ignore failed."), EMPTY},
        {"whispers unignored",
            N_("All whispers unignored."), EMPTY},
        {"whispers unignore failed",
            N_("All whispers unignore failed."), EMPTY},
        {"skill fail message",
            "%s", STRING},
        {"pvp off gvg off",
            N_("pvp off, gvg off"), SPEECH},
        {"pvp on",
            N_("pvp on"), SPEECH},
        {"gvg on",
            N_("gvg on"), SPEECH},
        {"pvp on gvg on",
            N_("pvp on, gvg on"), SPEECH},
        {"unknown pvp",
            N_("unknown pvp"), SPEECH},
    };
}  // namespace NotifyManager
#endif

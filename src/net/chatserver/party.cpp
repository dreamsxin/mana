/*
 *  The Mana World
 *  Copyright 2008 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include "party.h"

#include "internal.h"

#include "../connection.h"
#include "../messageout.h"
#include "../protocol.h"

#include "../../log.h"

void Net::ChatServer::Party::invitePlayer(const std::string &name)
{
    logger->log("Sending PCMSG_PARTY_INVITE");
    MessageOut msg(PCMSG_PARTY_INVITE);

    msg.writeString(name);

    Net::ChatServer::connection->send(msg);
}

void Net::ChatServer::Party::acceptInvite(const std::string &name)
{
    logger->log("Sending PCMSG_PARTY_ACCEPT_INVITE");
    MessageOut msg(PCMSG_PARTY_ACCEPT_INVITE);

    msg.writeString(name);

    Net::ChatServer::connection->send(msg);
}

void Net::ChatServer::Party::getPartyMembers()
{
    logger->log("Sending PCMSG_PARTY_GET_MEMBERS");
//    MessageOut msg(PCMSG_GUILD_GET_MEMBERS);

//    msg.writeInt16(guildId);

//    Net::ChatServer::connection->send(msg);
}

void Net::ChatServer::Party::quitParty()
{
    logger->log("Sending PCMSG_PARTY_QUIT");
    MessageOut msg(PCMSG_PARTY_QUIT);

    Net::ChatServer::connection->send(msg);
}

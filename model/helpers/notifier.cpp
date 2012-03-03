
// This file is part of IRC Chatter, the first IRC Client for MeeGo.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program  is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright (C) 2012, Timur Kristóf <venemo@fedoraproject.org>

#include <MNotificationGroup>
#include <MNotification>
#include "notifier.h"

static MNotificationGroup *notificationGroup = 0;

void Notifier::notify(const QString &message)
{
    if (!notificationGroup)
    {
        notificationGroup = new MNotificationGroup(MNotification::ImReceivedEvent, "New IRC messages");
        notificationGroup->setImage("/usr/share/icons/hicolor/80x80/apps/irc-chatter-harmattan-icon.png");
        notificationGroup->publish();
    }

    if (!notificationGroup->isPublished())
        notificationGroup->publish();

    MNotification *notification = new MNotification(MNotification::ImReceivedEvent, "New IRC message", message);
    notification->setImage("/usr/share/icons/hicolor/80x80/apps/irc-chatter-harmattan-icon.png");
    notification->setGroup(*notificationGroup);
    notification->publish();

    // TODO: why doesn't the image work?
    // TODO: perhaps add an action so that the app is focused when the user clicks on the notifications
}

void Notifier::unpublish()
{
    if (notificationGroup)
    {
        notificationGroup->remove();
    }
}

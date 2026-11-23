/*
Littlefield Grid FTRLV modified version, 2026-11-22.
Changes relative to Kokua 7.2.0.61057 are documented in doc/CHANGES-FTRLV.md.
Existing upstream copyright and license notices are retained.
*/
/**
 * @file llfloatergridstatus.cpp
 * @brief Littlefield Grid status floater - reports viewer connection state
 *
 * $LicenseInfo:firstyear=2009&license=viewerlgpl$
 * Second Life Viewer Source Code
 * Copyright (C) 2017, Linden Research, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License only.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
 * $/LicenseInfo$
 */

#include "llviewerprecompiledheaders.h"
#include "llfloatergridstatus.h"

#include "llagent.h"
#include "llappviewer.h"
#include "lllittlefieldedition.h"
#include "lltextbox.h"
#include "lluictrl.h"
#include "llweb.h"

LLFloaterGridStatus::LLFloaterGridStatus(const LLSD& key) : LLFloater(key)
{
}

bool LLFloaterGridStatus::postBuild()
{
    // The existing LFGrid integration supplies a welcome page, not a status feed.
    // Never infer grid-wide availability from a web request or a region connection.
    getChild<LLUICtrl>("open_portal")->setCommitCallback([](LLUICtrl*, const LLSD&)
    {
        LLWeb::loadURL(LittlefieldEdition::LOGIN_PAGE);
    });
    return true;
}

void LLFloaterGridStatus::draw()
{
    const bool connected = gAgent.getRegion() && !gDisconnected;
    getChild<LLTextBox>("connection_state")->setText(
        getString(connected ? "session_connected" : "session_disconnected"));
    LLFloater::draw();
}

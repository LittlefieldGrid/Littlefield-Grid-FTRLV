/*
Littlefield Grid FTRLV modified version, 2026-11-22.
Changes relative to Kokua 7.2.0.61057 are documented in doc/CHANGES-FTRLV.md.
Existing upstream copyright and license notices are retained.
*/
/**
 * @file llfloatergridstatus.h
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

#ifndef LL_LLFLOATERGRIDSTATUS_H
#define LL_LLFLOATERGRIDSTATUS_H

#include "llfloater.h"

class LLFloaterGridStatus : public LLFloater
{
public:
    LLFloaterGridStatus(const LLSD& key);
    void draw() override;

private:
    bool postBuild() override;
};

#endif // LL_LLFLOATERGRIDSTATUS_H

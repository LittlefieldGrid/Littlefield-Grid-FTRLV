/*
Littlefield Grid FTRLV modified version, 2026-11-22.
Changes relative to Kokua 7.2.0.61057 are documented in doc/CHANGES-FTRLV.md.
Existing upstream copyright and license notices are retained.
*/
// Application identity and initial login policy for the Littlefield edition.
#ifndef LL_LITTLEFIELD_EDITION_H
#define LL_LITTLEFIELD_EDITION_H

namespace LittlefieldEdition
{
inline constexpr char APPLICATION_NAME[] = "Littlefield Grid FTRLV";
// Preserve the established profile, cache, and Windows window class.
inline constexpr char PROFILE_NAME[] = "Littlefield Grid Kokua FTRLV Edition";
inline constexpr char GRID_KEY[] = "grid.lfgrid.com:8002";
inline constexpr char GRID_LABEL[] = "Littlefield Grid";
inline constexpr char GRID_ID[] = "lfgrid";
inline constexpr char LOGIN_URI[] = "http://grid.lfgrid.com:8002/";
inline constexpr char LOGIN_PAGE[] = "https://portal.lfgrid.com/welcome/";
}

#endif

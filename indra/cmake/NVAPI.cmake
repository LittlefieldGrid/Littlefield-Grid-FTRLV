# Littlefield Grid FTRLV modified version, 2026-11-22.
# Changes relative to Kokua 7.2.0.61057 are documented in doc/CHANGES-FTRLV.md.
# Existing upstream copyright and license notices are retained.
# -*- cmake -*-
include(Prebuilt)

set(NVAPI ON CACHE BOOL "Use NVAPI.")

if (NVAPI)
  if (WINDOWS)
    add_library( ll::nvapi INTERFACE IMPORTED )
    target_link_libraries( ll::nvapi INTERFACE nvapi)
    target_compile_definitions( ll::nvapi INTERFACE LL_NVAPI )
    use_prebuilt_binary(nvapi)
  endif (WINDOWS)
endif (NVAPI)


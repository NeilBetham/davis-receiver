/**
 * @brief typedefs for LWIP to work in this build env
 */
#pragma once

#define LWIP_NO_STDINT_H 0
#define LWIP_NO_STDDEF_H 0

#undef __linux__

#define LWIP_CHKSUM_ALGORITHM 3

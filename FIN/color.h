#pragma once

#define ASCII_ESC "\033"

#define CLR_BOLD ASCII_ESC"[1m"
#define CLR_FLASH ASCII_ESC"[5m"

#define CLR_RST ASCII_ESC"[0m"
#define CLR_RED ASCII_ESC"[31m"
#define CLR_GRN ASCII_ESC"[32m"
#define CLR_YLW ASCII_ESC"[33m"
#define CLR_BL ASCII_ESC"[34m"
#define CLR_MGT ASCII_ESC"[35m"
#define CLR_CYN ASCII_ESC"[36m"
#define CLR_WHT ASCII_ESC"[37m"
#define CLR_GRY ASCII_ESC"[90m"
#define CLR_LRED ASCII_ESC"[91m"
#define CLR_LGRN ASCII_ESC"[92m"
#define CLR_LYLW ASCII_ESC"[93m"
#define CLR_LBL ASCII_ESC"[94m"
#define CLR_LMGT ASCII_ESC"[95m"
#define CLR_LCYN ASCII_ESC"[96m"
#define CLR_LWHT ASCII_ESC"[97m"

#define CLR_24_FG(R,G,B) ASCII_ESC"[38;2;"#R ";"#G ";"#B "m"
#define CLR_24_BG(R,G,B) ASCII_ESC"[48;2;"#R ";"#G ";"#B "m"
SET C51INC=D:\keil C51\C51\Inc;D:\keil C51\C51\Inc\STC
SET C51LIB=D:\keil C51\C51\Lib
SET CPU_TYPE=STC12C5A60S2
SET CPU_VENDOR=STC
SET UV2_TARGET=Target 1
SET CPU_XTAL=0x02AEA540
"D:\keil C51\C51\BIN\C51.EXE" @".\Objects\delay.__i"
"D:\keil C51\C51\BIN\C51.EXE" @".\Objects\main.__i"
"D:\keil C51\C51\BIN\C51.EXE" @".\Objects\uart.__i"
"D:\keil C51\C51\BIN\C51.EXE" @".\Objects\temp.__i"
"D:\keil C51\C51\BIN\BL51.EXE" @".\Objects\pro.lnp"
"D:\keil C51\C51\BIN\OH51.EXE" ".\Objects\pro" 

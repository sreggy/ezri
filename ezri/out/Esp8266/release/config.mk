# Automatically generated file. Do not edit.

APP_NAME=app
COM_OPTS=--raw --encoding ascii --rts 0 --dtr 0
COM_PORT=/dev/cu.usbserial-0001
COM_PORT_ESPTOOL=/dev/cu.usbserial-0001
COM_PORT_GDB=$(COM_PORT)
COM_SPEED=115200
COM_SPEED_ESPTOOL=$(COM_SPEED)
COM_SPEED_SERIAL=$(COM_SPEED)
DEBUG_PRINT_FILENAME_AND_LINE=0
DEBUG_VERBOSE_LEVEL=2
DISABLE_NETWORK=0
DISABLE_WIFI=0
ENABLE_CUSTOM_HEAP=0
ENABLE_CUSTOM_LWIP=1
ENABLE_CUSTOM_PWM=1
ENABLE_ESPCONN=0
ENABLE_FILE_SIZE64=
ENABLE_GDB=
ENABLE_LWIPDEBUG=0
ENABLE_SMART_CONFIG=
ENABLE_SPI_DEBUG=0
ENABLE_SSL=0
ENABLE_STORAGE_SIZE64=
ENABLE_TASK_COUNT=
ENABLE_WPS=
FSBUILD_OPTIONS=
FULL_COMPONENT_BUILD=
FWFS_DEBUG=0
GDB_CMDLINE=trap  INT; $(GDB) -x $(GDBSTUB_DIR)/gdbcmds -b $(COM_SPEED_GDB) -ex "target remote $(COM_PORT_GDB)"
GDB_UART_SWAP=
HTTP_SERVER_EXPOSE_NAME=1
HTTP_SERVER_EXPOSE_VERSION=0
HWCONFIG=ota
HWCONFIG_OPTS=
KILL_TERM=pkill -9 -f "$(COM_PORT) $(COM_SPEED_SERIAL)" || exit 0
LOCALE=
PARTITION_TABLE_OFFSET=0x003fa000
PIP_ARGS=
RBOOT_GPIO_ENABLED=0
RBOOT_GPIO_SKIP_ENABLED=0
RBOOT_LD_TEMPLATE=$(RBOOT_DIR)/rboot.rom0.ld
RBOOT_ROM_0=rom0
RBOOT_ROM_1=rom1
RBOOT_RTC_ENABLED=0
RBOOT_SILENT=0
SERVER_OTA_PORT=9999
SPIFFS_OBJ_META_LEN=16
SPIFF_BIN=spiff_rom
SPIFF_FILEDESC_COUNT=7
SPIFF_FILES=files
SPISOFT_DELAY_FIXED=0
SPISOFT_DELAY_VARIABLE=0
SSL_DEBUG=0
STRING_OBJECT_SIZE=12
TASK_QUEUE_LENGTH=10
TERMINAL=$(PYTHON) -m serial.tools.miniterm $(COM_OPTS) $(COM_PORT) $(COM_SPEED_SERIAL)
TRACE=
USER_CFLAGS=
USE_US_TIMER=1
WIFI_PWD=
WIFI_SSID=

CACHED_VAR_NAMES := APP_NAME COM_OPTS COM_PORT COM_PORT_ESPTOOL COM_PORT_GDB COM_SPEED COM_SPEED_ESPTOOL COM_SPEED_SERIAL DEBUG_PRINT_FILENAME_AND_LINE DEBUG_VERBOSE_LEVEL DISABLE_NETWORK DISABLE_WIFI ENABLE_CUSTOM_HEAP ENABLE_CUSTOM_LWIP ENABLE_CUSTOM_PWM ENABLE_ESPCONN ENABLE_FILE_SIZE64 ENABLE_GDB ENABLE_LWIPDEBUG ENABLE_SMART_CONFIG ENABLE_SPI_DEBUG ENABLE_SSL ENABLE_STORAGE_SIZE64 ENABLE_TASK_COUNT ENABLE_WPS FSBUILD_OPTIONS FULL_COMPONENT_BUILD FWFS_DEBUG GDB_CMDLINE GDB_UART_SWAP HTTP_SERVER_EXPOSE_NAME HTTP_SERVER_EXPOSE_VERSION HWCONFIG HWCONFIG_OPTS KILL_TERM LOCALE PARTITION_TABLE_OFFSET PIP_ARGS RBOOT_GPIO_ENABLED RBOOT_GPIO_SKIP_ENABLED RBOOT_LD_TEMPLATE RBOOT_ROM_0 RBOOT_ROM_1 RBOOT_RTC_ENABLED RBOOT_SILENT SERVER_OTA_PORT SPIFFS_OBJ_META_LEN SPIFF_BIN SPIFF_FILEDESC_COUNT SPIFF_FILES SPISOFT_DELAY_FIXED SPISOFT_DELAY_VARIABLE SSL_DEBUG STRING_OBJECT_SIZE TASK_QUEUE_LENGTH TERMINAL TRACE USER_CFLAGS USE_US_TIMER WIFI_PWD WIFI_SSID
COMPONENT_SOC := esp*

COMPONENT_DEPENDS += CommandProcessing OtaNetwork


ARDUINO_LIBRARIES := ArduinoJson6
HWCONFIG := ota
# HWCONFIG := spiffs-2m
SPIFF_FILES = files

# COMPONENT_DEPENDS := OtaNetwork

# CONFIG_VARS := RBOOT_TWO_ROMS 
# RBOOT_TWO_ROMS := 1

# # download urls, set appropriately
# CONFIG_VARS += ROM_0_URL \
# 			   ROM_1_URL \
# 			   SPIFFS_URL
			    
# ROM_0_URL := "http://192.168.7.5:80/rom0.bin"
# ROM_1_URL := "http://192.168.7.5:80/rom1.bin"
# SPIFFS_URL := "http://192.168.7.5:80/spiff_rom.bin"

# APP_CFLAGS = -DROM_0_URL="\"$(ROM_0_URL)"\" \
#              -DROM_1_URL="\"$(ROM_1_URL)"\" \
#              -DSPIFFS_URL="\"$(SPIFFS_URL)"\" \


# COMPONENT_PATH := /Users/a020900/dev/iot/ezri

COMPONENT_SRCDIRS := ../ez-lib/src
COMPONENT_SRCDIRS += ./app
COMPONENT_INCDIRS := ../ez-lib/include
COMPONENT_INCDIRS += ./include


ROM_TYPE := ezri
ROM_ENVIRONMENT := test

# APP_CFLAGS = -D__ROM_TYPE__=\"$(ROM_TYPE)\" \
#  			 -D__ROM_ENVIRONMENT__=\"$(ROM_ENVIRONMENT)\"



# ifneq ($(RBOOT_TWO_ROMS),)
# 	APP_CFLAGS += -DRBOOT_TWO_ROMS=$(RBOOT_TWO_ROMS)
# endif
fprelib:
	../ez-lib/pre-script.py -m
fpreapps:
	../script/pre-script.py -m -t $(ROM_TYPE) -e $(ROM_ENVIRONMENT)
post:
	../script/post-script.py --local true -t $(ROM_TYPE)

full: fprelib fpreapps all buildpart post

fapps: fpreapps all buildpart post

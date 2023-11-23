DIR_Config	 = ./lib/Config
DIR_EPD		 = ./lib/e-Paper
DIR_FONTS	 = ./lib/Fonts
DIR_GUI		 = ./lib/GUI
DIR_Hardware = ./lib/Hardware
DIR_Tools    = ./lib/Tools
DIR_Examples = ./examples
DIR_BIN		 = ./bin

EPD = NULL
ifeq ($(EPD), epd2in9V2)
	OBJ_CXX_EPD = ${DIR_EPD}/2in9v2/epd_2in9v2.cpp
#	OBJ_C_Examples = ${DIR_Examples}/EPD_2in9_V2_test.c
else 
    OBJ_CXX_EPD = NULL
#    OBJ_C_Examples = NULL
endif
OBJ_CXX = $(wildcard ${DIR_Examples}/main.cpp ${OBJ_CXX_EPD} )
OBJ_C = $(wildcard  ${DIR_GUI}/*.c ${DIR_Examples}/ImageData2.c ${DIR_Examples}/ImageData.c ${DIR_FONTS}/*.c )
COBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))
CXXOBJ_O = $(patsubst %.cpp,${DIR_BIN}/%.o,$(notdir ${OBJ_CXX}))

NPI_DEV_C = 
#NPI_DEV_C = $(wildcard $(DIR_BIN)/sysfs_software_spi.o $(DIR_BIN)/npi_neo3.o $(DIR_BIN)/epd_2in9v2.o )

OPTS = 

ifdef DEBUG 
	OPTS += -D DEBUG
endif

ifdef NPI_OPTS
	OPTS += -D $(NPI_OPTS)
endif

.PHONY : NPI clean

NPI: NPI_DEV NPI_epd

TARGET = epd
CC = gcc
CXX = g++
MSG = -g -O -ffunction-sections -fdata-sections -Wall $(OPTS)
CFLAGS += $(MSG) -D $(EPD) -g
CXXFLAGS += $(MSG) -D $(EPD) -g

NPI_epd:${COBJ_O} ${CXXOBJ_O}
	echo $(@)
	$(CXX) $(CFLAGS) $(COBJ_O) $(CXXOBJ_O) $(NPI_DEV_C) -o $(TARGET)

$(shell mkdir -p $(DIR_BIN))

${DIR_BIN}/%.o:$(DIR_Examples)/%.c
	$(CC) $(CFLAGS) -c	$< -o $@ -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD)

${DIR_BIN}/%.o:$(DIR_Examples)/%.cpp
	$(CXX) $(CFLAGS) -c	$< -o $@ -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD)

${DIR_BIN}/%.o:$(DIR_EPD)/%.c
	$(CC) $(CFLAGS) -c	$< -o $@ -I $(DIR_Config)

${DIR_BIN}/%.o:$(DIR_EPD)/%.cpp
	$(CXX) $(CFLAGS) -c	$< -o $@ -I $(DIR_Config)

${DIR_BIN}/%.o:$(DIR_FONTS)/%.c 
	$(CC) $(CFLAGS) -c	$< -o $@ 
	
${DIR_BIN}/%.o:$(DIR_GUI)/%.c
	$(CC) $(CFLAGS) -c	$< -o $@ -I $(DIR_Config)

NPI_DEV:
ifdef NPI_OPTS
ifeq ($(NPI_OPTS), SOFTSPI)
	$(CXX) $(CFLAGS) -c	 $(DIR_Tools)/software_spi/sysfs_software_spi.cpp -o $(DIR_BIN)/sysfs_software_spi.o
#		NPI_DEV_C += $(DIR_BIN)/sysfs_software_spi.o
endif
endif
#	$(CXX) $(CFLAGS) $(DEBUG_NPII) -c	 $(DIR_Config)/sysfs_software_spi.cpp -o $(DIR_BIN)/sysfs_software_spi.o $(LIB_NPII) $(DEBUG)
	$(CXX) $(CFLAGS) -c	$(DIR_Hardware)/npi_neo3/npi_neo3.cpp -o $(DIR_BIN)/npi_neo3.o
	$(CXX) $(CFLAGS) -c	$(DIR_EPD)/2in9v2/epd_2in9v2.cpp -o $(DIR_BIN)/epd_2in9v2.o
NPI_DEV_C += $(DIR_BIN)/npi_neo3.o $(DIR_BIN)/sysfs_software_spi.o


clean :
	rm $(DIR_BIN)/*.* 
	rm $(TARGET) 


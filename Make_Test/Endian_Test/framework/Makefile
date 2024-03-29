# Warning with empty module
ifeq ($(MODULE),)
	$(error export MODULE before starting tests: example export MODULE=gpio ...)
endif	

ifeq ($(TEST),)
	$(error export TEST before starting tests: example export TEST=gpio_TS_001 ...)
endif

# include make test suite
include ../test/test_$(MODULE)/make/$(TEST)

PRO_DIR := .
PATH_OUTPUT = ../output
PROJ_NAME := $(MODULE)

INC_DIRS += $(PRO_DIR)/include
SRC_DIRS += $(PRO_DIR)/source

SRC_FILES := $(foreach SRC_DIRS, $(SRC_DIRS),$(wildcard $(SRC_DIRS)/*))
INC_FILES := $(foreach INC_DIRS, $(INC_DIRS),$(wildcard $(INC_DIRS)/*))

LINKER_FILE := $(PRO_DIR)/linker/stm_ls.ld
COMPILER_DIR := C:/GNU_Toolchain/GCC_NEW

# Compiler and linker flags
CC := $(COMPILER_DIR)/bin/arm-none-eabi-gcc
LD := $(COMPILER_DIR)/bin/arm-none-eabi-ld

INC_DIRS_OPT := $(foreach INC_DIRS,$(INC_DIRS),-I$(INC_DIRS))

# Compiler and linker option
CC_OPT := -mcpu=cortex-m4 -c -std=gnu11 -o0 -g -mthumb $(INC_DIRS_OPT)
LD_OPT := -T $(LINKER_FILE) -Map $(PATH_OUTPUT)/$(PROJ_NAME).map

OJB_FILES := $(notdir $(SRC_FILES))
OJB_FILES := $(subst .c,.o,$(OJB_FILES))
PATH_OJBS := $(foreach OJB_FILES,$(OJB_FILES),$(PATH_OUTPUT)/$(OJB_FILES))

vpath %.c $(SRC_DIRS)
vpath %.h $(INC_DIRS)

build: $(OJB_FILES) $(LINKER_FILE)
	$(LD) $(LD_OPT) $(PATH_OJBS) -o $(PATH_OUTPUT)/$(PROJ_NAME).elf
	$(COMPILER_DIR)/arm-none-eabi/bin/objcopy.exe -O ihex "$(PATH_OUTPUT)/$(PROJ_NAME).elf" "$(PATH_OUTPUT)/$(PROJ_NAME).hex"
	size $(PATH_OUTPUT)/$(PROJ_NAME).elf
	
%.o: %.c $(INC_FILES)
	$(CC) $(CC_OPT) -c $< -o $(PATH_OUTPUT)/$@
	
.PHONY:clean
clean:
	rm -rf $(PATH_OUTPUT)/*.o $(PATH_OUTPUT)/*.elf $(PATH_OUTPUT)/*.hex $(PATH_OUTPUT)/*.bin $(PATH_OUTPUT)/*.log $(PATH_OUTPUT)/*.map

.PHONY:run
run:
	$(PRO_DIR)/ST-LINKUtility/ST-LINK_CLI.exe -ME
	$(PRO_DIR)/ST-LINKUtility/ST-LINK_CLI.exe -p "../output/$(PROJ_NAME).hex" 0x08000000
	$(PRO_DIR)/ST-LINKUtility/ST-LINK_CLI.exe -rst
	
	# remove report file
	#rm -fr ../output/$(TEST).xml
	rm -fr $(PATH_OUTPUT)/result.log
	
	# read data from <TEST> address to check test case -> export result to log file
	@$(PRO_DIR)/ST-LINKUtility/ST-LINK_CLI.exe -r32 0x20000000 4 | grep -i 00000000 > $(PATH_OUTPUT)/result.log
	

.PHONY:report
report:
	if [[ -s $(PATH_OUTPUT)/result.log ]]; then \
		echo "Test $(TEST) is PASSED "; \
		cp $(PRO_DIR)/temp_report/Pass/Template.xml ../output/$(TEST).xml; \
		sed -i 's/TEST_NAME/$(TEST)/g' ../output/$(TEST).xml; \
		sed -i 's/TEST_DISCRIPTION/$(TEST_DETAILS)/g' ../output/$(TEST).xml; \
	else \
		echo "Test $(TEST) is FAILED "; \
		cp $(PRO_DIR)/temp_report/Fail/Template.xml ../output/$(TEST).xml; \
		sed -i 's/TEST_NAME/$(TEST)/g' ../output/$(TEST).xml; \
		sed -i 's/TEST_DISCRIPTION/$(TEST_DETAILS)/g' ../output/$(TEST).xml; \
	fi
	@$(PRO_DIR)/ST-LINKUtility/ST-LINK_CLI.exe -rst
# list all test suite	
.PHONY:info
info:	
	@ls ../test/test_$(MODULE)/make
	
print-%:
	@echo $($(subst print-,,$@))

py: ./python/xmldata.py
	python $<

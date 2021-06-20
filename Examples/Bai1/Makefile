INC_DIRS = D:/FPT/GAM_DAP_Traning/Makefile/Examples/Bai1/include
SRC_DIRS = D:\FPT\GAM_DAP_Traning\Makefile\Examples\Bai1\source
OUT_DIRS = D:\FPT\GAM_DAP_Traning\Makefile\Examples\Bai1\output

PATH_OUTPUT = output

# result SRC_FILES: 
#	D:\FPT\GAM_DAP_Traning\Makefile\Examples\Bai1\source\main.c
# 	D:\FPT\GAM_DAP_Traning\Makefile\Examples\Bai1\source\sum.c
SRC_FILES := $(foreach SRC_DIRS,$(SRC_DIRS),$(wildcard $(SRC_DIRS)/*))
INC_FILES := $(foreach INC_DIRS,$(INC_DIRS),$(wildcard $(INC_DIRS)/*))

# result OJB_FILES:
# 	main.c sum.c
OJB_FILES := $(notdir $(SRC_FILES))	

# result OJB_FILES:
#	main.o sum.o
OJB_FILES := $(subst .c,.o,$(OJB_FILES))

PATH_OJB := $(foreach OJB_FILES,$(OJB_FILES),$(PATH_OUTPUT)/$(OJB_FILES))

# search path .c and .h files 
vpath %.c $(SRC_DIRS)
vpath %.h $(INC_DIRS)

all: $(OJB_FILES) build run

%.o: %.c $(INC_FILES)
	@gcc -I$(INC_DIRS) -c $< -o $(PATH_OUTPUT)/$@
	
build: $(PATH_OJB)
	@gcc $^ -o $(OUT_DIRS)/main.exe
	
run:
	@$(OUT_DIRS)/main.exe
	
clean: 
	@rm ./output/*
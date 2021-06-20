I continuously complete makefile project (Endian_Test Project)
I try to create a project as a common template for driver projects
** Project have 4 main folders:   
 - MODULE has source codes of driver modules, for examples: gpio, flash, ...
 - FRAMEWORK contains fixed files, used for any projects of a microcontroller family
 - TEST contains test suites and test cases
 - OUTPUT contains object files, binary files, test output & report

(P)_____DRIVER__________GPIO__________INCLUDE___gpio.h
   |	      	   |	         |_____SOURCE___gpio.c
   |		   |___ + <other driver modules>
   |
   |___FRAMEWORK_______INCLUDE_________result.h 	 <test header>
   |               |____SOURCE_________result.c 	 <test source>
   |               |              |____stm32_startup.c 	 <startup file>
   |		   |____LINKER__________stm_ls.ld 	 <linker script file>
   |               |_ST-LINKUTILITY____<files to download FLASH>
   |               |___TEMP_REPORT_____<Template XML file for test report>
   |               |____PYTHON_________xmldata.py	 <Export test report>
   |               |___MAKEFILE
   |
   |_____TEST_________TEST_GPIO________gpio_TS_001_____test_suite__ <include + source>
   |               |               |                |___test_case__ <include + source>
   |               |               |___ + <other test suites> 
   |               |               |_______MAKE_____ <makefiles for test suites>
   |               |___+ <other test modules> 
   |
   |_____OUTPUT_____ <output files contains:__ object files, map file, log file
    					   |__ elf/hex/bin files
                                           |__ xml file (test suite output)
					   |__ csv file (test report)


  


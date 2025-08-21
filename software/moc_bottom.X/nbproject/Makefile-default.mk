#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/moc_bottom.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/moc_bottom.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/main.cpp C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/timer.c C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/ledandcolor.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/797313709/main.o ${OBJECTDIR}/_ext/797313709/timer.o ${OBJECTDIR}/_ext/797313709/ledandcolor.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/797313709/main.o.d ${OBJECTDIR}/_ext/797313709/timer.o.d ${OBJECTDIR}/_ext/797313709/ledandcolor.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/797313709/main.o ${OBJECTDIR}/_ext/797313709/timer.o ${OBJECTDIR}/_ext/797313709/ledandcolor.o

# Source Files
SOURCEFILES=C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/main.cpp C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/timer.c C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/ledandcolor.c

# Pack Options 
PACK_COMPILER_OPTIONS=-I "${DFP_DIR}/include"
PACK_COMMON_OPTIONS=-B "${DFP_DIR}/gcc/dev/attiny1626"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/moc_bottom.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATtiny1626
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/797313709/timer.o: C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/timer.c  .generated_files/flags/default/931e35d8bab138e02b4e6887e992f4e6ac31ea0e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/797313709" 
	@${RM} ${OBJECTDIR}/_ext/797313709/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/797313709/timer.o 
	 ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1626 ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS} -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/_ext/797313709/timer.o.d" -MT "${OBJECTDIR}/_ext/797313709/timer.o.d" -MT ${OBJECTDIR}/_ext/797313709/timer.o  -o ${OBJECTDIR}/_ext/797313709/timer.o C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/timer.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/797313709/ledandcolor.o: C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/ledandcolor.c  .generated_files/flags/default/34a6ae4b6ded9c0af9b187472b7599babf228c38 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/797313709" 
	@${RM} ${OBJECTDIR}/_ext/797313709/ledandcolor.o.d 
	@${RM} ${OBJECTDIR}/_ext/797313709/ledandcolor.o 
	 ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1626 ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS} -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/_ext/797313709/ledandcolor.o.d" -MT "${OBJECTDIR}/_ext/797313709/ledandcolor.o.d" -MT ${OBJECTDIR}/_ext/797313709/ledandcolor.o  -o ${OBJECTDIR}/_ext/797313709/ledandcolor.o C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/ledandcolor.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/_ext/797313709/timer.o: C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/timer.c  .generated_files/flags/default/268dccb731f3e13740750d77bb8549971d55c21f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/797313709" 
	@${RM} ${OBJECTDIR}/_ext/797313709/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/797313709/timer.o 
	 ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1626 ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS}  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/_ext/797313709/timer.o.d" -MT "${OBJECTDIR}/_ext/797313709/timer.o.d" -MT ${OBJECTDIR}/_ext/797313709/timer.o  -o ${OBJECTDIR}/_ext/797313709/timer.o C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/timer.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/797313709/ledandcolor.o: C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/ledandcolor.c  .generated_files/flags/default/3a633037f4fecda7750edb9bab22ad003b80ba7f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/797313709" 
	@${RM} ${OBJECTDIR}/_ext/797313709/ledandcolor.o.d 
	@${RM} ${OBJECTDIR}/_ext/797313709/ledandcolor.o 
	 ${MP_CPPC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1626 ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS}  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/_ext/797313709/ledandcolor.o.d" -MT "${OBJECTDIR}/_ext/797313709/ledandcolor.o.d" -MT ${OBJECTDIR}/_ext/797313709/ledandcolor.o  -o ${OBJECTDIR}/_ext/797313709/ledandcolor.o C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/ledandcolor.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/797313709/main.o: C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/main.cpp  .generated_files/flags/default/90919ec0349ffba14d5c0c834e2a4452cb12ca1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/797313709" 
	@${RM} ${OBJECTDIR}/_ext/797313709/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/797313709/main.o 
	 ${MP_CPPC} $(MP_EXTRA_CC_PRE) -mmcu=attiny1626 ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS} -g -DDEBUG  -gdwarf-2  -x c++ -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/_ext/797313709/main.o.d" -MT "${OBJECTDIR}/_ext/797313709/main.o.d" -MT ${OBJECTDIR}/_ext/797313709/main.o  -o ${OBJECTDIR}/_ext/797313709/main.o C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/main.cpp  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/_ext/797313709/main.o: C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/main.cpp  .generated_files/flags/default/9cd4e7c900ec6ffdf4b3d59940e7d6dea08e8b40 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/797313709" 
	@${RM} ${OBJECTDIR}/_ext/797313709/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/797313709/main.o 
	 ${MP_CPPC} $(MP_EXTRA_CC_PRE) -mmcu=attiny1626 ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS}  -x c++ -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/_ext/797313709/main.o.d" -MT "${OBJECTDIR}/_ext/797313709/main.o.d" -MT ${OBJECTDIR}/_ext/797313709/main.o  -o ${OBJECTDIR}/_ext/797313709/main.o C:/Users/tiri/Documents/GitHub/a3-motion/software/moc_bottom.X/src/main.cpp  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/moc_bottom.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CPPC} $(MP_EXTRA_LD_PRE) -mmcu=attiny1626 ${PACK_COMMON_OPTIONS}   -gdwarf-2 -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}\moc_bottom.X.${IMAGE_TYPE}.map"    -o ${DISTDIR}/moc_bottom.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	
	
	
	
	
	
else
${DISTDIR}/moc_bottom.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CPPC} $(MP_EXTRA_LD_PRE) -mmcu=attiny1626 ${PACK_COMMON_OPTIONS}  -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}\moc_bottom.X.${IMAGE_TYPE}.map"    -o ${DISTDIR}/moc_bottom.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}\\avr-objcopy -O ihex "${DISTDIR}/moc_bottom.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/moc_bottom.X.${IMAGE_TYPE}.hex"
	
	
	
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

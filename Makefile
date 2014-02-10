# Copyright {c} 2014 Josh Friend
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files {the "Software"}, to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

#==============================================================================
#     Project properties
#==============================================================================

# Project name
PROJECT_NAME = demo

# Project executable target
EXE = ${ARTIFACTS_DIR}/${PROJECT_NAME}.axf

# Set build artifacts
ARTIFACTS_DIR = build
ARTIFACTS = ${EXE}                                \
            ${ARTIFACTS_DIR}/${PROJECT_NAME}.bin  \
            ${ARTIFACTS_DIR}/${PROJECT_NAME}.lst  \
            ${ARTIFACTS_DIR}/${PROJECT_NAME}.size \
            ${ARTIFACTS_DIR}/${PROJECT_NAME}.map

#==============================================================================
#     Target properties
#==============================================================================

# Microcontroller properties.
PART=LM4F120H5QR

#==============================================================================
#    Make rules
#==============================================================================

all: ${LIBDRIVER_PATH} ${ARTIFACTS}

ifneq ($(MAKECMDGOALS),clean)
# Contains compile rules and toolchain settings
include makedefs
endif

# TI Stellaris/Tivia library
${LIBDRIVER_PATH}:
	@echo Making driverlib...
	make -C ${DRIVERLIB_PATH}

# Project executable
.NOTPARALLEL:
${EXE}: ${OBJS} ${LIBDRIVER_PATH}
	@mkdir -p ${dir $@}
	@if [ 'x${VERBOSE}' = x ];          \
	 then                               \
	     echo "LD  $@";                 \
	 else                               \
	     echo ${LD} ${LFLAGS} -o $@ $^; \
	 fi
	@${LD} ${LFLAGS} -o $@ $^

# Clean compiled files
.NOTPARALLEL:
.PHONY: clean
clean:
	rm -rf ${ARTIFACTS_DIR}

# Create GDB command file
.NOTPARALLEL:
debug.gdbcmd: ${EXE}
	@echo file ${EXE} > $@
	@echo set tdesc filename target.xml >> $@
	@echo target remote localhost:3333 >> $@
	@echo monitor reset halt >> $@
	@echo load >> $@

# Create GDB command file
.NOTPARALLEL:
flash.gdbcmd: ${EXE}
	@echo file ${EXE} > $@
	@echo set tdesc filename target.xml >> $@
	@echo target remote localhost:3333 >> $@
	@echo monitor reset halt >> $@
	@echo load >> $@
	@echo quit >> $@

# Run a debug session
.NOTPARALLEL:
.PHONY: debug
debug: all debug.gdbcmd
	@echo Opening GDB...
	@${GDB} -x debug.gdbcmd

# Flash target
.NOTPARALLEL:
.PHONY: flash
flash: all flash.gdbcmd
	@echo Opening GDB...
	@${GDB} -x flash.gdbcmd

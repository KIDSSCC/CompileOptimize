SRC_PATH ?= src
INC_PATH += include
BUILD_PATH ?= build
TEST_PATH ?= test/all
OBJ_PATH ?= $(BUILD_PATH)/obj
BINARY ?= $(BUILD_PATH)/compiler
SYSLIB_PATH ?= sysyruntimelibrary

INC = $(addprefix -I, $(INC_PATH))
SRC = $(shell find $(SRC_PATH)  -name "*.cpp")
CFLAGS = -O0 -g -Wall -std=c++11 $(INC)
FLEX ?= $(SRC_PATH)/lexer.l
LEXER ?= $(addsuffix .cpp, $(basename $(FLEX)))
BISON ?= $(SRC_PATH)/parser.y
PARSER ?= $(addsuffix .cpp, $(basename $(BISON)))
SRC += $(LEXER)
SRC += $(PARSER)

#在SRC资源目录下，首先添加了src文件夹下所有的.cpp文件，随后又将词法分析和语法分析的lexer.l和paeser.y转换为了.cpp文件，添加到了资源目录下
OBJ = $(SRC:$(SRC_PATH)/%.cpp=$(OBJ_PATH)/%.o)

#parser.h文件路径
PARSERH ?= $(INC_PATH)/$(addsuffix .h, $(notdir $(basename $(PARSER))))

TESTCASE = $(shell find $(TEST_PATH) -name "*.sy")
TESTCASE_NUM = $(words $(TESTCASE))
LLVM_IR = $(addsuffix _std.ll, $(basename $(TESTCASE)))
GCC_ASM = $(addsuffix _std.s, $(basename $(TESTCASE)))

#各个实验的阶段性输出，词法单元，ast节点树，llvm中间代码，.s汇编代码，.res运行结果,.bin应该是二进制文件，.log为输出日志
OUTPUT_LAB4 = $(addsuffix .toks, $(basename $(TESTCASE)))
OUTPUT_LAB5 = $(addsuffix .ast, $(basename $(TESTCASE)))
OUTPUT_LAB6 = $(addsuffix .ll, $(basename $(TESTCASE)))
OUTPUT_LAB7 = $(addsuffix .s, $(basename $(TESTCASE)))
OUTPUT_RES = $(addsuffix .res, $(basename $(TESTCASE)))
OUTPUT_BIN = $(addsuffix .bin, $(basename $(TESTCASE)))
OUTPUT_LOG = $(addsuffix .log, $(basename $(TESTCASE)))

.phony:all app run gdb testlab4 testlab5 testlab6 testlab7 test clean clean-all clean-test clean-app llvmir gccasm

all:app

# 词法分析器
# 将flex -o lexer.cpp lexer.l
$(LEXER):$(FLEX)
	@flex -o $@ $<

# 语法分析器
#生成parser.h和parser.cpp
$(PARSER):$(BISON)
	@bison -o $@ $< --warnings=error=all --defines=$(PARSERH)

# 采用g++编译器，将所有的src路径下的编译器文件编译为.o文件
$(OBJ_PATH)/%.o:$(SRC_PATH)/%.cpp
	@mkdir -p $(OBJ_PATH)
	@clang++ $(CFLAGS) -c -o $@ $<

# 采用g++编译器，.o文件链接并生成二进制编译器compile
$(BINARY):$(OBJ)
	@clang++ -O0 -g -o $@ $^

app:$(LEXER) $(PARSER) $(BINARY)

# make run命令，编译并运行example.sy文件
# 汇编文件采用arm-linux-gnueabihf-gcc进行汇编链接
run:app
	@$(BINARY) -S -o example.s example.sy -O1
	arm-linux-gnueabihf-gcc example.s $(SYSLIB_PATH)/libsysy.a -o example
	qemu-arm -L /usr/arm-linux-gnueabihf/ ./example
	echo $$?

# make timetest命令，将arm.s文件汇编运行500次，统计运行实践
timetest:app
	@arm-linux-gnueabihf-gcc arm.s $(SYSLIB_PATH)/libsysy.a -o arm
	@/usr/bin/time -f "runningtime: %E" sh -c 'for i in $$(seq 1 500); do qemu-arm -L /usr/arm-linux-gnueabihf/ ./arm; done'

# make run1命令，汇编并执行arm.s文件
run1:app
	@arm-linux-gnueabihf-gcc arm.s $(SYSLIB_PATH)/libsysy.a -o arm
	@qemu-arm -L /usr/arm-linux-gnueabihf/ ./arm
	@echo $$?

# make arm文件，将arm.c文件编译为arm.s文件
arm:
	@arm-linux-gnueabihf-gcc -o arm.s -S -O0 arm.c -fno-asynchronous-unwind-tables


runes:app
	@qemu-arm -L /usr/arm-linux-gnueabihf/ ./example
	echo $$?

gdb:app
	@gdb $(BINARY)

#lexer.cpp生成lexer.o
$(OBJ_PATH)/lexer.o:$(SRC_PATH)/lexer.cpp
	@mkdir -p $(OBJ_PATH)
	@g++ $(CFLAGS) -c -o $@ $<

$(TEST_PATH)/%.toks:$(TEST_PATH)/%.sy
	@$(BINARY) $< -o $@ -t

$(TEST_PATH)/%.ast:$(TEST_PATH)/%.sy
	@$(BINARY) $< -o $@ -a

$(TEST_PATH)/%.ll:$(TEST_PATH)/%.sy
	@$(BINARY) $< -o $@ -i	

$(TEST_PATH)/%_std.ll:$(TEST_PATH)/%.sy
	@clang -x c $< -S -m32 -emit-llvm -o $@ 

$(TEST_PATH)/%_std.s:$(TEST_PATH)/%.sy
	@arm-linux-gnueabihf-gcc -x c $< -S -o $@ 

$(TEST_PATH)/%.s:$(TEST_PATH)/%.sy
	@timeout 10s $(BINARY) $< -o $@ -S 2>$(addsuffix .log, $(basename $@))
	@[ $$? != 0 ] && echo "\033[1;31mCOMPILE FAIL:\033[0m $(notdir $<)" || echo "\033[1;32mCOMPILE SUCCESS:\033[0m $(notdir $<)"

llvmir:$(LLVM_IR)

gccasm:$(GCC_ASM)

testlab4:app $(OUTPUT_LAB4)

testlab5:app $(OUTPUT_LAB5)

testlab6:app $(OUTPUT_LAB6)

testlab7:app $(OUTPUT_LAB7)

.ONESHELL:
test:app
	@success=0
	@for file in $(sort $(TESTCASE))
	do
		ASM=$${file%.*}.s
		LOG=$${file%.*}.log
		BIN=$${file%.*}.bin
		RES=$${file%.*}.res
		IN=$${file%.*}.in
		OUT=$${file%.*}.out
		FILE=$${file##*/}
		FILE=$${FILE%.*}

		# 高级语言至汇编语言过程
		# $(BINARY) $${file} -o $${ASM} -S 2>$${LOG}
		$(BINARY) -S -o $${ASM} $${file} -O1 2>$${LOG}
		RETURN_VALUE=$$?
		if [ $$RETURN_VALUE = 124 ]; then
			echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mCompile Timeout\033[0m"
			continue
		else if [ $$RETURN_VALUE != 0 ]; then
			echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mCompile Error\033[0m"
			continue
			fi
		fi

		# 汇编语言至可执行文件过程
		arm-linux-gnueabihf-gcc -mcpu=cortex-a72 -o $${BIN} $${ASM} $(SYSLIB_PATH)/libsysy.a >>$${LOG} 2>&1
		if [ $$? != 0 ]; then
			echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mAssemble Error\033[0m"
		else
			# 开始执行
			if [ -f "$${IN}" ]; then
				timeout 10s qemu-arm -L /usr/arm-linux-gnueabihf $${BIN} <$${IN} >$${RES} 2>>$${LOG}
			else
				timeout 10s qemu-arm -L /usr/arm-linux-gnueabihf $${BIN} >$${RES} 2>>$${LOG}
			fi

			# 输出结果文件到.res
			RETURN_VALUE=$$?
			FINAL=`tail -c 1 $${RES}`
			[ $${FINAL} ] && echo "\n$${RETURN_VALUE}" >> $${RES} || echo "$${RETURN_VALUE}" >> $${RES}
			
			if [ "$${RETURN_VALUE}" = "124" ]; then
				echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mExecute Timeout\033[0m"
			else if [ "$${RETURN_VALUE}" = "127" ]; then
				echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mExecute Error\033[0m"
				else
					diff -Z $${RES} $${OUT} >/dev/null 2>&1
					if [ $$? != 0 ]; then
						echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mWrong Answer\033[0m"
					else
						success=$$((success + 1))
						echo "\033[1;32mPASS:\033[0m $${FILE}"
					fi
				fi
			fi
		fi
	done
	echo "\033[1;33mTotal: $(TESTCASE_NUM)\t\033[1;32mAccept: $${success}\t\033[1;31mFail: $$(($(TESTCASE_NUM) - $${success}))\033[0m"
	[ $(TESTCASE_NUM) = $${success} ] && echo "\033[5;32mAll Accepted. Congratulations!\033[0m"
	:

clean-app:
	@rm -rf $(BUILD_PATH) $(PARSER) $(LEXER) $(PARSERH)

clean-test:
	@rm -rf $(OUTPUT_LAB4) $(OUTPUT_LAB5) $(OUTPUT_LAB6) $(OUTPUT_LAB7) $(OUTPUT_LOG) $(OUTPUT_BIN) $(OUTPUT_RES) $(LLVM_IR) $(GCC_ASM) ./example.ast ./example.ll ./example.s

clean-all:clean-test clean-app

clean:clean-all
	@rm -rf example.toks example.res example.bin

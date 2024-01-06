TARGET	:=	run
DTARGET	:=	run-g
SRC		:=	main.cpp normal.cpp boosttest.cpp
HDRS	:=	main.hpp normal.hpp boosttest.hpp template.hpp
CXX		:=	C:/ProgramData/MinGW/bin/g++.exe
BLIBS	:=	-lboost_regex-mgw63-mt-1_65_1 -llibboost_filesystem-mgw63-1_65_1 -llibboost_system-mgw63-1_65_1
CLANG	:=	C:/synopsys_longc/LLVM/bin/clang.exe

INCPATH		:=	"-IC:/synopsys_longc/boost_1_65_1"
LINKPATH	:=	"-LC:/synopsys_longc/boost_1_65_1/stage/lib"

$(TARGET) : $(SRC) $(HDRS)
	g++ -std=gnu++14 -o $(TARGET) $(SRC) $(LINKPATH) $(INCPATH) $(BLIBS)

debug : $(SRC) $(HDRS)
	g++ -std=gnu++14 -g -DDEBUG -o $(DTARGET) $(SRC) $(LINKPATH) $(INCPATH) $(BLIBS)

clang-target : $(SRC) $(HDRS)
	clang -std=gnu++14 -o $(TARGET) $(SRC) $(LINKPATH) $(INCPATH) $(BLIBS)

# Makefile for TestFtpStack
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2019/02/12

include ../Makefile.mk

INC=-I../FtpStack -I../SipPlatform
LIB=-L../FtpStack -lftpstack \
	-L../SipPlatform -lsipplatform \
	-lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestFtpStack.exe

SRV_OBJ=TestFtpStack.o

TestFtpStack.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*


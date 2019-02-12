# Makefile for all of C++ FtpStack
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2019/02/12

all:
	cd SipPlatform && make
	cd FtpStack && make
	cd TestFtpStack && make

clean:
	cd SipPlatform && make clean
	cd FtpStack && make clean
	cd TestFtpStack && make clean

install:


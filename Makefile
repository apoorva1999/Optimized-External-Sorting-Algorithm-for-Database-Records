CPPOPT=-g -Og -D_DEBUG -O0
# -O2 -Os -Ofast
# -fprofile-generate -fprofile-use
CPPFLAGS=$(CPPOPT) -ansi -pedantic -std=c++20 
# -Wparentheses -Wno-unused-parameter -Wformat-security
# -fno-rtti -std=c++11 -std=c++98

# documents and scripts
DOCS=Tasks.txt
SCRS=

# headers and code sources
HDRS=	defs.h \
		Iterator.h Scan.h Filter.h Sort.h Witness.h TournamentTree.h InternalSort.h Disk.h ExternalSort.h 
SRCS=	defs.cpp Assert.cpp Test.cpp \
		Iterator.cpp Scan.cpp Filter.cpp Sort.cpp Witness.cpp  TournamentTree.cpp InternalSort.cpp Disk.cpp ExternalSort.cpp
	

# compilation targets
OBJS=	defs.o Assert.o Test.o \
		Iterator.o Scan.o Filter.o Sort.o Witness.o TournamentTree.o InternalSort.o Disk.o ExternalSort.o


# RCS assists
REV=-q -f
MSG=no message

# default target
#
Test.exe : Makefile $(OBJS)
	g++ $(CPPFLAGS) -o Test.exe $(OBJS)
generateData.exe : Makefile $(OBJS)
	g++ $(CPPFLAGS) -o generateData.exe $(OBJS)
trace : a.out Makefile
	@date > trace
	@size -t a.out $(OBJS) | sort -r >> trace
	./a.out >> trace

$(OBJS) : Makefile defs.h
Test.o : Iterator.h Scan.h Filter.h Sort.h Witness.h
Iterator.o Scan.o Filter.o Sort.o : Iterator.h
Scan.o : Scan.h
Filter.o : Filter.h
Sort.o : Sort.h
Witness.o : Witness.h

list : Makefile
	echo Makefile $(HDRS) $(SRCS) $(DOCS) $(SCRS) > list
count : list
	@wc `cat list`

ci :
	ci $(REV) -m"$(MSG)" $(HDRS) $(SRCS) $(DOCS) $(SCRS)
	ci -l $(REV) -m"$(MSG)" Makefile
co :
	co $(REV) -l $(HDRS) $(SRCS) $(DOCS) $(SCRS)

clean :
	@rm -f $(OBJS) a.out a.out.stackdump trace 
	@rm -rf pass_*

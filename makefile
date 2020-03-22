#
#	File:	makefile
#
#	(C)opyright 1987-1992 InfoTaskforce.
#

OBJECTS =	enhanced.o file.o fns.o globals.o infocom.o init.o input.o\
			interp.o io.o jump.o message.o object.o options.o\
			page.o plus_fns.o print.o property.o status.o\
			support.o variable.o

#
# Compile options are described below:
#
# AMIGAOS		: Compile on a Commodore Amiga.
# ANSI_ESCAPE	: Compile a version that uses ANSI ESCAPE sequences for
#				  terminal output.
# ANSI_COLOR	: Compile a version that uses ANSI ESCAPE sequences with
#				  color handling for terminal output.
#				  (Works under both MS-DOS and UNIX!)
# BSD			: Compile on a BSD UNIX machine. (This also defines "UNIX")
# CURSES		: Compile a version that uses curses features for
#				  terminal output. You may also have to include a
#				  curses library as well.
# CURSES_COLOR	: Compile a version that uses the color capabilities of
#				  UNIX System V R3 curses features for terminal output.
# LSC			: Compile on a Macintosh using LightSpeed C Version 2.01.
# MSC			: Compile on an MS-DOS machine using Microsoft C.
# MSDOS			: Compile on an MS-DOS machine.
# SYS_V			: Compile on a UNIX System V machine. (This also defines "UNIX")
# TERMCAP		: Compile a version that uses termcap features for
#				  terminal output. You may also have to include a
#				  termcap library as well.
# THINKC		: Compile on a Macintosh using THINK C Version 4.0.
# TURBOC		: Compile on an MS-DOS machine using Borland C or Turbo C.
# UNIX			: Compile on a UNIX machine.
#

#
#	Uncomment the following lines for compiling with Turbo C or
#	Borland C under MS-DOS.
#
#.SUFFIXES:	.exe .o .c
#
#CC =		bcc
#CFLAGS =	-DTURBOC -m$(MODEL) -G -O -Z
#MODEL =	c
#TC_DIR =	c:\bc
#TC_BIN =	$(TC_DIR)\bin
#
#.c.o:
#	$(TC_BIN)\$(CC) $(CFLAGS) -c -o$@ $*.c
#
#turboc:	$(OBJECTS) infocom.rc
#	echo $(TC_DIR)\lib\c0$(MODEL).obj + > info.cmd
#	echo enhanced.o file.o fns.o globals.o infocom.o init.o + >> info.cmd
#	echo input.o interp.o io.o jump.o message.o object.o + >> info.cmd
#	echo options.o page.o plus_fns.o print.o property.o + >> info.cmd
#	echo status.o support.o variable.o + >> info.cmd
#	echo ,infocom.exe,nul.map,$(TC_DIR)\lib\c$(MODEL).lib >> info.cmd
#	$(TC_BIN)\tlink @info.cmd
#	-rm info.cmd
#

infocom.rc:		makefile
	@echo # Default initialization file for MS-DOS machines > $@
	@echo # This overrides the number of screen rows automagically determined >> $@
	@echo # height 25 >> $@
	@echo # This overrides the number of screen columns automagically determined >> $@
	@echo # width 80 >> $@
	@echo # Attributes are specified in this order: >> $@
	@echo # normal, inverse, bold, inverse&bold, underline, inverse&underline, >> $@
	@echo # bold&underline, inverse&bold&underline >> $@
	@echo # The following attributes may be specified: >> $@
	@echo # normal, high, low, italic, underline, blink, fastblink, reverse >> $@
	@echo # f_black, f_red, f_green, f_yellow, f_blue, f_magenta, f_cyan, f_white >> $@
	@echo # b_black, b_red, b_green, b_yellow, b_blue, b_magenta, b_cyan, b_white >> $@
	@echo # Not all attributes may be supported >> $@
	@echo attr b_blue f_white >> $@
	@echo attr b_white f_blue >> $@
	@echo attr b_blue f_white high >> $@
	@echo attr b_white f_white high >> $@
	@echo attr b_blue f_red blink >> $@
	@echo attr b_white f_red >> $@
	@echo attr b_blue f_red high >> $@
	@echo attr b_white f_red high >> $@

.infocomrc:		makefile
	@echo '# Default initialization file for UNIX machines' > $@
	@echo '# This overrides the number of screen rows' >> $@
	@echo '# height 25' >> $@
	@echo '# This overrides the number of screen columns' >> $@
	@echo '# width 80' >> $@
	@echo '# This allows use of ANSI save/restore cursor features' >> $@
	@echo '# save' >> $@
	@echo '# Attributes are specified in this order:' >> $@
	@echo '# normal, inverse, bold, inverse&bold, underline, inverse&underline,' >> $@
	@echo '# bold&underline, inverse&bold&underline' >> $@
	@echo '# The following attributes may be specified:' >> $@
	@echo '# normal, high, low, italic, underline, blink, fastblink, reverse' >> $@
	@echo '# f_black, f_red, f_green, f_yellow, f_blue, f_magenta, f_cyan, f_white' >> $@
	@echo '# b_black, b_red, b_green, b_yellow, b_blue, b_magenta, b_cyan, b_white' >> $@
	@echo '# Not all attributes may be supported' >> $@
	@echo 'attr b_blue f_white' >> $@
	@echo 'attr b_white f_blue' >> $@
	@echo 'attr b_blue f_white high' >> $@
	@echo 'attr b_white f_white high' >> $@
	@echo 'attr b_blue f_red blink' >> $@
	@echo 'attr b_white f_red' >> $@
	@echo 'attr b_blue f_red high' >> $@
	@echo 'attr b_white f_red high' >> $@

#
#	For MSDOS Compilers, use either the Compact, Large or Huge Memory
#	Models ( these all use far data pointers ) because Standard Library
#	Functions such as "fread ()" are passed a buffer as a huge pointer.
#	Tiny, Small and Medium Models only allow near pointers in Standard
#	Library Function Parameters.
#
#	Uncomment the following lines for compiling with Microsoft C.
#
#.SUFFIXES:	.exe .o .c
#
#CC =		cl
#
#.c.o:
#	$(CC) $(CFLAGS) -DMSC -Fo$*.o -c $*.c
#

qc:
	make "CFLAGS=-AC -qc" "LIBS=graphics.lib" _msdos

msc:
	make "CFLAGS=-AC -Ozax -D__STDC__ -W3" "LIBS=graphics.lib" _msdos

_msdos: $(OBJECTS) infocom.rc
	link /ST:10240 $(OBJECTS),infocom.exe,,$(LIBS);

CC=cc
OPTFLAG=-O

bsd_ansi:
	make CFLAGS='$(OPTFLAG) -DBSD -DANSI_ESCAPE' unix

bsd_ansic:
	make CFLAGS='$(OPTFLAG) -DBSD -DANSI_COLOR' unix

bsd_curses:
	make CFLAGS='$(OPTFLAG) -DBSD -DCURSES' LDFLAGS='-lcurses -ltermcap' unix

bsd:
	make CFLAGS='$(OPTFLAG) -DBSD -DTERMCAP' LDFLAGS='-ltermcap' unix

sysv_ansi:
	make CFLAGS='$(OPTFLAG) -DSYS_V -DANSI_ESCAPE' unix

sysv_ansic:
	make CFLAGS='$(OPTFLAG) -DSYS_V -DANSI_COLOR' unix

sysv_curses:
	make CFLAGS='$(OPTFLAG) -DSYS_V -DCURSES' LDFLAGS='-lcurses' unix

sysv_cursesc:
	make CFLAGS='$(OPTFLAG) -DSYS_V -DCURSES_COLOR' LDFLAGS='-lcurses' unix

sysv_termcap:
	make CFLAGS='$(OPTFLAG) -DSYS_V -DTERMCAP' LDFLAGS='-ltermlib' unix

sysv:
	make CFLAGS='$(OPTFLAG) -DSYS_V -DTERMINFO' LDFLAGS='-lcurses' unix

unix: $(OBJECTS)
	$(CC) $(OPTFLAG) -o infocom $(OBJECTS) $(LDFLAGS)

$(OBJECTS): infocom.h machine.h makefile

clean:
		-rm *.o
		-rm .infocomrc
		-rm infocom.rc
		-rm infocom

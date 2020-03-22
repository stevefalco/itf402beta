/*
**	File:	infocom.h
**
**	(C)opyright 1987-1992 InfoTaskforce.
*/

/*
**	INFOCOM INTERPRETER.
**	
**	(C)opyright InfoTaskforce.
**	4th April, 1988.
**
**	This version:
**					26th September, 1992.
**
**	"May the Grues be with you !"
**
*/

#ifndef	__INFOCOM__

#define		__INFOCOM__

#ifdef	__STDC__
#define		dclproto(func,args) func args ;
#else	/* __STDC__ */
#define		dclproto(func,args) func () ;
#endif	/* __STDC__ */

#ifdef	__STDC__
#include	<stdlib.h>
#endif	/* __STDC__ */

#include	<stdio.h>
#include	"machine.h"

/*
**	Universal Type Definitions.
**
**	'byte'			- 8 bits	; unsigned.
**	'word'			- 16 bits	; unsigned.
**	'long_word'		- 32 bits	; unsigned.
**
**	'signed_word'	- 16 bits	; signed.
**	'signed_long'	- 32 bits	; signed.
*/

typedef		unsigned char	byte ;
typedef		unsigned short	word ;
typedef		unsigned long	long_word ;
typedef		short			signed_word ;
typedef		long			signed_long ;

typedef		byte HUGE		*byte_ptr ;

typedef		char			boolean ;
typedef		Void			(*proc_ptr)() ;
typedef		byte_ptr		property ;

/*
**	Universal Constants
**
**	Note:
**		MAX_BYTES			= maximum RAM size ( in bytes - rounded to the
**							  nearest 512-byte boundary ) allocated by the
**							  standard "malloc" library function. This
**							  value assumes the arguement to "malloc" is
**							  a 16-bit unsigned int.
**		STACK_SIZE			= size in words
**		BLOCK_SIZE			= size in bytes
**		TABLE_SIZE			= size of white_space buffer
**		MAX_LINE_LENGTH		= maximum screen line size
*/

#ifndef	FALSE
#define		FALSE						(0)
#endif	/* FALSE */

#ifndef	TRUE
#define		TRUE						(!FALSE)
#endif	/* TRUE */

#define		BITS_PER_BYTE				((byte)8)
#define		BYTE_MASK					((byte)0xFF)

#define		BELL						((char)0x07)
#define		VERTICAL_TAB				((char)0x0B)
#define		LOCAL_VARS					((byte)0x10)
#define		MAX_PARAMS					((byte)0x08)

#define		MAX_BYTES					((word)0xFE00)
#define		STACK_SIZE					((word)0x0400)
#define		BLOCK_SIZE					((word)0x0200)
#define		TABLE_SIZE					((word)0x0100)
#define		MAX_LINE_LENGTH				((word)0x0200)

#define		MOST_SIGNIFICANT_BYTE(x)	(((word)(x) >> BITS_PER_BYTE) & BYTE_MASK)
#define		LEAST_SIGNIFICANT_BYTE(x)	((word)(x) & BYTE_MASK)

#define		VERSION_1					((byte)0x01)
#define		VERSION_2					((byte)0x02)
#define		VERSION_3					((byte)0x03)
#define		VERSION_4					((byte)0x04)
#define		VERSION_5					((byte)0x05)

#define		MAX_VERSION					VERSION_5

#define		PROCEDURE					((word)0)
#define		FUNCTION					((word)0x0100)

#define		STD_PAGE(x)					((word)(x) >> BITS_PER_BYTE)
#define		STD_OFFSET(x)				(((word)(x) & BYTE_MASK) << 1)
#define		STD_CHARS_PER_WORD			((word)0x06)
#define		STD_ENCODED_SIZE			((word)(STD_CHARS_PER_WORD/3))
#define		PLUS_PAGE(x)				((word)(x) >> 7)
#define		PLUS_OFFSET(x)				(((word)(x) & 0x7F) << 2)
#define		PLUS_CHARS_PER_WORD			((word)0x09)
#define		PLUS_ENCODED_SIZE			((word)(PLUS_CHARS_PER_WORD/3))

/*
**	Enhanced and Plus Series Windowing Definitions.
*/

#define		WINDOW_0					((word)0)
#define		WINDOW_1					((word)1)
#define		FULL_SCREEN					((word)2)

#define		STD_TOP_SCREEN_LINE			((word)1)
#define		PLUS_TOP_SCREEN_LINE		((word)0)

#define		NORMAL_FONT					((word)1)
#define		GRAPHICS_FONT				((word)3)

/*
**	Error Codes
*/

#define		ERR_MEMORY					((word)0x0000)
#define		ERR_OPCODE					((word)0x0015)
#define		ERR_HEADER					((word)0x0016)
#define		ERR_PUT_PROP				((word)0x0017)
#define		ERR_NEXT_PROP				((word)0x0018)

#define		PLUS_ERROR_1				((word)0x03E9)
#define		PLUS_ERROR_2				((word)0x03EA)

/*
**	Signal Handler States.
*/

#define		SH_EXIT						((int)-1)
#define		SH_INIT						((int)0)
#define		SH_NORMAL					((int)1)
#define		SH_NO_IO					((int)2)

#define		SH_NO_SIGNAL				((int)0x00)
#define		SH_CLOSE					((int)0x01)
#define		SH_IO						((int)0x02)
#define		SH_COREDUMP					((int)0x04)

/*
**	Command Line Options
*/

#define		NO_OPTIONS		((word)0x0000)	/*     No Options Selected        */
#define		OBJ_ATTR		((word)0x0001)	/* a = Monitor Object Attributes  */
#define		ECHO_INPUT		((word)0x0002)	/* e = Echo Input Characters      */
#define		HEAD_INFO		((word)0x0004)	/* h = Print Header Information   */
#define		OBJECTS			((word)0x0008)	/* o = Print Object / Room List   */
#define		NO_PAGE			((word)0x0010)	/* p = Disable pager              */
#define		TREE			((word)0x0020)	/* r = Print Object / Room Tree   */
#define		SHOW_PROPS		((word)0x0040)	/* s = Print Object Properties    */
#define		OBJ_XFER		((word)0x0080)	/* t = Monitor Object Transfers   */
#define		VOCABULARY		((word)0x0100)	/* v = Print Vocabulary Word List */
#define		EXTENDED_VOCAB	((word)0x0200)	/* x = Print Extended Vocabulary  */

/*
**	Object Structure Definition.
**
**	Note:
**		We want the size of a standard object to be 9 bytes, but some
**	compliers will only make it an integral number of WORDS. Thus
**	we have to explicitly define the size of an object structure.
*/

#define		STD_OBJ_SIZE				((byte)0x09)
#define		PLUS_OBJ_SIZE				(sizeof(plus_object))
#define		STD_OBJ_OFFSET				((byte)0x35)
#define		PLUS_OBJ_OFFSET				((byte)0x70)
#define		FIRST_ATTRIBUTE				((byte)0x80)

#define		STD_OBJ_ADDR(x)				((std_object_ptr)((byte_ptr)std_obj_list + ((x) * STD_OBJ_SIZE) + STD_OBJ_OFFSET))
#define		PLUS_OBJ_ADDR(x)			((plus_object_ptr)((byte_ptr)plus_obj_list + ((x) * PLUS_OBJ_SIZE) + PLUS_OBJ_OFFSET))

typedef struct
{
	byte	attributes[4] ;
	byte	location[1] ;
	byte	link[1] ;
	byte	holds[1] ;
	byte	prop_ptr[2] ;
} std_object ;

typedef struct
{
	byte	attributes[6] ;
	byte	location[2] ;
	byte	link[2] ;
	byte	holds[2] ;
	byte	prop_ptr[2] ;
} plus_object ;

typedef		std_object HUGE				*std_object_ptr ;
typedef		plus_object HUGE			*plus_object_ptr ;

/*
**	Property Mask Definitions.
*/

#define		STD_PROP_NUM_BITS			((int)5)
#define		PLUS_PROP_NUM_BITS			((int)6)
#define		STD_PROP_NUM_MASK			((byte)0x1F)
#define		PLUS_PROP_NUM_MASK			((byte)0x3F)
#define		NEXT_BYTE_IS_LENGTH			((byte)0x80)
#define		STD_WORD_MASK				((byte)0x20)
#define		PLUS_WORD_MASK				((byte)0x40)
#define		STD_PROPERTY_LENGTH(p)		((*(p) >> STD_PROP_NUM_BITS) + 1)
#define		PLUS_PROPERTY_LENGTH(p)		PLUS_PROPERTY_NUMBER(p)
#define		STD_PROPERTY_NUMBER(p)		(*(p) & STD_PROP_NUM_MASK)
#define		PLUS_PROPERTY_NUMBER(p)		(*(p) & PLUS_PROP_NUM_MASK)

/*
**	Page Table Definitions.
*/

#define		EMPTY_PAGE					((word)0xFFFE)
#define		END_OF_TABLE				((word)0xFFFF)
#define		MAX_COUNT					((long_word)0xFFFFFFFF)

typedef struct
{
	word		page ;
	long_word	count ;
	word		padding ;
} page_table_t ;

typedef		page_table_t HUGE			*page_table_ptr ;

/*
**	Infocom Game Header Structure.
*/

struct	header
{
	byte	z_code_version ;		/* Game's Z-CODE Version Number         */
	byte	mode_bits ;				/* Status Bar display indicator         */
	word	release ;				/* Game Release Number                  */
	word	resident_bytes ;		/* No. bytes in the Resident Area       */
	word	start ;					/* Offset to Start of Game              */
	word	vocab ;					/* Offset to VocabtList                 */
	word	object_list ;			/* Offset to Object/Room List           */
	word	globals ;				/* Offset to Global Variables           */
	word	save_bytes ;			/* No. bytes in the Save Game Area      */
	word	script_status ;			/* Z-CODE printing modes                */
	char	serial_no[6] ;			/* Game's Serial Number                 */
	word	common_word ;			/* Offset to Common Word List           */
	word	verify_length ;			/* No. words in the Game File           */
	word	verify_checksum ;		/* Game Checksum - used by Verify       */
	byte	interpreter_number ;	/* Number          - Set by Interpreter */
	byte	interpreter_version ;	/* ASCII Character - Set by Interpreter */
	byte	screen_height ;			/* Screen Height   - Set by Interpreter */
	byte	screen_width ;			/* Screen Width    - Set by Interpreter */
	byte	left ;					/* Left Coordinate - Set by Interpreter */
	byte	right ;					/* Right Coord.    - Set by Interpreter */
	byte	top ;					/* Top Coordinate  - Set by Interpreter */
	byte	bottom ;				/* Bottom Coord.   - Set by Interpreter */
	byte	unknown1 ;				/* Unknown         - Set by Interpreter */
	byte	unknown2 ;				/* Unknown         - Set by Interpreter */
	word	padding1[2] ;			/* Blank                                */
	byte	unknown3 ;				/* Unknown         - Set by Interpreter */
	byte	unknown4 ;				/* Unknown         - Set by Interpreter */
	word	unknown5 ;				/* Unknown         - Set in Data File   */
	word	padding2[3] ;			/* Blank                                */
	word	unknown6 ;				/* Unknown         - Set in Data File   */
	word	padding3[4] ;			/* Blank                                */
} ;
typedef		struct header	header ;
typedef		header HUGE		*header_ptr ;

/*
**	Header Information.
**
**	The 'z_code_version' byte has the following meaning:
**		$01 : Game compiled for an early version of the interpreter
**		$02 : Game compiled for an early version of the interpreter
**		$03 : Game compiled for the current 'Standard Series Interpreter'
**		$04 : Game compiled for the current 'Plus Series Interpreter'
**		$05 : Game compiled for the current 'Advanced Series Interpreter'
**		$06 : Game compiled for the current 'Graphics Series Interpreter'
**
**	The 'mode_bits' byte performs the following functions:
**
**		Bit 0 :		Clear	- Standard: Game Header OK.
**					Set		- Standard: Game Header Error.
**					Clear	- Advanced: No Colour.
**					Set		- Advanced: Colour.
**		Bit 1 :		Clear	- Status Bar displays the SCORE.
**					Set		- Status Bar displays the TIME.
**		Bit 2 :		Clear
**					Set
**		Bit 3 :		Clear	- Standard:	Normal.
**					Set		- Standard:	"Licensed to Tandy Corporation" Flag.
**					Clear	- Plus:		Capitalise instead of Underline.
**					Set		- Plus:		Has Underline Capability.
**		Bit 4 :		Clear	- Normal Prompt.
**					Set		- Alternative Prompt.
**		Bit 5 :		Clear	- No Special Screen Modes Available.
**					Set		- Special Screen Modes Available.
**		Bit 6 :		Clear
**					Set
**		Bit 7 :		Clear
**					Set
**
**	The 'script_status' word is used by Z-CODE to set printing modes
**	for use by the interpreter:
**
**		Bit  0 :	Clear	- Script mode off.
**					Set		- Script mode on.
**		Bit  1 :	Clear	- Use any type of Font.
**					Set		- Use a Non-Proportional Font only.
**		Bit  4 :	Clear	- Standard: No Sound.
**					Set		- Standard: Sound Available.
**		Bit 10 :	Clear	- Printer OK.
**					Set		- Printer Error (e.g.: Not Connected ).
*/

/*
**	"mode_bits" Bit Definitions
*/

#define		GAME_HEADER_OK		((byte)0xFE)
#define		GAME_HEADER_BAD		((byte)0x01)
#define		NO_COLOUR			((byte)0xFE)
#define		USE_COLOUR			((byte)0x01)
#define		USE_SCORE			((byte)0xFD)
#define		USE_TIME			((byte)0x02)
#define		NON_TANDY			((byte)0xF7)
#define		TANDY				((byte)0x08)
#define		CAPITALISE			((byte)0xF7)
#define		UNDERLINE			((byte)0x08)
#define		NORMAL_PROMPT		((byte)0xEF)
#define		ALTERNATIVE_PROMPT	((byte)0x10)
#define		NO_SCREEN_MODES		((byte)0xDF)
#define		SCREEN_MODES		((byte)0x20)

/*
**	"script_status" Bit Definitions
*/

#define		SCRIPT_MODE_OFF		((word)0xFFFE)
#define		SCRIPT_MODE_ON		((word)0x0001)
#define		USE_ANY_FONT		((word)0xFFFD)
#define		USE_NON_PROP_FONT	((word)0x0002)
#define		VERSION_3_NO_SOUND	((word)0xFFEF)
#define		VERSION_3_SOUND		((word)0x0010)
#define		SCRIPT_OK			((word)0xFBFF)
#define		SCRIPT_ERROR		((word)0x0400)

/*
**	"interpreter_number" Byte Definitions
*/

#define		XZIP				((byte)0x00)
#define		DEC_20				((byte)0x01)
#define		APPLE_2E			((byte)0x02)
#define		MACINTOSH			((byte)0x03)
#define		AMIGA				((byte)0x04)
#define		ATARI_ST			((byte)0x05)
#define		IBM_MSDOS			((byte)0x06)
#define		COMMODORE_128		((byte)0x07)
#define		C64					((byte)0x08)
#define		APPLE_2C			((byte)0x09)
#define		APPLE_2GS			((byte)0x0A)
#define		TANDY_COLOR			((byte)0x0B)

/*
**	Interpreter Arrow Key Codes
*/

#define		UP_ARROW			((int)0x81)
#define		DOWN_ARROW			((int)0x82)
#define		LEFT_ARROW			((int)0x83)
#define		RIGHT_ARROW			((int)0x84)
#define		F1_KEY				((int)0x85)
#define		F2_KEY				((int)0x86)
#define		F3_KEY				((int)0x87)
#define		F4_KEY				((int)0x88)
#define		F5_KEY				((int)0x89)
#define		F6_KEY				((int)0x8A)
#define		F7_KEY				((int)0x8B)
#define		F8_KEY				((int)0x8C)
#define		F9_KEY				((int)0x8D)
#define		F10_KEY				((int)0x8E)

/*
**	Function Prototypes
*/

/*
**	File:	enhanced.c
*/

dclproto ( extern Void				set_current_window,( word ) )
dclproto ( extern Void				split_screen,( word ) )
dclproto ( extern Void				save_cursor_position,( Void ) )
dclproto ( extern Void				restore_cursor_position,( Void ) )
dclproto ( extern Void				set_print,( Void ) )
dclproto ( extern Void				record_mode,( word ) )
dclproto ( extern Void				play_sound,( Void ) )

/*
**	File:	file.c
*/

dclproto ( extern Void				read_header,( header * ) )
dclproto ( extern Void				assign,( header *,byte_ptr ) )
dclproto ( extern word				make_word,( byte_ptr * ) )
dclproto ( extern int				open_file,( char * ) )
dclproto ( extern Void				close_file,( Void ) )
dclproto ( extern Void				load_page,( word,word,byte_ptr ) )
dclproto ( extern boolean			filename,( char * ) )
dclproto ( extern boolean			put_word,( FILE *,word ) )
dclproto ( extern boolean			ld_word,( FILE *,word * ) )
dclproto ( extern Void				save_game,( Void ) )
dclproto ( extern boolean			check,( header * ) )
dclproto ( extern Void				restore_game,( Void ) )
dclproto ( extern Void				init_script,( Void ) )
dclproto ( extern boolean			open_script,( Void ) )
dclproto ( extern Void				close_script,( Void ) )
dclproto ( extern Void				script_char,( char ) )

/*
**	File:	fns.c
*/

dclproto ( extern Void				plus,( word,word ) )
dclproto ( extern Void				minus,( word,word ) )
dclproto ( extern Void				multiply,( word,word ) )
dclproto ( extern Void				divide,( word,word ) )
dclproto ( extern Void				mod,( word,word ) )
dclproto ( extern Void				std_random,( word ) )
dclproto ( extern Void				plus_random,( word ) )
dclproto ( extern Void				less_than,( word,word ) )
dclproto ( extern Void				greater_than,( word,word ) )
dclproto ( extern Void				bit,( word,word ) )
dclproto ( extern Void				or,( word,word ) )
dclproto ( extern Void				not,( word ) )
dclproto ( extern Void				and,( word,word ) )
dclproto ( extern Void				compare,( Void ) )
dclproto ( extern Void				cp_zero,( word ) )

/*
**	File:	infocom.c
*/

dclproto ( extern int				main,( int,char *[] ) )
dclproto ( extern Void				usage,( char * ) )

/*
**	File:	init.c
*/

dclproto ( extern Void				init,( Void ) )
dclproto ( extern Void				init_opcodes,( byte ) )

/*
**	File:	input.c
*/

dclproto ( extern Void				init_vocab,( byte_ptr ) )
dclproto ( extern Void				init_input,( word ) )
dclproto ( extern Void				input,( Void ) )
dclproto ( extern word				read_line,( byte_ptr ) )
dclproto ( extern word				look_up,( word [],word ) )
dclproto ( extern Void				std_parse_buffer,( byte_ptr,byte_ptr ) )
dclproto ( extern Void				plus_parse_buffer,( byte_ptr,byte_ptr ) )
dclproto ( extern word				scan_buffer,( byte_ptr *,word ) )
dclproto ( extern Void				buffer_copy,( byte_ptr,word,byte *,word ) )
dclproto ( extern word				get_code,( byte_ptr *,word,word [] ) )
dclproto ( extern Void				advanced_parse_buffer,( byte_ptr,byte_ptr,byte_ptr,word ) )

/*
**	File:	interp.c
*/

dclproto ( extern Void				init_interpreter,( boolean ) )
dclproto ( extern Void				execute_opcode,( Void ) )
dclproto ( extern Void				operand1,( word ) )
dclproto ( extern Void				operand2,( word ) )
dclproto ( extern word				*push_params,( word,word *,int ) )
dclproto ( extern Void				operand3,( word ) )
dclproto ( extern Void				call_function,( word ) )
dclproto ( extern Void				illegal_opcode,( Void ) )
dclproto ( extern Void				operand4,( word ) )

/*
**	File:	io.c
*/

dclproto ( static char				*rc_get_token,( char ** ) )
dclproto ( static Void				rc_parse_attr,( int,char * ) )
dclproto ( static Void				default_attrs,( Void ) )
dclproto ( static Void				read_rcfile,( Void ) )
dclproto ( extern Void				print_status,( byte [] ) )
dclproto ( extern Void				display,( char * ) )
dclproto ( extern Void				raw_display,( char * ) )
dclproto ( extern Void				hex_digit,( word,int ) )
dclproto ( extern Void				hex_display,( word ) )
dclproto ( extern byte				read_char,( Void ) )
dclproto ( extern Void				out_char,( char ) )
dclproto ( extern Void				more,( Void ) )
dclproto ( extern word				allocate,( word,word ) )
dclproto ( extern Void				deallocate,( Void ) )
dclproto ( extern Void				seed_random,( Void ) )
dclproto ( extern Void				default_signal_init,( int ) )
dclproto ( extern Void				default_signal_quit,( int ) )
dclproto ( extern Void				signal_shit,( int ) )
dclproto ( extern Void				signal_chit,( int ) )
dclproto ( extern Void				null_io,( Void ) )
dclproto ( extern Void				default_putchar,( char ) )
dclproto ( extern Void				default_goto_xy,( int,int ) )
dclproto ( extern int				default_get_x,( Void ) )
dclproto ( extern int				default_get_y,( Void ) )
dclproto ( extern Void				default_use_window,( word ) )
dclproto ( extern Void				default_erase_window,( word,word ) )
dclproto ( extern Void				default_save_cursor,( Void ) )
dclproto ( extern Void				default_restore_cursor,( Void ) )
dclproto ( extern Void				default_play_sound,( word,word,word,word ) )
dclproto ( extern Void				default_set_font,( word ) )
dclproto ( extern int				default_kbd_hit,( Void ) )
dclproto ( extern Void				lsc_init_io,( Void ) )
dclproto ( extern Void				lsc_putchar,( char ) )
dclproto ( extern Void				lsc_use_window,( word ) )
dclproto ( extern Void				lsc_erase_window,( word,word ) )
dclproto ( extern Void				lsc_erase_to_eoln,( Void ) )
dclproto ( extern Void				msc_textattr,( long * ) )
dclproto ( extern Void				msc_init_io,( Void ) )
dclproto ( extern Void				msc_exit_io,( Void ) )
dclproto ( extern Void				msc_putchar,( char ) )
dclproto ( extern Void				msc_goto_xy,( int,int ) )
dclproto ( extern int				msc_get_x,( Void ) )
dclproto ( extern int				msc_get_y,( Void ) )
dclproto ( extern Void				msc_erase_window,( word,word ) )
dclproto ( extern int				msc_get_ch,( Void ) )
dclproto ( extern Void				msc_erase_to_eoln,( Void ) )
dclproto ( extern Void				tc_textattr,( long * ) )
dclproto ( extern Void				tc_init_io,( Void ) )
dclproto ( extern Void				tc_putchar,( char ) )
dclproto ( extern Void				tc_goto_xy,( int,int ) )
dclproto ( extern int				tc_get_x,( Void ) )
dclproto ( extern int				tc_get_y,( Void ) )
dclproto ( extern int				tc_getch,( Void ) )
dclproto ( extern Void				tc_erase_window,( word,word ) )
dclproto ( extern Void				tc_erase_to_eoln,( Void ) )
dclproto ( static Void				ansi_textattr,( long * ) )
dclproto ( extern Void				msdos_signal_init,( Void ) )
dclproto ( extern Void				msdos_signal_quit,( int ) )
dclproto ( extern Void				msdos_putchar,( char ) )
dclproto ( extern Void				ansi_init_io,( Void ) )
dclproto ( extern Void				ansi_exit_io,( Void ) )
dclproto ( extern Void				ansi_putchar,( char ) )
dclproto ( extern Void				ansi_goto_xy,( int,int ) )
dclproto ( extern int				ansi_get_x,( Void ) )
dclproto ( extern int				ansi_get_y,( Void ) )
dclproto ( extern Void				ansi_erase_window,( word,word ) )
dclproto ( extern Void				ansi_save_cursor,( Void ) )
dclproto ( extern Void				ansi_restore_cursor,( Void ) )
dclproto ( extern Void				ansi_erase_to_eoln,( Void ) )
dclproto ( static Void				signal_winch,( int ) )
dclproto ( extern Void				unix_init_io,( Void ) )
dclproto ( extern Void				unix_exit_io,( Void ) )
dclproto ( extern Void				unix_signal_init,( Void ) )
dclproto ( extern Void				unix_signal_quit,( int ) )
dclproto ( extern Void				tcap_init_io,( Void ) )
dclproto ( extern Void				tcap_exit_io,( Void ) )
dclproto ( extern Void				tcap_putchar,( char ) )
dclproto ( extern Void				tcap_goto_xy,( int,int ) )
dclproto ( extern int				tcap_get_x,( Void ) )
dclproto ( extern int				tcap_get_y,( Void ) )
dclproto ( extern Void				tcap_erase_window,( word,word ) )
dclproto ( extern Void				tcap_save_cursor,( Void ) )
dclproto ( extern Void				tcap_restore_cursor,( Void ) )
dclproto ( extern Void				tcap_erase_to_eoln,( Void ) )
dclproto ( extern Void				tinfo_init_io,( Void ) )
dclproto ( extern Void				tinfo_exit_io,( Void ) )
dclproto ( extern Void				tinfo_putchar,( char ) )
dclproto ( extern Void				tinfo_goto_xy,( int,int ) )
dclproto ( extern int				tinfo_get_x,( Void ) )
dclproto ( extern int				tinfo_get_y,( Void ) )
dclproto ( extern Void				tinfo_erase_window,( word,word ) )
dclproto ( extern Void				tinfo_save_cursor,( Void ) )
dclproto ( extern Void				tinfo_restore_cursor,( Void ) )
dclproto ( extern Void				tinfo_erase_to_eoln,( Void ) )
dclproto ( static Void				curses_init_attrs,( Void ) )
dclproto ( extern Void				curses_init_io,( Void ) )
dclproto ( extern Void				curses_exit_io,( Void ) )
dclproto ( extern Void				curses_putchar,( char ) )
dclproto ( extern Void				curses_goto_xy,( int,int ) )
dclproto ( extern int				curses_get_x,( Void ) )
dclproto ( extern int				curses_get_y,( Void ) )
dclproto ( extern Void				curses_erase_window,( word,word ) )
dclproto ( extern int				curses_get_ch,( Void ) )
dclproto ( extern Void				curses_erase_to_eoln,( Void ) )
dclproto ( extern int				_main,( int,char *[] ) )
dclproto ( extern Void				thinkc_init_io,( Void ) )
dclproto ( extern Void				thinkc_exit_io,( Void ) )
dclproto ( extern Void				thinkc_putchar,( char ) )
dclproto ( extern Void				thinkc_goto_xy,( int,int ) )
dclproto ( extern int				thinkc_get_x,( Void ) )
dclproto ( extern int				thinkc_get_y,( Void ) )
dclproto ( extern Void				thinkc_use_window,( word ) )
dclproto ( extern Void				thinkc_erase_window,( word,word ) )
dclproto ( extern Void				thinkc_erase_to_eoln,( Void ) )

#ifdef	AMIGAOS

dclproto ( extern VOID				ConFlush,( VOID ) )
dclproto ( extern VOID				ConCursorOn,( VOID ) )
dclproto ( extern VOID				ConCursorOff,( VOID ) )
dclproto ( extern VOID				ConCleanup,( VOID ) )
dclproto ( extern BOOL				ConSetup,( VOID ) )
dclproto ( extern UBYTE				ConGetChar,( VOID ) )
dclproto ( extern char				ConParseChar,( VOID ) )
dclproto ( extern VOID				ConWrite,( const char *,const int ) )
dclproto ( extern VOID				ConMove,( const int,const int ) )

dclproto ( extern Void				amiga_signal_init,( int ) )
dclproto ( extern Void				amiga_signal_quit,( int ) )
dclproto ( extern Void				amiga_init_io,( Void ) )
dclproto ( extern Void				amiga_exit_io,( Void ) )
dclproto ( extern Void				amiga_putchar,( char ) )
dclproto ( extern Void				amiga_goto_xy,( int,int ) )
dclproto ( extern int				amiga_get_x,( Void ) )
dclproto ( extern int				amiga_get_y,( Void ) )
dclproto ( extern Void				amiga_use_window,( int ) )
dclproto ( extern Void				amiga_erase_window,( int,int ) )
dclproto ( extern Void				amiga_save_cursor,( Void ) )
dclproto ( extern Void				amiga_restore_cursor,( Void ) )
dclproto ( extern Void				amiga_erase_to_eoln,( Void ) )
dclproto ( extern int				amiga_time,( Void ) )
dclproto ( extern int				amiga_kbd_hit,( Void ) )
dclproto ( extern int				amiga_getch,( Void ) )

#endif	/* AMIGAOS */

/*
**	File:	jump.c
*/

dclproto ( extern Void				std_gosub,( Void ) )
dclproto ( extern Void				plus_gosub,( Void ) )
dclproto ( extern Void				adv_gosub,( Void ) )
dclproto ( extern Void				std_rtn,( word ) )
dclproto ( extern Void				adv_rtn,( word ) )
dclproto ( extern Void				ret_true,( Void ) )
dclproto ( extern Void				ret_false,( Void ) )
dclproto ( extern Void				jump,( word ) )
dclproto ( extern Void				rts,( Void ) )
dclproto ( extern Void				std_pop_stack,( Void ) )
dclproto ( extern Void				adv_pop_stack,( Void ) )

/*
**	File:	message.c
*/

dclproto ( extern Void				init_message,( byte ) )
dclproto ( extern Void				print_coded,( word *,word * ) )
dclproto ( extern Void				decode,( word ) )
dclproto ( extern Void				letter_v1,( char ) )
dclproto ( extern Void				letter_v2,( char ) )
dclproto ( extern Void				letter_v3,( char ) )
dclproto ( extern Void				std_encode,( byte *,word [] ) )
dclproto ( extern Void				plus_encode,( byte *,word [] ) )
dclproto ( extern word				find_mode_v1,( char ) )
dclproto ( extern word				find_mode_v3,( char ) )
dclproto ( extern word				convert,( char ) )

/*
**	File:	object.c
*/

dclproto ( extern Void				std_transfer,( word,word ) )
dclproto ( extern Void				plus_transfer,( word,word ) )
dclproto ( extern Void				std_remove_obj,( word ) )
dclproto ( extern Void				plus_remove_obj,( word ) )
dclproto ( extern Void				std_test_attr,( word,word ) )
dclproto ( extern Void				plus_test_attr,( word,word ) )
dclproto ( extern Void				std_set_attr,( word,word ) )
dclproto ( extern Void				plus_set_attr,( word,word ) )
dclproto ( extern Void				std_clr_attr,( word,word ) )
dclproto ( extern Void				plus_clr_attr,( word,word ) )
dclproto ( extern Void				std_get_loc,( word ) )
dclproto ( extern Void				plus_get_loc,( word ) )
dclproto ( extern Void				std_get_holds,( word ) )
dclproto ( extern Void				plus_get_holds,( word ) )
dclproto ( extern Void				std_get_link,( word ) )
dclproto ( extern Void				plus_get_link,( word ) )
dclproto ( extern Void				std_check_loc,( word,word ) )
dclproto ( extern Void				plus_check_loc,( word,word ) )

/*
**	File:	options.c
*/

dclproto ( extern Void				options,( word ) )
dclproto ( extern Void				print_item,( char *,word ) )
dclproto ( extern Void				show_header,( Void ) )
dclproto ( extern Void				print_word,( word ) )
dclproto ( extern Void				show_vocab,( word ) )
dclproto ( extern boolean			prop_name,( word ) )
dclproto ( extern Void				show_std_props,( word ) )
dclproto ( extern Void				show_plus_props,( word ) )
dclproto ( extern Void				show_objects,( byte,word ) )
dclproto ( extern Void				bit_byte,( byte ) )
dclproto ( extern Void				show_tree,( byte ) )
dclproto ( extern Void				obtree,( byte,word,int ) )

/*
**	File:	page.c
*/

dclproto ( extern Void				init_page,( byte_ptr,word ) )
dclproto ( extern byte_ptr			fetch_page,( word ) )
dclproto ( extern page_table_ptr	get_LRU_page,( Void ) )
dclproto ( extern Void				fix_pc,( Void ) )

/*
**	File:	plus_fns.c
*/

dclproto ( extern Void				parameter_copy,( word *,word * ) )
dclproto ( extern Void				plus_compare2,( Void ) )
dclproto ( extern Void				adv_compare2,( Void ) )
dclproto ( extern Void				gosub2,( word ) )
dclproto ( extern Void				erase_line,( word ) )
dclproto ( extern Void				do_clear_screen,( word ) )
dclproto ( extern Void				set_cursor_posn,( Void ) )
dclproto ( extern Void				set_text_mode,( word ) )
dclproto ( extern Void				io_buffer_mode,( word ) )
dclproto ( extern Void				io_mode,( Void ) )
dclproto ( extern Void				get_key,( Void ) )
dclproto ( extern word				read_the_key,( Void ) )
dclproto ( extern boolean			wait_for_key,( word,word ) )
dclproto ( extern word				special_gosub,( word ) )
dclproto ( extern Void				gosub4,( Void ) )
dclproto ( extern Void				gosub5,( word ) )
dclproto ( extern Void				call,( word ) )
dclproto ( extern Void				branch_true,( Void ) )
dclproto ( extern Void				throw_away_stack_frame,( word,word ) )
dclproto ( extern Void				parse,( Void ) )
dclproto ( extern Void				encrypt,( word,word,word,word ) )
dclproto ( extern Void				block_copy,( Void ) )
dclproto ( extern Void				print_text,( Void ) )
dclproto ( extern Void				num_local_params,( word ) )
dclproto ( extern Void				logical_shift,( word,word ) )
dclproto ( extern Void				arithmetic_shift,( word,word ) )
dclproto ( extern Void				set_current_font,( word ) )
dclproto ( extern Void				clear_flag,( Void ) )
dclproto ( extern Void				set_flag,( Void ) )
dclproto ( extern int				array_test,( byte_ptr,word ) )
dclproto ( extern Void				test_byte_array,( word,word ) )
dclproto ( extern Void				save_context,( Void ) )
dclproto ( extern Void				restore_context,( Void ) )

/*
**	File:	print.c
*/

dclproto ( extern Void				init_print,( Void ) )
dclproto ( extern Void				flush_prt_buff,( boolean ) )
dclproto ( extern Void				print_num,( word ) )
dclproto ( extern Void				PrintNumber,( int ) )
dclproto ( extern Void				std_print2,( word ) )
dclproto ( extern Void				plus_print2,( word ) )
dclproto ( extern Void				print1,( word ) )
dclproto ( extern Void				std_p_obj,( word ) )
dclproto ( extern Void				plus_p_obj,( word ) )
dclproto ( extern Void				wrt,( Void ) )
dclproto ( extern Void				writeln,( Void ) )
dclproto ( extern Void				new_line,( Void ) )
dclproto ( extern Void				print_char,( word ) )
dclproto ( extern Void				print_buffer,( Void ) )

/*
**	File:	property.c
*/

dclproto ( extern property			std_prop_addr,( std_object_ptr ) )
dclproto ( extern property			plus_prop_addr,( plus_object_ptr ) )
dclproto ( extern property			std_next_addr,( property ) )
dclproto ( extern property			plus_next_addr,( property ) )
dclproto ( extern Void				std_getprop,( word,word ) )
dclproto ( extern Void				plus_getprop,( word,word ) )
dclproto ( extern Void				std_put_prop,( word,word,word ) )
dclproto ( extern Void				plus_put_prop,( word,word,word ) )
dclproto ( extern Void				std_get_next_prop,( word,word ) )
dclproto ( extern Void				plus_get_next_prop,( word,word ) )
dclproto ( extern Void				load_word_array,( word,word ) )
dclproto ( extern Void				load_byte_array,( word,word ) )
dclproto ( extern Void				save_word_array,( word,word,word ) )
dclproto ( extern Void				save_byte_array,( word,word,word ) )
dclproto ( extern Void				std_get_prop_addr,( word,word ) )
dclproto ( extern Void				plus_get_prop_addr,( word,word ) )
dclproto ( extern Void				std_get_p_len,( word ) )
dclproto ( extern Void				plus_get_p_len,( word ) )

/*
**	File:	status.c
*/

dclproto ( extern Void				put_status,( word ) )
dclproto ( extern char				*copy_string,( char *,char * ) )
dclproto ( extern Void				prt_status,( Void ) )

/*
**	File:	support.c
*/

dclproto ( extern Void				null,( Void ) )
dclproto ( extern Void				restart,( Void ) )
dclproto ( extern Void				quit,( Void ) )
dclproto ( extern Void				verify,( Void ) )
dclproto ( extern Void				store,( word ) )
dclproto ( extern Void				ret_value,( word ) )
dclproto ( extern byte				get_byte,( word *,word * ) )
dclproto ( extern word				get_word,( word *,word * ) )
dclproto ( extern byte				next_byte,( Void ) )
dclproto ( extern word				next_word,( Void ) )
dclproto ( extern Void				error,( word ) )

/*
**	File:	variable.c
*/

dclproto ( extern Void				get_var,( word ) )
dclproto ( extern word				load_var,( word ) )
dclproto ( extern Void				put_var,( word,word ) )
dclproto ( extern Void				push,( word ) )
dclproto ( extern Void				pop,( word ) )
dclproto ( extern Void				inc_var,( word ) )
dclproto ( extern Void				dec_var,( word ) )
dclproto ( extern Void				inc_chk,( word,word ) )
dclproto ( extern Void				dec_chk,( word,word ) )
dclproto ( extern word				load,( int ) )

#endif	/* __INFOCOM__ */

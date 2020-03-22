/*
**	File:	enhanced.c
**
**	(C)opyright 1987-1992 InfoTaskforce.
*/

#include	"infocom.h"

/*
**	Enhanced Windowing and Screen Printing Functions.
*/

Void
set_current_window ( the_window )
word	the_window ;
{
	extern header	data_head ;
	extern word		top_screen_line ;
	extern word		current_window ;
	extern boolean	disable_script ;

	if ( data_head.z_code_version >= VERSION_5 )
		flush_prt_buff ( TRUE ) ;
	if ( the_window != 0 )
	{
		/*
		**	Use the Upper Window.
		*/

		USE_WINDOW ( WINDOW_1 ) ;
		disable_script = TRUE ;
		save_cursor_position () ;

		/*
		**	Move the cursor to the top left-hand corner of the screen.
		*/

		GOTO_XY ( 0,top_screen_line ) ;
	}
	else
	{
		/*
		**	Use the Lower Window.
		*/

		USE_WINDOW ( WINDOW_0 ) ;
		disable_script = FALSE ;
		if ( current_window != the_window )
			restore_cursor_position () ;

		/*
		**	Turn off all text attributes.
		**
		**	Prior to VERSION_4, the "set_text_mode ()" function
		**	did not exist. In VERSION_3 of the interpreter the
		**	statement:
		**							print_char ( (word)1 ) ;
		**	was used instead of:
		**							set_text_mode ( (word)0 ) ;
		*/

		set_text_mode ( (word)0 ) ;
	}
	current_window = the_window ;
}

Void
split_screen ( param )
word	param ;
{
	extern header	data_head ;
	extern boolean	windowing_enabled ;
	extern word		top_screen_line ;
	extern word		window_height ;
	extern int		screen_height ;
	extern int		linecount ;
	
	if ( param == 0 )
	{
		/*
		**	Use the entire Screen.
		*/
		
		USE_WINDOW ( FULL_SCREEN ) ;
		restore_cursor_position () ;
		windowing_enabled = FALSE ;
		window_height = 0 ;
		linecount = 0 ;
	}
	else
	{
		windowing_enabled = TRUE ;
		if ( param >= (word)screen_height )
			param = (word)screen_height - 1 ;
		window_height = param ;
		if ( data_head.z_code_version <= VERSION_3 )
		{
			save_cursor_position () ;
			ERASE_WINDOW ( top_screen_line,top_screen_line + param ) ;
			restore_cursor_position () ;
		}
		USE_WINDOW ( WINDOW_0 ) ;
		if ( data_head.z_code_version >= VERSION_5 )
			GOTO_XY ( 0,screen_height - 1 ) ;
	}
}

/*
**	Enhanced Windowing Support Routines.
*/

boolean		cursor_pos_saved		= FALSE ;

Void
save_cursor_position ()
{
	extern boolean	cursor_pos_saved ;

	if ( cursor_pos_saved == FALSE )
	{
		SAVE_CURSOR () ;
		cursor_pos_saved = TRUE ;
	}
}

Void
restore_cursor_position ()
{
	extern header	data_head ;
	extern boolean	cursor_pos_saved ;
	extern int		screen_height ;

	if ( data_head.z_code_version <= VERSION_4 )
	{
		if ( cursor_pos_saved != FALSE )
		{
			RESTORE_CURSOR () ;
			cursor_pos_saved = FALSE ;
		}
	}
	else
	{
		if ( cursor_pos_saved != FALSE )
		{
			RESTORE_CURSOR () ;
			cursor_pos_saved = FALSE ;
		}
		else
			GOTO_XY ( 0,screen_height - 1 ) ;
	}
}

Void
set_print ()
{
	/*
	**	Certain STANDARD series games have debugging words "#comm", "#reco"
	**	and "#unre" which use two opcodes ($33 and $34). Their function
	**	is currently unknown.
	*/
}

/*ARGSUSED*/
Void
record_mode ( mode )
word	mode ;
{
	/*
	**	Certain STANDARD series games have debugging words "#comm", "#reco"
	**	and "#unre" which use two opcodes ($33 and $34). Their function
	**	is currently unknown.
	*/
}

Void
play_sound ()
{
	extern word		param_stack[] ;
	extern word		default_param_stack[] ;

	/*
	**	Note:
	**			In the Amiga Standard Series Executor, a sound opcode ($35)
	**			was added for the "Lurking Horror" game.
	**
	**			In the PLUS series games, only param_stack[1] is used. In this
	**			case, the only legal values for param_stack[1] are 1 and 2.
	**
	**			The following more general form is used in the ADVANCED games:
	**
	**				This function can have upto 4 parameters. They have the
	**				following meaning:
	**									parameter 1 -> sound number to play,
	**									parameter 2 -> action to perform,
	**									parameter 3 -> volume to play sound at,
	**									parameter 4 -> ?
	**
	**				If there is only one parameter, then parameter 1 indicates
	**				the number of beeps ( control-G character ) to make.
	**				If there are two or more parameters, then the action
	**				argument has the following values:
	**
	**									action = 2 -> play sound file,
	**									action = 3 -> stop playing sound file,
	**									action = 4 -> free sound resources.
	**
	**				If there are three or more parameters, the volume parameter
	**				is a value between 1 and 8, representing the volume to play
	**				the sound at.
	*/

	default_param_stack[0] = 4 ;
	default_param_stack[2] = 2 ;
	default_param_stack[3] = 0xFF ;
	default_param_stack[4] = 0 ;
	parameter_copy ( default_param_stack,param_stack ) ;
	PLAY_SOUND ( param_stack[1],param_stack[2],param_stack[3],param_stack[4] ) ;
}

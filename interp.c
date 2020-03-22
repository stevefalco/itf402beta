/*
**	File:	interp.c
**
**	(C)opyright 1987-1992 InfoTaskforce.
*/

#include	"infocom.h"

extern proc_ptr	jmp_op0[] ;
extern proc_ptr	jmp_op1[] ;
extern proc_ptr	jmp_op2[] ;
extern int		operands[] ;

Void
init_interpreter ( init_pc )
boolean		init_pc ;
{
	extern word			pc_page ;
	extern word			pc_offset ;
	extern word			*stack_base ;
	extern word			*stack_var_ptr ;
	extern word			*stack ;
	extern byte_ptr		base_ptr ;
	extern header		data_head ;
	extern int			screen_width ;
	extern int			screen_height ;
	extern boolean		enhanced ;
	extern boolean		alternate_prompt ;

	register byte_ptr	ptr ;

	if ( init_pc == TRUE )
	{
		stack_var_ptr = stack_base ;
		stack = --stack_var_ptr ;
		pc_page = data_head.start / BLOCK_SIZE ;
		pc_offset = data_head.start % BLOCK_SIZE ;
	}

	fix_pc () ;
	init_print () ;

	/*
	**	Set the "script_status" word in the data file to the
	**	value currently in use by the interpreter.
	*/

	ptr = (byte_ptr) &(((header_ptr)base_ptr) -> script_status) ;
	*ptr++ = MOST_SIGNIFICANT_BYTE ( data_head.script_status ) ;
	*ptr = LEAST_SIGNIFICANT_BYTE ( data_head.script_status ) ;

	if ( alternate_prompt )
	{
		((header_ptr)base_ptr) -> mode_bits |= ALTERNATIVE_PROMPT ;
		data_head.mode_bits |= ALTERNATIVE_PROMPT ;
	}

	if ( data_head.z_code_version <= VERSION_3 )
	{
		if ( enhanced )
		{
			((header_ptr)base_ptr) -> mode_bits |= SCREEN_MODES ;
			data_head.mode_bits |= SCREEN_MODES ;
			USE_WINDOW ( FULL_SCREEN ) ;
		}
	}
	else
	{
		/*
		**	Screen Coordinates:
		**
		**		Horizontal:	From 0 to (screen_width - 1)
		**		Vertical:	From 0 to (screen_height - 1)
		*/
			
		((header_ptr)base_ptr) -> interpreter_number = INTERPRETER ;
		((header_ptr)base_ptr) -> interpreter_version = (byte)'A' ;
		((header_ptr)base_ptr) -> screen_height = (byte)(screen_height - 1) ;
		((header_ptr)base_ptr) -> screen_width = (byte)(screen_width - 1) ;
		((header_ptr)base_ptr) -> mode_bits |= MODE_BITS ;
		data_head.mode_bits |= MODE_BITS ;
		USE_WINDOW ( FULL_SCREEN ) ;

		if ( data_head.z_code_version >= VERSION_5 )
		{

			((header_ptr)base_ptr) -> left = 0 ;
			((header_ptr)base_ptr) -> right = (byte)(screen_width - 1) ;
			((header_ptr)base_ptr) -> top = 0 ;
			((header_ptr)base_ptr) -> bottom = (byte)(screen_height - 1) ;
			((header_ptr)base_ptr) -> unknown1 = 1 ;
			((header_ptr)base_ptr) -> unknown2 = 1 ;
			((header_ptr)base_ptr) -> unknown3 = 9 ;
			((header_ptr)base_ptr) -> unknown4 = 2 ;
		}
	}
}

Void
execute_opcode ()
{
	extern boolean	debug ;
	extern FILE		*debug_file ;
	extern char		name[] ;
	extern boolean	stop ;
	extern word		pc_page ;
	extern word		pc_offset ;

	register word	opcode ;
	register long	debug_pc ;

	if ( debug )
	{
		if ( debug_file == (FILE *)0 )
		{
			while ( ! filename ( "Debug Filename: " ) ) ;
			if (( debug_file = fopen ( name,APPEND_STRING )) == (FILE *)0 )
			{
				display ( "Cannot Open Debug File\n" ) ;
				stop = TRUE ;
			}
		}
	}

	stop = FALSE ;
	while ( stop == FALSE )
	{
		if ( debug )
			debug_pc = ( (long)pc_page * BLOCK_SIZE ) + (long)pc_offset ;
		opcode = next_byte () ;
		if ( debug )
			fprintf ( debug_file,"PC: $%06lX - opcode: $%02X",debug_pc,opcode );
		if ( opcode < 0x80 )
			operand2 ( opcode ) ;
		else
		{
			if ( opcode < 0xB0 )
				operand1 ( opcode ) ;
			else
			{
				if ( opcode >= 0xC0 )
					operand3 ( opcode ) ;
				else
				{
					/*
					**	Opcode 0xBE is an ADVANCED series opcode. It is
					**	used to access opcodes 0x40 to 0x4A in the
					**	"jmp_op2[]" table. These call "illegal_opcode ()"
					**	for game files earlier than ADVANCED.
					*/

					if ( opcode == 0xBE )
					{
						opcode = ( next_byte () & 0x3F ) | 0x40 ;
						if ( debug )
							fprintf ( debug_file,":<$%02X>",opcode ) ;
						operand4 ( opcode ) ;
					}
					else
						(*jmp_op0[opcode & 0x0F])() ;
				}
			}
		}
		if ( debug )
			fprintf ( debug_file,"\n" ) ;
	}
	if ( debug )
	{
		if ( debug_file != (FILE *)0 )
		{
			if ( fclose ( debug_file ) )
				display ( "Cannot Close Debug File\n" ) ;
			debug_file = (FILE *)0 ;
		}
	}
}

Void
operand1 ( opcode )
register word	opcode ;
{
	extern boolean	debug ;
	extern FILE		*debug_file ;

	register word	param1 ;

	param1 = load ((int)(( opcode >> 4 ) & 0x03 )) ;
	if ( debug )
		fprintf ( debug_file,"\t1 Parameter : $%04X",param1 ) ;
	(*jmp_op1[opcode & 0x0F])( param1 ) ;
}

Void
operand2 ( opcode )
register word	opcode ;
{
	extern boolean	debug ;
	extern FILE		*debug_file ;
	extern word		param_stack[] ;

	register word	*param_ptr ;
	register word	param1 ;
	register word	param2 ;
	register int	mode ;

	mode = 1 ;
	if ( opcode & 0x40 )
		++mode ;
	param1 = load ( mode ) ;
	mode = 1 ;
	if ( opcode & 0x20 )
		++mode ;
	param2 = load ( mode ) ;
	if ( debug )
		fprintf ( debug_file,"\t2 Parameters: $%04X $%04X",param1,param2 ) ;
	opcode &= 0x1F ;
	if ( operands[opcode] )
		(*jmp_op2[opcode])( param1,param2 ) ;
	else
	{
		param_ptr = &param_stack[0] ;
		*param_ptr++ = 2 ;
		*param_ptr++ = param1 ;
		*param_ptr++ = param2 ;
		(*jmp_op2[opcode])() ;
	}
}

word
*push_params ( modes,param_ptr,count )
register word	modes ;
register word	*param_ptr ;
register int	count ;
{
	extern word		param_stack[] ;

	if ( count )
	{
		param_ptr = push_params ( ( modes >> 2 ),param_ptr,--count ) ;
		if ( param_ptr != (word *)0 )
		{
			modes &= 0x03 ;
			if ( modes != 0x03 )
			{
				*param_ptr++ = load ( (int)modes ) ;
				++param_stack[0] ;
				return ( param_ptr ) ;
			}
		}
		return ( (word *)0 ) ;
	}
	return ( param_ptr ) ;
}

Void
operand3 ( opcode )
register word	opcode ;
{
	extern word		param_stack[] ;

	register word	*param_ptr ;

	/*
	**	Reset the Parameter Count.
	*/

	param_stack[0] = 0 ;

	/*
	**	Fetch the Parameters.
	**
	**	Opcode 0xFA is an ADVANCED series opcode. It is used to
	**	access opcode 0x3A in the "jmp_op2[]" table. This calls
	**	"illegal_opcode ()" for game files earlier than ADVANCED.
	*/

	if (( opcode == 0xEC ) || ( opcode == 0xFA ))
		param_ptr = push_params ( (word) next_word (),&param_stack[1],8 ) ;
	else
		param_ptr = push_params ( (word) next_byte (),&param_stack[1],4 ) ;

	opcode &= 0x3F ;
	call_function ( opcode ) ;
}

Void
call_function ( opcode )
register word	opcode ;
{
	extern boolean	debug ;
	extern FILE		*debug_file ;
	extern word		param_stack[] ;

	register proc_ptr	func ;

	if ( debug )
		fprintf ( debug_file,"\t%d Parameters: $%04X $%04X $%04X",param_stack[0],param_stack[1],param_stack[2],param_stack[3] ) ;
	func = jmp_op2[opcode] ;
	switch ( operands[opcode] )
	{
		case 0 :
				(*func)() ;
				break ;
		case 1 :
				(*func)( param_stack[1] ) ;
				break ;
		case 2 :
				(*func)( param_stack[1],param_stack[2] ) ;
				break ;
		case 3 :
				(*func)( param_stack[1],param_stack[2],param_stack[3] ) ;
				break ;
	}
}

Void
illegal_opcode ()
{
	extern boolean	debug ;
	extern FILE		*debug_file ;

	if ( debug )
		fprintf ( debug_file,"\n" ) ;
	error ( ERR_OPCODE ) ;
}

Void
operand4 ( opcode )
register word	opcode ;
{
	extern word		param_stack[] ;

	register word	*param_ptr ;

	/*
	**	Reset the Parameter Count.
	*/

	param_stack[0] = 0 ;

	/*
	**	Fetch the Parameters.
	*/

	param_ptr = push_params ( (word) next_byte (),&param_stack[1],4 ) ;
	call_function ( opcode ) ;
}

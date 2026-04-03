////////////////////////////////
//~ tt: Build Options

#define BUILD_CONSOLE_INTERFACE 1

////////////////////////////////
//~ tt: includes

//~ tt: headers
#include "base/base_inc.h"
#include "os/os_inc.h"
#include "lexer/lexer.h"

//~ tt: impls
#include "base/base_inc.c"
#include "os/os_inc.c"
#include "lexer/lexer.c"

internal void
entry_point(CmdLine *cmdline)
{
	U8 *buffer = "u8 kappa = 12; \n u32 kippo = 12";
	Lexer *lexer = lexer_alloc();
	lexer->curr = buffer;

	lexer_analyze(lexer);

	for (Token *curr = lexer->token_head; curr != 0; curr = curr->next)
		lexer_print_token(curr);

	lexer_release(lexer);
	return;
}

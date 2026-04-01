internal Lexer *
lexer_alloc(void)
{
  Arena *arena = arena_alloc();
  Lexer *lexer = push_array(arena, Lexer, 1);
  lexer->arena = arena;
  lexer->token_head = 0;
  lexer->token_end = 0;
  lexer->curr = 0;
  lexer->line_start = 0;
  lexer->line_number = 0;
  lexer->col_number = 0;
  return lexer;
}

internal void
lexer_token_push(Lexer *lexer, TokenKind kind, String8 value, Location location, String8 error_messege)
{
  Token *token = push_array(lexer->arena, Token, 1);
  token->next = 0;
  token->kind = kind;
  token->value = value;
  token->location = location;
  token->error_messege = error_messege;
  SLLQueuePush(lexer->token_head, lexer->token_end, token);
}

internal void
lexer_release(Lexer *lexer)
{
  arena_release(lexer->arena);
}

internal void
lexer_print_token(Token *token)
{
  String8 token_to_literal[] =
  {
    str8_lit_comp("TokenKind_num"),
    str8_lit_comp("TokenKind_ident"),
  };

  fprintf(stdout, "\n==== NEW TOKEN ====");
  fprintf(stdout, "\nTokne->kind = %.*s", str8_varg(token_to_literal[token->kind]));
  fprintf(stdout, "\nTokne->value = %.*s", str8_varg(token->value));
  fprintf(stdout, "\nTokne->location = { line: %llu, col: %llu }", token->location.line, token->location.col);
  fprintf(stdout, "\n");
}

internal void
lexer_analyze(Lexer *lexer)
{
  while (*lexer->curr != '\0')
  {
    if (char_is_alpha(*lexer->curr))
    { 
      U64 ident_size = 1;
      U8  peak_next = *(lexer->curr + 1);
      B32 is_alpha = char_is_alpha(peak_next) && !char_is_space(peak_next);
      for (U64 i = ident_size + 1; is_alpha == 1; ++i)
      {
        ++ident_size;
        peak_next = *(lexer->curr + i);
        is_alpha = char_is_alpha(peak_next) && !char_is_space(peak_next);
      }
      lexer_token_push(lexer, TokenKind_identifier, str8(lexer->curr, ident_size), (Location){lexer->line_number, lexer->line_number}, str8("", 0));
      lexer->curr += ident_size;
    }
    lexer->curr++;
  }
}

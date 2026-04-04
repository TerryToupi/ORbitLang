
read_only internal String8 token_to_literal[] =
{
  str8_lit_comp("TokenKind_TS_u8"),
  str8_lit_comp("TokenKind_TS_u16"),
  str8_lit_comp("TokenKind_TS_u32"),
  str8_lit_comp("TokenKind_TS_u64"),
  str8_lit_comp("TokenKind_TS_s8"),
  str8_lit_comp("TokenKind_TS_s16"),
  str8_lit_comp("TokenKind_TS_s32"),
  str8_lit_comp("TokenKind_TS_s64"),
  str8_lit_comp("TokenKind_TS_b8"),
  str8_lit_comp("TokenKind_TS_b16"),
  str8_lit_comp("TokenKind_TS_b32"),
  str8_lit_comp("TokenKind_TS_b64"),
  str8_lit_comp("TokenKind_TS_f32"),
  str8_lit_comp("TokenKind_TS_f64"),
  str8_lit_comp("TokenKind_TS_TS_COUNT"),
  str8_lit_comp("TokenKind_num"),
  str8_lit_comp("TokenKind_identifier"),
  str8_lit_comp("TokenKind_string"),
  str8_lit_comp("TokenKind_COUNT"),
};

read_only internal String8 token_to_type_literal[] =
{
  str8_lit_comp("u8"),
  str8_lit_comp("u16"),
  str8_lit_comp("u32"),
  str8_lit_comp("u64"),
  str8_lit_comp("s8"),
  str8_lit_comp("s16"),
  str8_lit_comp("s32"),
  str8_lit_comp("s64"),
  str8_lit_comp("b8"),
  str8_lit_comp("b16"),
  str8_lit_comp("b32"),
  str8_lit_comp("b64"),
  str8_lit_comp("f32"),
  str8_lit_comp("f64"),
};

read_only internal U8 token_to_type_literal_size[] =
{
  2,
  3,
  3,
  3,
  2,
  3,
  3,
  3,
  2,
  3,
  3,
  3,
  3,
  3,
};

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

internal U8
lexer_peak(Lexer *lexer, U8 advance)
{
  if (*(lexer->curr + advance))
    return *(lexer->curr + advance);
  else
    return 0;
}

internal B32
lexer_is_curr_type_keyword(Lexer *lexer)
{
  U8 peak = lexer_peak(lexer, 0);
  if (!(peak == 'u' || peak == 's' || peak == 'b' || peak == 'f'))
    return 0;

  if (lexer_peak(lexer, 1) == '8')
    return 1; 

  String8 value = { lexer->curr + 1, 2 };
  return str8_match(value, str8("16", 2), StringMatchFlag_CaseInsensitive) ||
          str8_match(value, str8("32", 2), StringMatchFlag_CaseInsensitive) ||
          str8_match(value, str8("64", 2), StringMatchFlag_CaseInsensitive);
}

internal B32
lexer_is_curr_ident(Lexer *lexer)
{
  U8 peak = lexer_peak(lexer, 0);
  B32 valid_char = !char_is_space(peak) && ( char_is_alpha(peak) || peak == '_' ) ? 1 : 0;
  return valid_char;
}

internal void
lexer_push_type_keyword(Lexer *lexer)
{
  for (TokenKind kind = TokenKind_TS_u8; kind < TokenKind_COUNT; ++kind)
  {
    if (str8_match(str8(lexer->curr, token_to_type_literal_size[kind]), token_to_type_literal[kind], StringMatchFlag_CaseInsensitive))
    {
      lexer_token_push(lexer, kind, str8(lexer->curr, token_to_type_literal_size[kind]), (Location){0, 0}, str8("", 0));
      lexer->curr += token_to_type_literal_size[kind];
      return;
    }
  }
}

internal void
lexer_push_ident(Lexer *lexer)
{
  String8 value = { lexer->curr, 0 };
  while (lexer_is_curr_ident(lexer))
  {
    value.size += 1;
    lexer->curr += 1;
  }

  lexer_token_push(lexer, TokenKind_identifier, value, (Location){0, 0}, str8("", 0));
}

internal void
lexer_print_token(Token *token)
{
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
    if (lexer_is_curr_ident(lexer))
    {
      if (lexer_is_curr_type_keyword(lexer))
      {
        lexer_push_type_keyword(lexer);
      }
      else
      {
        lexer_push_ident(lexer);
      }
    }
    else
    {
      lexer->curr += 1;
    }
  }
}

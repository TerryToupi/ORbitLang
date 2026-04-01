
typedef U64 TokenKind;
enum
{
  TokenKind_num         = 0,
  TokenKind_identifier  = 1,
  TokenKind_string      = 2,
};

typedef struct Location Location;
struct Location
{
  U64 line;
  U64 col;
};

typedef struct Token Token;
struct Token
{
  Token     *next;
  TokenKind  kind;
  String8    value;
  Location   location;
  String8    error_messege;
};

typedef struct Lexer Lexer;
struct Lexer
{
  Arena *arena;

  Token *token_head;
  Token *token_end;

  U8    *curr;
  U8    *line_start;
  U64    line_number;
  U64    col_number;
};

internal Lexer * lexer_alloc(void);
internal void    lexer_analyze(Lexer *lexer);
internal void    lexer_release(Lexer *lexer);

internal void    lexer_print_token(Token *token);
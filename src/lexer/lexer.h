
typedef U64 TokenKind;
enum
{
  TokenKind_TS_u8       = 0,
  TokenKind_TS_u16      = 1,
  TokenKind_TS_u32      = 2,
  TokenKind_TS_u64      = 3,
  TokenKind_TS_s8       = 4,
  TokenKind_TS_s16      = 5,
  TokenKind_TS_s32      = 6,
  TokenKind_TS_s64      = 7,
  TokenKind_TS_b8       = 8,
  TokenKind_TS_b16      = 9,
  TokenKind_TS_b32      = 10,
  TokenKind_TS_b64      = 11,
  TokenKind_TS_f32      = 12,
  TokenKind_TS_f64      = 13,
  TokenType_TS_COUNT    = 14,
  TokenKind_P_COUNT     = 15,
  TokenKind_num         = 15,
  TokenKind_identifier  = 16,
  TokenKind_string      = 17,
  TokenKind_COUNT       = 18,
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
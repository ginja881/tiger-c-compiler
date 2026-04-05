#ifndef __TOKENS_H__
#define __TOKENS_H__


#include "utils.h"

// Main Enum for Tokens
typedef enum {
      // Operations
      MUL,
      ADD,
      MOD,
      SUB,
      GT,
      GT_EQ,
      LT, 
      LT_EQ,
      NOT
      COMPAR_EQ,
      COMPAR_AND,
      COMPAR_OR,
      BIT_AND,
      BIT_RSHIFT,
      BIT_LSHIFT,
      BIT_OR,
      INCREMENT,
      DECREMENT,
      // Control Flow statements
      FOR,
      TO,
      IN,
      WHILE,
      DO,
      IF,
      ELSE,
      THEN,
      FUNCTION_DEF,
      END,
      LET,
      // Expressions
      NULL_VAL,
      ID,
      REAL,
      NUM,
      // Useful punctuation used for parsing/reporting errors that the LEXER should report
      SEMI_COLON,
      COLON,
      L_PAREN,
      R_PAREN,
      NEW_LINE,
      // BRCKT = BRACKET
      L_CURLY_BRCKT,
      R_CURLY_BRCKT,
      L_SQUARE_BRCKT,
      R_SQUARE_BRCKT,
      // Vars and Types
      VAR_DEC,
      TYPE_DEC,
      VAR_ASSIGN,
      TYPE_ASSIGN,
      CHAR,
      STRING,
      INTERPOLATED_STRING,
      END_OF_FILE
} token;

// Raw Token Representation
struct Token_ {
     size_t line_pos;
     size_t char_pos,
     size_t text_size;
     token token_type;

     struct Token_* next;
};
typedef struct Token_* Token;

// TokenQueue as classic data structure for parsing
struct TokenQueue_ {
      size_t size;
      Token front;    
      Token tail;
};

typedef struct TokenQueue_* TokenQueue;

// entire lexer data structure to make parsing way easier
struct Lexer_ {
     TokenQueue queue;
     size_t current_pos;
     size_t current_line;
     
};

typedef struct Lexer_* Lexer;


// Constructors
Token make_token(size_t line_pos, size_t char_pos, size_t text_size, token token_type);
Lexer make_lexer();

// Parser operations
Token peek(TokenQueue queue);
token match_keyword(string text);
int match(TokenQueue queue, token token_type);


// Dynamic operations
Token dequeue_token(TokenQueue queue);
TokenQueue enqueue_token(TokenQueue queue, Token token);



#endif


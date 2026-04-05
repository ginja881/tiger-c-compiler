#include "tokens.h"

Token make_token(size_t line_pos, size_t char_pos, size_t text_size, string input, token token_type) {
      Token new_token = (Token)checked_malloc(sizeof(struct Token_));
      new_token->line_pos = line_pos;
      new_token->char_pos = char_pos;
      new_token->text_size = text_size;
      new_token->input = input;
      new_token->token_type = token_type;
      
      return new_token;
}

Lexer make_lexer() {
       Lexer lexer = (Lexer)checked_malloc(sizeof(struct Lexer_));
       lexer->queue = (TokenQueue)checked_malloc(sizeof(struct TokenQueue_));
       lexer->queue->front = NULL;
       lexer->queue->tail = NULL;

       lexer->queue->size = 0;

       lexer->current_pos = 0;
       lexer->current_line = 1;

       return lexer;
}

Token peek(TokenQueue queue) {
     return queue->front;
}

int match(TokenQueue queue, token token_type) {
    return queue->front->token_type == token_type ? TRUE : FALSE;
}

TokenQueue enqueue_token(TokenQueue queue, Token token) {
      if (queue->size == 0) {
           queue->front = token;
	   queue->tail = queue->front;
      }
      else {
          queue->tail->next = token;
	  queue->tail = queue->tail->next;
      }
      queue->size++;
      return queue;
     
}

Token dequeue_token(TokenQueue queue) {
      if (queue->size == 0) {
           return NULL;
      }

      Token old_front = queue->front;
      queue->front = queue->front->next;
      queue->size--;
      return old_front;
}

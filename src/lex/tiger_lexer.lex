%{

#include "tokens.h"
extern Lexer lexer;

void advance() {
    lexer->current_pos += yyleng;
    lexer->current_input_size =yyleng;
    lexer->current_input = yytext;   
}
void lexer_error() {
     report_error(
     	IllegalCharacter, 
     	lexer->current_input, 
     	lexer->current_line, 
     	lexer->current_pos
     );
}
// Helpers for strings

char* text;
%}

digits [0-9]+
identifiers [a-zA-Z_]([a-zA-Z_0-9])*
%Start MULTI_COMMENT STRING SINGLE_COMMENT

%%
<INITIAL>function		{ advance(); return FUNCTION_DEF; }
<INITIAL>while			{ advance(); return WHILE; }
<INITIAL>if			{ advance(); return IF; }
<INITIAL>end			{ advance(); return END; }
<INITIAL>to			{ advance(); return TO; }
<INITIAL>in			{ advance(); return IN; }
<INITIAL>else			{ advance(); return ELSE; }
<INITIAL>then			{ advance(); return THEN; }
<INITIAL>for			{ advance(); return FOR; }
<INITIAL>do			{ advance(); return DO; }
<INITIAL>var 			{ advance(); return VAR_DEC; }
<INITIAL>type			{ advance(); return TYPE_DEC; }
<INITIAL><<EOF>>		{ return END_OF_FILE;}
<INITIAL>:=			{ advance(); return VAR_ASSIGN; }
<INITIAL>!=			{ advance(); return COMPAR_NOT_EQ;}
<INITIAL>==			{ advance(); return COMPAR_EQ; }
<INITIAL>>>			{ advance(); return BIT_LSHIFT; }
<INITIAL><<			{ advance(); return BIT_RSHIFT; } 
<INITIAL>>=			{ advance(); return GT_EQ; }
<INITIAL><=			{ advance(); return LT_EQ; }
<INITIAL>>			{ advance(); return GT; }
<INITIAL><			{ advance(); return LT; }
<INITIAL>= 			{ advance(); return TYPE_ASSIGN;}
<INITIAL>,			{ advance(); return COMMA;}
<INITIAL>&&			{ advance(); return COMPAR_AND;}
<INITIAL>"||"			{ advance(); return COMPAR_OR;}
<INITIAL>"|"			{ advance(); return BIT_OR;}
<INITIAL>&			{ advance(); return BIT_AND; }
<INITIAL>!			{ advance(); return NOT; }
<INITIAL>\+			{ advance(); return ADD; }
<INITIAL>-			{ advance(); return SUB; }
<INITIAL>"/*"			{ advance(); BEGIN MULTI_COMMENT;}
<MULTI_COMMENT>"*/"		{ advance(); BEGIN INITIAL; }
<MULTI_COMMENT>.		{ advance(); }
<INITIAL>"//"			{ advance(); BEGIN SINGLE_COMMENT; }
<SINGLE_COMMENT>"\n"		{ advance(); BEGIN INITIAL;}
<SINGLE_COMMENT>.		{ advance(); }
<INITIAL>"*"			{ advance(); return MUL; }
<INITIAL>"/"			{ advance(); return DIV; }
<INITIAL>%			{ advance(); return MOD; }
<INITIAL>"++"			{ advance(); return INCREMENT; }
<INITIAL>--			{ advance(); return DECREMENT; } 
<INITIAL>;			{ advance(); return SEMI_COLON; }
<INITIAL>:			{ advance(); return COLON; }
<INITIAL>"("			{ advance(); return L_PAREN; }
<INITIAL>")"			{ advance(); return R_PAREN; } 
<INITIAL>"{"			{ advance(); return L_CURLY_BRCKT; }
<INITIAL>"}"			{ advance(); return R_CURLY_BRCKT; }
<INITIAL>"["			{ advance(); return L_SQUARE_BRCKT; }
<INITIAL>"]"			{ advance(); return R_SQUARE_BRCKT; }
<INITIAL>(nil|NIL)		{ advance(); return NULL_VAL; }
<INITIAL>"\t"			{ advance(); return TAB; }
<INITIAL>"\n"			{ advance(); return NEW_LINE; }
<INITIAL>true|TRUE		{ advance(); return TRUE;}
<INITIAL>false|FALSE		{ advance(); return FALSE;}
<INITIAL>{identifiers}		{ advance(); return ID; }
<INITIAL>{digits}"."{digits}	{ advance(); return REAL; }
<INITIAL>{digits}		{ advance(); return NUM; } 
<INITIAL>\"			{       
                                        lexer->current_input_size = 0;
					lexer->current_pos++;
					BEGIN STRING; 
				}
<STRING>"\n"			{ lexer_error();}
<STRING><<EOF>>			{ lexer_error();}
<STRING>[^\"]+			{
					text = strdup(yytext);
                                        lexer->current_input_size += yyleng;
				}
<STRING>\"		        {
					lexer->current_input = text;
					BEGIN INITIAL; 
					return STRING;
				}
<INITIAL>\'[^\\n]{1}\' 		{ advance(); return CHAR;}
<INITIAL>(" ")+			{ advance(); }
<INITIAL>"."			{ advance(); return MEMBER_REF;}
<INITIAL>.		        { lexer_error();}
 

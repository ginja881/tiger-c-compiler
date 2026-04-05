%{ 

#include "tokens.h"
extern Lexer lexer;
void advance() {
    lexer->current_input = NULL;
    lexer->current_pos += yyleng;
    lexer->current_input = String(yytext);
}
%}

digits [0-9]+


%%

if 			{ advance(); return IF; }
for 			{ advance(); return FOR; }
while 			{ advance(); return WHILE; }
else			{ advance(); return ELSE; }
then 			{ advance(); return THEN; }
end			{ advance(); return END; }
to			{ advance(); return TO; }
in			{ advance(); return IN; }
function		{ advance(); return FUNCTION_DEF; }
do			{ advance(); return DO; }
let			{ advamce(); return LET; }
var			{ advance(); return VAR_DEC; }
type 			{ advance(); return TYPE_DEC; }
=			{ advance(); return TYPE_ASSIGN; }
:=			{ advance(); return VAR_ASSIGN; }
>=			{ advance(); return GT_EQ; }
<=			{ advance(); return LT_EQ; }
<			{ advance(); return LT; }
>			{ advance(); return GT; }
==			{ advance(); return COMPAR_EQ; }
&			{ advance(); return BIT_AND; }
&&			{ advance(); return COMPAR_AND; }
||			{ advance(); return COMPAR_OR; }
!			{ advance(); return NOT; }
>>			{ advance(); return BIT_LSHIFT; }
<<			{ advance(); return BIT_RSHIFT; }
+			{ advance(); return ADD; }
-			{ advance(); return SUB; }
*			{ advance(); return MUL; }
/			{ advance(); return DIV; }
%			{ advance(); return MOD; }
++			{ advance(); return INCREMENT; }
--			{ advance(); return DECREMENT; } 
;			{ advance(); return SEMI_COLON; }
:			{ advance(); return COLON; }
(			{ advance(); return L_PAREN; }
)			{ advance(); return R_PAREN; } 
{			{ advance(); return L_CURLY_BRCKT; }
}			{ advance(); return R_CURLY_BRCKT; }
"["			{ advance(); return L_SQUARE_BRCKT; }
"]"			{ advance(); return R_SQUARE_BRCKT; }
null			{ advance(); return NULL_VAL; }
"\n"			{ return NEW_LINE; }
{digits}		{ advance(); return NUM; }
{digits}"."{digits}	{ advance(); return REAL; }

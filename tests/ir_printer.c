#include "parser/ast.h"
#include "lex/tokens.h"
#include "semant/semant.h"
#include "util.h"

Lexer lexer;
int panic_mode;
extern FILE* yyin;

int invalid_wrapped_irs = 0;
int invalid_expression_irs = 0;
int invalid_statement_irs = 0;
int invalid_condition_irs = 0;

void IRPrinter_printIR(TreeIR ir_root);
void IRPrinter_printExExpression(T_Exp ex_exp);
void IRPrinter_printNxExpression(T_Stm nx_exp);
void IRPrinter_printCxExpression(T_Cx cx_exp);
void IRPrinter_printFragments(void);
void IRPrinter_printFrame(Frame frame);

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "\n Must specify a .tig file \n");
		return EXIT_FAILURE;
	}

	char* filename = argv[1];
	string extension = strrchr(filename, '.');
	if (!extension || strcmp(extension +1, "tig") != 0) {
		fprintf(stderr, "\n Must specify a .tig file\n");
		return EXIT_FAILURE;
	}


	lexer = make_lexer();
	int token;
	int panic_mode = FALSE;

	yyin = fopen(filename, "r");

	while ((token = yylex()) != -1) {
		Token new_token = make_token(
			lexer->current_line,
			lexer->current_pos,
			lexer->current_input_size,
			String(lexer->current_input),
			token
		);

		enqueue_token(lexer->queue, new_token);
		if (new_token->token_type == NEW_LINE) {
			lexer->current_line++;
			lexer->current_pos = 0;
		}
		else if (new_token->token_type == END_OF_FILE)
			break;
	}

	Parser parser = make_parser();
	parser->root = parse_program(lexer, parser);

	SemanticAnalyzer sem = make_semantic_analyzer(parser);
        TreeIR root = semantic_main(sem);
	
	IRPrinter_printIR(root);
	IRPrinter_printFragments();

	printf("\n INVALID WRAPPED IRs (Tr_Exp): %d \n", invalid_wrapped_irs);
	printf("\n INVALID EXPRESSION IRs (T_Exp): %d \n", invalid_expression_irs);
	printf("\n INVALID STATEMENT IRs (T_Stm): %d \n", invalid_statement_irs);
	printf("\n INVALID CONDITION IRs (T_Cx): %d \n", invalid_condition_irs);

	return EXIT_SUCCESS;
}


void IRPrinter_printIR(TreeIR ir_root) {
	if (ir_root == NULL) {
		printf("\n ROOT NULL\n");
		return;
	}
	
	switch (ir_root->kind) {
		case TreeExp: {
			Tr_Exp expression = ir_root->u.expression.exp_ir;
			
			if (expression->kind == EX)
				IRPrinter_printExExpression(expression->u.ex);
			else if (expression->kind == NX)
				IRPrinter_printNxExpression(expression->u.nx);
			else if (expression->kind == CX)
				IRPrinter_printCxExpression(expression->u.cx);
			else {
				printf("\nIR Expression at %p is invalid, check if referenced memory is a valid Tr_Exp\n", expression);
				invalid_wrapped_irs++;
			}

			return;
		}
		case TreeCompound: {
			printf("\n------ COMPOUND STATEMENT 1 ------\n");
			IRPrinter_printIR(ir_root->u.compound.exp1);
			printf("\n------ COMPOUND STATEMENT 2 ------\n");
			IRPrinter_printIR(ir_root->u.compound.exp2);
		}
	}
}


void IRPrinter_printExExpression(T_Exp ex_exp) {
	if (ex_exp == NULL) {
		return;
	}

	switch (ex_exp->kind) {
		case TR_CONST: {
			printf("\nCONSTANT(%d)\n", ex_exp->u.constant);
			return;
		}
		case TR_BINOP: {
			printf("\nBINOP( ");
			IRPrinter_printExExpression(ex_exp->u.binop.op1);
			printf(",\n");
			IRPrinter_printExExpression(ex_exp->u.binop.op2);
			printf(")\n");
			return;
		}
		case TR_MEM: {
			printf("\nMEM( ");
			IRPrinter_printExExpression(ex_exp->u.mem.memory);
			printf(",\n");
			printf("%d\n", ex_exp->u.mem.size);
			printf(")\n");
			return;
		}
		case TR_TEMP: {
			printf("\nTEMP(%d)\n", ex_exp->u.temp->num);
			return;
		}
		case TR_NAME: {
			printf("\nNAME(%p)\n", ex_exp->u.name);
			return;
		}
		case TR_CHAR: {
			printf("\nCHAR(%c)\n", (char)ex_exp->u.character);
			return;
		}
		case TR_CALL: {
			printf("\nCALL(");
			IRPrinter_printExExpression(ex_exp->u.call_exp.function_name);
			printf(",\n");

			T_ExpList current_argument = ex_exp->u.call_exp.arguments;

			while (current_argument != NULL) {
				IRPrinter_printExExpression(current_argument->tr_exp);
				if (current_argument->next != NULL)
					printf(",\n");
				else
					printf(")\n");

				current_argument = current_argument->next;
				
			}
			return;
		}
		case TR_ESEQ: {
			printf("\n---BEGINING OF EXPRESSION SEQUENCE----\n");

			T_Exp exp_seq_head = ex_exp;

			while (exp_seq_head != NULL) {
				if (exp_seq_head->kind != TR_ESEQ) {
					IRPrinter_printExExpression(exp_seq_head);
					break;
				}
				IRPrinter_printNxExpression(exp_seq_head->u.seq_exp.statement);
				exp_seq_head = exp_seq_head->u.seq_exp.next;
			}

			printf("\n---END OF EXPRESSION SEQUENCE---\n");
			return;
		}
		default: return;
	}
}


void IRPrinter_printNxExpression(T_Stm nx_exp) {
	if (nx_exp == NULL)
		return;
	
	switch (nx_exp->kind) {
		case TR_MOVE: {
			printf("\n------MOVE---------\n");
			printf("\n MOVE DEST BELOW \n");

			IRPrinter_printExExpression(nx_exp->u.move.source);

			printf("\n MOVE RES BELOW \n");

			IRPrinter_printExExpression(nx_exp->u.move.result);

			printf("\n-----END OF MOVE-------\n");
			return;
		}
		case TR_EXP: {
			printf("\n------EXPRESSION STATEMENT------\n");

			IRPrinter_printExExpression(nx_exp->u.exp);

			printf("\n------ END OF EXPRESSION STATEMENT----\n");
			return;
		}
		case TR_JUMP: {
			printf("\n-----JUMP STATEMENT-----\n");

			printf("\n JUMP DESTINATION EXP BELOW \n");
			IRPrinter_printExExpression(nx_exp->u.jump.destination);

			printf("\n CANDIDATE LABELS \n");
			
			TempLabelList current_label_node = nx_exp->u.jump.labels;
			
			while (current_label_node != NULL) {
				TempLabel label = current_label_node->temp_label;
				printf("Valid label at %p, ", label);
				current_label_node = current_label_node->next;
			}
			printf("\n");

			
			printf("\n------ END OF JUMP STATEMENT-----\n");
			return;
		}
		case TR_CJUMP: {
			printf("\n----- CONDITIONAL JUMP------\n");

			printf("LEFT EXP BELOW\n");
			IRPrinter_printExExpression(nx_exp->u.cjump.left);
			printf("RIGHT EXP BELOW\n");
			IRPrinter_printExExpression(nx_exp->u.cjump.right);
			

			printf("True label at %p, False label at %p\n", nx_exp->u.cjump.true_dest, nx_exp->u.cjump.false_dest);
			printf("\n--- END OF CONDITIONAL JUMP ---\n");
			return;
		}
		case TR_LABEL: {
			printf("\n------ LABEL AT: %p------\n", nx_exp->u.label);
			return;
		}
		case TR_SEQ: {
			printf("\n----START OF STATEMENT SEQ----\n");
			T_Stm current_seq = nx_exp;
			while (current_seq != NULL) {
				if (current_seq->kind != TR_SEQ) {
					IRPrinter_printNxExpression(current_seq);
					break;
				}

				IRPrinter_printNxExpression(current_seq->u.seq.stm);
				current_seq = current_seq->u.seq.next;
			}

			printf("\n----END OF STATEMENT SEQ----\n");
			return;
		}
		default: return;
	}
}

void IRPrinter_printCxExpression(T_Cx cx_exp) {
	if (cx_exp == NULL) {
		printf("\nEMPTY CX\n");
		return;
	}

	printf("\n---CX EXPRESSION----\n");
	printf("Conditional statement\n");
	IRPrinter_printNxExpression(cx_exp->stm);
	
	Tr_PatchList current_true = cx_exp->trues;
	Tr_PatchList current_false = cx_exp->falses;
	
	printf("True Labels\n");
	while (current_true != NULL) {
		printf("True patch made with label at %p\n", current_true->label);
		current_true = current_true->next;
	}

	printf("False Labels\n");
	while (current_false != NULL) {
		printf("False patch made with label at %p\n", current_false->label);
		current_false = current_false->next;
	}
		
	printf("\n---END OF CX EXPRESSION---\n");
}

void IRPrinter_printFragments(void) {
	F_CodeFragmentList fragments = frame_ReturnFragmentList();
	if (fragments == NULL) {
		printf("\nEMPTY PROGRAM FRAGMENTS\n");
		return;
	}

	while (fragments != NULL) {
		F_CodeFragment fragment = fragments->fragment;
		switch(fragment->kind) {
			case StringFrag: {
				printf("\n STRING FRAG FOUND\n");
				printf(
					"LABEL(%p): %s\n", 
					fragment->u.string_fragment.label, 
					fragment->u.string_fragment.txt
				);
				break;
			}
			case ProcFrag: {
				printf("\n PROC FRAGMENT FOUND\n");
				IRPrinter_printFrame(fragment->u.procedure_fragment.frame);
				IRPrinter_printNxExpression(fragment->u.procedure_fragment.block);

				break;
			}
			default: break;
		}
		fragments = fragments->next;
	}
}


void IRPrinter_printFrame(Frame frame) {
	// Rewrite test when resolved situation on locals
	if (frame == NULL) {
		printf("\n NULL FRAME\n");
		return;
	}
	
	printf("\n FRAME INFORMATION\n");

	F_AccessList parameters = frame_parameters(frame);
	int used_space = 0;
	int used_registers = 0;

	TempLabel frame_label = frame_name(frame);
	F_Access static_link_access = frame_static_link(frame);
	
	while (parameters != NULL) {
		F_Access parameter_access = parameters->access;

		if (does_escape(parameter_access)) {
			printf("\n Frame-Resident Access at: %d\n", get_frame_offset(parameter_access));
			used_space++;
		}
		else {
			printf("\n Register-resident Access at: %p\n", parameter_access);
			used_registers++;
		}
		parameters = parameters->next;
	}

	
}

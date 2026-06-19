#include "semant/escape.h"



void analyze_escapes(A_Stm root, Environment escape_environment, int starting_depth) {
	if (root == NULL)
		return;

	switch(root->kind) {
		case Decl_Stm:
			return;
		case Exp_Stm:
			traverseExp(root->u.exp_stm.exp, starting_depth);
		case Compound_Stm: {
			analyze_escapes(root->u.compound_stm.stm1, starting_depth);
			analyze_escapes(root->u.compound_stm.stm2, starting_depth);
		}
	};
}

static void traverseExp(Environment escape_environment, int starting_depth, A_Exp expression) {
	if (expression == NULL)
		return;
	switch (expression->kind) {
		case ID_Exp: {
			string id = expression->u.id_xp.identifier;
			Symbol variable = get_symbol(escape_environment, id);

			if (variable != NULL) {
				if (variable->u.environment_entry.depth < starting_depth) 
					variable->u.environment_entry.escapes = true;
			}

			return;
		};
		case Field_Exp: {
			if (expression->u.field_exp.field->kind == Subscript_Field) {
			}
			else if (expression->u.field_exp.field->kind == Ref_Field) {
				traverse_
			}
		}
		case Unary_Exp: {
			traverseExp(escape_environment, starting_depth, expression->u.unary_exp.exp);
			return;
		}
		case Op_Exp: {
			traverseExp(escape_environment, starting_depth, expression->u.op_exp.exp1);
			traverseExp(escape_environment, starting_depth, expression->u.op_exp.exp2);
			return;
		}
		case Assign_Exp: {}
		case Let_Exp: {
			// Process declarations
			A_DecList declarations = expression->u.let_exp.declarations;
			while (declarations != NULL) {
				traverseDeclarations(escape_environment, starting_depth, declarations->dec);
				declarations = declarations->next;
			}
			// Process sequence
			traverseExp(escape_environment, starting_depth, expression->u.let_exp.block);
			return;
		}
		case While_Exp: {
			traverseExp(escape_environment, starting_depth, expression->u.while_exp.cond);
			traverseExp(escape_environment, starting_depth, expression->u.while_exp.block);
			return;
		}
		case For_Exp: {
			traverseExp(escape_environment, starting_depth, expression->u.for_exp.low_id);
			traverseExp(escape_environment, starting_depth, expression->u.for_exp.low);
			traverseExp(escape_environment, starting_depth, expression->u.for_exp.high);
			traverseExp(escape_environment, starting_depth, expression->u.for_exp.block);
			return;
		}
		default:
			return;

	}
}
static void traverseDeclarations(Environment escape_environment, int starting_depth, A_Dec declaration) {
}

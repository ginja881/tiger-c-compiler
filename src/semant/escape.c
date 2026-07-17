#include "semant/escape.h"

static Environment traverseExp(Environment escape_environment, int starting_depth, A_Exp expression);
static Environment traverseDeclarations(Environment escape_environment, int starting_depth, A_Dec declaration);

Environment analyze_escapes(A_Exp root, Environment escape_environment, int starting_depth) {
	if (root == NULL)
		return escape_environment;

	escape_environment = traverseExp(escape_environment, starting_depth, root);
	return escape_environment;
}

static Environment traverseExp(Environment escape_environment, int starting_depth, A_Exp expression) {
	if (expression == NULL)
		return escape_environment;
	switch (expression->kind) {
		case ID_Exp: {
			string id = expression->u.id_exp.identifier;
			Symbol variable = get_symbol(escape_environment, id);

			if (variable != NULL) {
				if (variable->environment_entry->u.escape_entry.depth < starting_depth) 
					variable->environment_entry->u.escape_entry.escapes = true;
			}

			return escape_environment;
		};
		case Field_Exp: {
			if (expression->u.field_exp.field->kind == Subscript_Field) {
				string array_name = expression->u.field_exp.field->u.subscript_field.id;
				Symbol array = get_symbol(escape_environment, array_name);
				if (array != NULL) {
					if (array->environment_entry->u.escape_entry.depth < starting_depth)
						array->environment_entry->u.escape_entry.escapes = true;
				}
			}
			else if (expression->u.field_exp.field->kind == Ref_Field) 
				escape_environment = traverseExp(escape_environment, starting_depth, expression->u.field_exp.field->u.ref_field.left_id);
			return escape_environment;
		}
		case Unary_Exp: {
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.unary_exp.exp);
			return escape_environment;
		}
		case Op_Exp: {
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.op_exp.exp1);
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.op_exp.exp2);
			return escape_environment;
		}
		case Assign_Exp: {
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.assign_exp.identifier);
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.assign_exp.val);
			return escape_environment;
		}
		case Let_Exp: {
			// Process declarations
			A_DecList declarations = expression->u.let_exp.declarations;
			int new_depth = starting_depth + 1;
			while (declarations != NULL) {
				escape_environment = traverseDeclarations(escape_environment, new_depth, declarations->dec);
				declarations = declarations->next;
			}
			// Process sequence
			escape_environment = traverseExp(escape_environment, new_depth, expression->u.let_exp.block);
			return escape_environment;
		}
		case While_Exp: {
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.while_exp.cond);
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.while_exp.block);
			return escape_environment;
		}
		case For_Exp: {
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.for_exp.low_id);
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.for_exp.low);
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.for_exp.high);
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.for_exp.block);
			return escape_environment;
		}
		case Seq_Exp: {
			A_ExpList expressions = expression->u.seq_exp.exp_list;
			while (expressions != NULL) {
				escape_environment = traverseExp(escape_environment, starting_depth, expressions->exp);
				expressions = expressions->next;
			}
			return escape_environment;
		}
		case Callee_Exp: {
			A_ExpList arguments = expression->u.callee_exp.args;
			while (arguments != NULL) {
				escape_environment = traverseExp(escape_environment, starting_depth, arguments->exp);
				arguments = arguments->next;
			}
			return escape_environment;
		}
		case If_Exp: {
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.if_exp.cond);
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.if_exp.then);
			escape_environment = traverseExp(escape_environment, starting_depth, expression->u.if_exp.else_block);
			return escape_environment;
		}
		default:
			return escape_environment;

	}
	return escape_environment;
}
static Environment traverseDeclarations(Environment escape_environment, int starting_depth, A_Dec declaration) {
	if (declaration == NULL)
		return escape_environment;
	switch (declaration->kind) {
		case Simple_Var_Dec: {
			string var_name = declaration->u.simple_var_dec.id;
			Symbol new_variable = make_symbol(var_name, make_escape_entry(starting_depth));
			escape_environment = insert_symbol(escape_environment, new_variable);
			escape_environment = traverseExp(escape_environment, starting_depth, declaration->u.simple_var_dec.val);
			return escape_environment;
		}
		case Field_Var_Dec: {
			string var_name = declaration->u.field_var_dec.field->u.ty_field.id;
			Symbol new_variable = make_symbol(var_name, make_escape_entry(starting_depth));
			escape_environment = insert_symbol(escape_environment, new_variable);
			escape_environment = traverseExp(escape_environment, starting_depth, declaration->u.field_var_dec.val);
			return escape_environment;
		}
		case Func_Dec: {
			int new_depth = starting_depth + 1;
			
			A_FieldList parameters = declaration->u.func_dec.args;
			while (parameters != NULL) {
				A_Field field = parameters->field;
				
				string parameter_name = field->u.ty_field.id;
				Symbol parameter = make_symbol(parameter_name, make_escape_entry(new_depth));
				escape_environment = insert_symbol(escape_environment, parameter);
				
				parameters = parameters->next;
			}

			escape_environment =  traverseExp(escape_environment, new_depth, declaration->u.func_dec.block);
			return escape_environment;
		}
		default:
			return escape_environment;
	}
	return escape_environment;
}

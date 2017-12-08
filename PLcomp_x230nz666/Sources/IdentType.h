#pragma once

enum SymbolType {
	// null type
	nul, eof,

	// value type
	ident_type, int_val, dec_val,

	// the follow are data types
	int_datatype, dec_datatype,

	// the follow are operators
	plus_op, minus_op, times_op, slash_op, mod_op, eql_op, neq_op, lss_op, leq_op, gtr_op, geq_op, lparen_op, rparen_op, lbrace_op, rbrace_op, comma_op, semicolon_op, colon_op, period_op, dblplus_op, dblminus_op, to_op,

	// the follow are sentences
	var_sym, as_sym, func_sym, const_sym, ref_sym, if_sym, unless_sym, else_sym, while_sym, repeat_sym, until_sym, break_sym, continue_sym, for_sym, in_sym, step_sym, print_sym, read_sym, return_sym, odd_sym, call_sym
};
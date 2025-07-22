#ifndef CMD_H_
#define CMD_H_

#define F(string_literal) (PSTR(string_literal))
// See https://stackoverflow.com/a/5459929
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

void handle_command_input();

#endif

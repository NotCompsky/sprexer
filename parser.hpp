#pragma once

extern "C" {
# include <lexbor/html/html.h>
}


class Parser {
 public:
	lxb_html_parser_t parser;
	
	Parser();
	
	lxb_html_document_t* parse(const char* html,  const size_t html_sz);
};

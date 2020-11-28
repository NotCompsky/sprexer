#include "parser.hpp"
#include <stdexcept>


Parser::Parser()
{
	if (lxb_html_parser_init(&this->parser) != LXB_STATUS_OK)
		throw std::runtime_error("Cannot create parser");
}

lxb_html_document_t* Parser::parse(const char* html,  const size_t html_sz){
	return lxb_html_parse(&this->parser, reinterpret_cast<const lxb_char_t*>(html), html_sz);
}

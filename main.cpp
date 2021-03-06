#include <cstdio>
#include "element.hpp"
#include "doc.hpp"
#include "parser.hpp"
#include <compsky/utils/nullstrview.hpp>


typedef unsigned char uchar;


std::string_view find_element_attr(const sprexer::Doc& doc,  char* const selector_path,  const char* const attr){
	sprexer::Element element(doc.get_element_from_class_selector_path(selector_path));
	if (element.is_null())
		return compsky::utils::nullstrview;
	return element.get_value(attr);
}


int main(int argc,  char* const* argv){
	if (argc != 4){
		printf(
			"USAGE\n"
			"    HTML_FILE SELECTOR_PATH ATTRIBUTE\n"
			#include "help.hpp"
		);
		return 0;
	}
	
	FILE* f = fopen(argv[1], "rb");
	if (f == nullptr)
		return 1;
	
	char html[1000 * 1024];
	const size_t html_sz = fread(html,  1,  sizeof(html) - 1,  f);
	if (html_sz == 0)
		return 2;
	
	sprexer::Parser parser;
	sprexer::Doc doc(parser, html, html_sz);
	const std::string_view v = find_element_attr(doc, argv[2], argv[3]);
	if (v == compsky::utils::nullstrview)
		printf("No such element/attribute found\n");
	else
		printf("%.*s\n", (int)v.size(), v.data());
	
	return 0;
}

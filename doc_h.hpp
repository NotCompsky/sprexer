#pragma once

extern "C" {
# include <lexbor/dom/dom.h>
}
#include "parser.hpp"


template<size_t n_elements>
class Collection;


class Doc {
  public:
	lxb_html_document_t* const document;
	
	Doc(Parser& parser,  const char* html,  const size_t html_sz);
	
	~Doc();
	
	lxb_dom_document_t* get_dom_doc() const;
	
	template<size_t n_elements>
	bool get_elements_of_class_name(Collection<n_elements>& collection,  const char* name) const;
	
	lxb_dom_element_t* get_element_from_class_selector_path(char* selector_path) const;
};

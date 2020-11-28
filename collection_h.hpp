#pragma once

extern "C" {
# include <lexbor/dom/dom.h>
}
#include "doc_h.hpp"


template<size_t n_elements>
class Collection {
  private:
	void* collection_array_list[n_elements];
  public:
	lxb_dom_collection_t collection;
	
	Collection();
	Collection(const Doc& doc);
	
	lxb_dom_element_t* at(const size_t i) const;
	
	size_t length() const;
	
	void filter_for_elements_with_tag_name(const char* const tag_name);
};

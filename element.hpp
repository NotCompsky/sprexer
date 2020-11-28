#pragma once

extern "C" {
# include <lexbor/dom/dom.h>
}
#include "collection_h.hpp"
#include <string_view>


class Element {
  public:
	lxb_dom_element_t* const element;
	
	Element(lxb_dom_node_t* node);
	Element(lxb_dom_element_t* _el);
	
	template<size_t n>
	Element(const Collection<n>& collection,  size_t i);
	
	bool is_null() const;
	
	bool has_id(const char* const id) const;
	bool has_tag_name(const char* const tag_name) const;
	bool has_attr(const char* const attr_name,  const char* const attr_val) const;
	bool has_class_name(const char* const class_name) const;
	
	std::string_view get_inner_text() const;
	std::string_view get_attr(const char* const name) const;
	std::string_view get_value(const char* const name) const;
	
	void print_tag_name() const;
	
	bool to_str(char* buf,  size_t& buf_sz) const;
	
	void print() const;
	
	template<size_t n_elements>
	bool get_elements_of_tag_name(Collection<n_elements>& collection,  const char* name) const;
	
	template<size_t n_elements>
	bool get_elements_of_class_name(Collection<n_elements>& collection,  const char* name) const;
	
	lxb_dom_element_t* get_element_given_tag_class_attr_indx(const char* id_name,  const char* tag_name,  const char* class_name,  const char* attr_name,  const char* attr_val,  const unsigned indx) const;
	
	lxb_dom_element_t* get_direct_child_element_given_tag_class_attr_indx(const char* id_name,  const char* tag_name,  const char* class_name,  const char* attr_name,  const char* attr_val,  const unsigned indx) const;
};


template<size_t n>
Element::Element(const Collection<n>& collection,  size_t i)
: Element(collection.at(i))
{}


template<size_t n_elements>
bool Element::get_elements_of_tag_name(Collection<n_elements>& collection,  const char* name) const {
	return (lxb_dom_elements_by_tag_name(this->element, &collection.collection, reinterpret_cast<const lxb_char_t*>(name), strlen(name)) != LXB_STATUS_OK);
}

template<size_t n_elements>
bool Element::get_elements_of_class_name(Collection<n_elements>& collection,  const char* name) const {
	return (lxb_dom_elements_by_class_name(this->element, &collection.collection, reinterpret_cast<const lxb_char_t*>(name), strlen(name)) != LXB_STATUS_OK);
}

#pragma once

#include "doc.hpp"
#include "notnull.hpp"
#include "str_utils.hpp"


Doc::Doc(Parser& parser,  const char* html,  const size_t html_sz)
: document(parser.parse(html, html_sz))
{
	assert_not_null(this->document);
}

Doc::~Doc(){
	lxb_html_document_destroy(this->document);
}

lxb_dom_document_t* Doc::get_dom_doc() const {
	return &this->document->dom_document;
}

template<size_t n_elements>
bool Doc::get_elements_of_class_name(Collection<n_elements>& collection,  const char* name) const {
	return Element(lxb_dom_interface_element(this->document)).get_elements_of_class_name(collection, name);
}

bool is_not_gt(const char* const a,  const char* const b){
	return ((a == nullptr) or (b < a));
}

char* null_if_lt(char* const a,  char* const b){
	return is_not_gt(a, b) ? b : nullptr;
}

void if_not_null_then_set_prev_char_to_zero(char* str){
	if (str)
		str[-1] = 0;
}

lxb_dom_element_t* Doc::get_element_from_class_selector_path(char* selector_path) const {
	/* selector_path being GLOBAL@TAG.CLASS#ID:ATTR=VALUE+INDEX>...>
	 * GLOBAL: If equal to *, then it is a walk, otherwise it only searches direct children
	 * e.g. (omitting >:ATTRIBUTE):
	 *   @div>@a                       <div><a></a></div>
	 *   @div.display-box>@img         <div class="display-box"><img/></div>
	 *   .display:onclick>@a          <??? class="display" onclick=???><a></a></???>
	 *   .foo:data-bar=ree           <??? class="foo" data-bar="ree"></???>
	 *   @div>>>@a                     <div><???><???><???><a></a></???></???></???>
	 *   div+2                       <div/><div/><div/>  (3rd div)
	 * 
	 * If > immediately precedes a number of ^, then go up levels according to the number of ^
	 * 
	 * Only the first element is a genuine global search - all the subsequent levels only filter direct child nodes. WARNING: Currently, it is assumed that a tag_name or class_name is supplied in the top-most level.
	 */
	lxb_dom_element_t* element = this->document->dom_document.element;
	while(true){
		const bool is_walk = selector_path[0] == '*';
		char* id_name    = after_next_char(selector_path, '#');
		char* tag_name   = after_next_char(selector_path, '@');
		char* class_name = after_next_char(selector_path, '.');
		char* attr_name  = after_next_char(selector_path, ':');
		char* attr_val   = after_next_char(selector_path, '=');
		char* indx_val   = after_next_char(selector_path, '+');
		selector_path = after_next_char(selector_path, '>');
		id_name    = null_if_lt(selector_path, id_name);
		tag_name   = null_if_lt(selector_path, tag_name);
		class_name = null_if_lt(selector_path, class_name);
		attr_name  = null_if_lt(selector_path, attr_name);
		attr_val   = null_if_lt(selector_path, attr_val);
		unsigned indx = 0;
		if (indx_val and is_not_gt(selector_path, indx_val))
			indx = a2n<unsigned>(indx_val);
		
		if_not_null_then_set_prev_char_to_zero(selector_path);
		if_not_null_then_set_prev_char_to_zero(id_name);
		if_not_null_then_set_prev_char_to_zero(tag_name);
		if_not_null_then_set_prev_char_to_zero(class_name);
		if_not_null_then_set_prev_char_to_zero(attr_name);
		if_not_null_then_set_prev_char_to_zero(attr_val);
		
		if (is_walk)
			element = Element(element).get_element_given_tag_class_attr_indx(id_name, tag_name, class_name, attr_name, attr_val, indx);
		else
			element = Element(element).get_direct_child_element_given_tag_class_attr_indx(id_name, tag_name, class_name, attr_name, attr_val, indx);
		
		if (element == nullptr)
			break;
		
		if (selector_path == nullptr)
			break;
		if (*selector_path == '^'){
			while (*selector_path == '^'){
				element = lxb_dom_interface_element(element->node.parent);
				++selector_path;
			}
			if (*selector_path == 0)
				break;
			element = lxb_dom_interface_element(element->node.parent);
			// Have to do it once more to cancel out the immediate stepping down a level
		}
	}
	return element;
}

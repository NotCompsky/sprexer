#include "element.hpp"
#include "collection_h.hpp"
#include "str_utils.hpp"
#include "nullstr.hpp"


Element::Element(lxb_dom_element_t* _el)
: element(_el)
{}

Element::Element(lxb_dom_node_t* node)
: element(lxb_dom_interface_element(node))
{}


bool Element::is_null() const {
	return this->element == nullptr;
}

bool Element::has_id(const char* const id) const {
	if (id == nullptr)
		return true;
	if (this->element->node.type != LXB_DOM_NODE_TYPE_ELEMENT)
		return false;
	size_t len = 0;
	const char* const actual_id = reinterpret_cast<const char*>(lxb_dom_element_get_attribute(this->element, (const lxb_char_t*)"id", 2, &len));
	return streq(id, actual_id, len);
}

bool Element::has_tag_name(const char* const tag_name) const {
	if (tag_name == nullptr)
		return true;
	if (this->element->node.type != LXB_DOM_NODE_TYPE_ELEMENT)
		return false;
	size_t len = 0;
	const char* const actual_tag_name = reinterpret_cast<const char*>(lxb_dom_element_qualified_name(this->element, &len));
	return streq(tag_name, actual_tag_name, len);
}

bool Element::has_attr(const char* const attr_name,  const char* const attr_val) const {
	if (attr_name == nullptr)
		return true;
	if (this->element->node.type != LXB_DOM_NODE_TYPE_ELEMENT)
		return false;
	size_t len = 0;
	const char* const actual_val = reinterpret_cast<const char*>(lxb_dom_element_get_attribute(this->element, (const lxb_char_t*)attr_name, strlen(attr_name), &len));
	if (actual_val == nullptr)
		return false;
	return ((attr_val == nullptr) or streq(attr_val, actual_val, len));
}

bool Element::has_class_name(const char* const class_name) const {
	if (class_name == nullptr)
		return true;
	size_t len = 0;
	const char* const actual_class_name = reinterpret_cast<const char*>(lxb_dom_element_get_attribute(this->element, (const lxb_char_t*)"class", 5, &len));
	// TODO: Account for whitespace
	return streq(class_name, actual_class_name, len);
}

std::string_view Element::get_inner_text() const {
	lxb_dom_node_t* node = this->element->node.first_child;
	while (node != nullptr){
		if (node->type == LXB_DOM_NODE_TYPE_TEXT)
			break;
		node = node->next;
	}
	if (node == nullptr)
		return null_str_view;
	lexbor_str_t str = lxb_dom_interface_text(node)->char_data.data;
	return std::string_view(reinterpret_cast<const char*>(str.data), str.length);
}

std::string_view Element::get_attr(const char* const name) const {
	size_t len = 0;
	const char* const value = reinterpret_cast<const char*>(lxb_dom_element_get_attribute(this->element, (const lxb_char_t*)name, strlen(name), &len));
	return std::string_view(value, len);
}

std::string_view Element::get_value(const char* const name) const {
	if ((name[0] == '.') and (name[1] == 0))
		return this->get_inner_text();
	return get_attr(name);
}

void Element::print_tag_name() const {
	size_t len = 0;
	const char* tag_name;
	switch (this->element->node.type){
		case LXB_DOM_NODE_TYPE_ELEMENT:
			tag_name = reinterpret_cast<const char*>(lxb_dom_element_qualified_name(this->element, &len));
			break;
		case LXB_DOM_NODE_TYPE_TEXT:
			tag_name = "[text]";
			len = 6;
			break;
		default:
			tag_name = "[not a node or text]";
			len = 20;
	}
	printf("%.*s\n", (int)len, tag_name);
}

bool Element::to_str(char* buf,  size_t& buf_sz) const {
	lexbor_str_t buf_view;
	buf_view.data = reinterpret_cast<lxb_char_t*>(buf);
	buf_view.length = buf_sz;
	if (lxb_html_serialize_str(lxb_dom_interface_node(this->element), &buf_view) != LXB_STATUS_OK)
		return true;
	buf_sz = buf_view.length;
	return false;
}

void Element::print() const {
	char buf[1000];
	size_t buf_len = sizeof(buf)-1;
	this->to_str(buf, buf_len);
	printf("%.*s\n", (int)buf_len, buf);
}

lxb_dom_element_t* Element::get_element_given_tag_class_attr_indx(const char* id_name,  const char* tag_name,  const char* class_name,  const char* attr_name,  const char* attr_val,  unsigned indx) const {
	// Walks until it finds an element satisfying all conditions
	
	lxb_dom_node_t* node = this->element->node.first_child;
	while (node != nullptr){
		// Inspired by https://github.com/lexbor/lexbor/blob/51930b4071459ac5144e48af1f053d34a782fa89/source/lexbor/dom/interfaces/element.c
		Element _element(node);
		if (_element.has_id(id_name) and _element.has_tag_name(tag_name) and _element.has_class_name(class_name) and _element.has_attr(attr_name, attr_val)){
			// Matched. Now see if it is at the correct "index"
			if (indx == 0)
				return _element.element;
			--indx;
		}
		if (node->first_child != nullptr)
			node = node->first_child;
		else {
			while((node != &this->element->node) and (node->next == nullptr))
				node = node->parent;
			if (node == &this->element->node)
				return nullptr;
			node = node->next;
		}
	}
	
	return nullptr;
}

lxb_dom_element_t* Element::get_direct_child_element_given_tag_class_attr_indx(const char* id_name,  const char* tag_name,  const char* class_name,  const char* attr_name,  const char* attr_val,  unsigned indx) const {
	lxb_dom_node_t* node = this->element->node.first_child;
	while (node != nullptr){
		if ((node->type == LXB_DOM_NODE_TYPE_ELEMENT) and (not lxb_html_node_is_void(node))){
			Element _element(node);
			if (_element.has_id(id_name))
				if (_element.has_tag_name(tag_name))
					if (_element.has_class_name(class_name))
						if (_element.has_attr(attr_name, attr_val))
							if (indx-- == 0)
								break;
		}
		node = node->next;
	}
	return node ? lxb_dom_interface_element(node) : nullptr;
}

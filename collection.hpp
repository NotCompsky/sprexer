#pragma once

#include "collection_h.hpp"
#include "element.hpp"


template<size_t n_elements>
Collection<n_elements>::Collection()
{
	this->collection.array.size = n_elements;
	this->collection.array.length = 0;
	this->collection.array.list = collection_array_list;
}

template<size_t n_elements>
Collection<n_elements>::Collection(const Doc& doc)
: Collection()
{
	this->collection.document = doc.get_dom_doc();
}

template<size_t n_elements>
lxb_dom_element_t* Collection<n_elements>::at(const size_t i) const {
	return (i < this->length()) ? (lxb_dom_element_t*)this->collection.array.list[i] : nullptr; //lxb_dom_collection_element(&collection, i);
}

template<size_t n_elements>
size_t Collection<n_elements>::length() const {
	return this->collection.array.length; //lxb_dom_collection_length(this->collection);
}

template<size_t n_elements>
void Collection<n_elements>::filter_for_elements_with_tag_name(const char* const tag_name){
	/*
	 * Iterate through each element in collection; if it doesn't fit the filter, its place is passed to a future element (to avoid memory allocations)
	 */
	size_t j = 0;
	for (size_t i = 0;  i < this->collection.array.length;  ++i){
		Element element(this->at(i));
		if (not element.has_tag_name(tag_name)){
			--this->collection.array.length;
		} else {
			if (j != i){
				this->collection.array.list[j] = element.element;
			}
			++j;
		}
	}
}

#pragma once

#include <stdexcept>


inline
void assert_not_null(void* x){
	if (x==nullptr)
		throw std::runtime_error("Unexpected nullptr");
}

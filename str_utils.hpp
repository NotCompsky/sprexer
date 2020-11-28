#pragma once


template<typename Char>
Char* after_next_char(Char* str,  const char c){
	while(true){
		if (*str == 0)
			return nullptr;
		if (*str == c)
			return str + 1;
		++str;
	}
}

template<typename Char>
bool streq(const char* a,  Char* b){
	if (a == nullptr)
		return true;
	if (b == nullptr)
		return false;
	while(true){
		if (*a != *b)
			return false;
		if (*a == 0)
			return true;
		++a;
		++b;
	}
}


template<typename Char>
bool streq(const char* a,  Char* b,  unsigned length_to_compare){
	if (a == nullptr)
		return true;
	if (b == nullptr)
		return false;
	while(length_to_compare){
		if (*a != *b)
			return false;
		++a;
		++b;
		--length_to_compare;
	}
	return (*a == 0);
}

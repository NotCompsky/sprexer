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
	/*
	 * Compares the script-given string (a) to the HTML-given string (b)
	 */
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


template<char terminator = 0>
constexpr
bool streq(const char* a,  const std::string_view v){
	unsigned length_to_compare = v.size();
	const char* b = v.data();
	//printf("streq\n  1 %s\n  2 %s\n", a, b); fflush(stdout);
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
	return (*b == terminator);
}

static_assert(streq<' '>("foo bar", "foo"));
static_assert(streq<' '>("foo  bar", "foo"));
static_assert(not streq<' '>("foo  bar", "bar"));
static_assert(not streq<' '>(" foo  bar", "foo"));



constexpr
bool streq_up_to_space(const char* const a,  const std::string_view v){
	if (a == nullptr)
		return true;
	unsigned length_to_compare = v.size();
	const char* b = v.data();
	if (b == nullptr)
		return false;
	while(true){
		if (streq<' '>(a, std::string_view(b, length_to_compare)))
			return true;
		while((*b != 0) and (*b != ' ')){
			++b;
			--length_to_compare;
		}
		while(*b == ' '){
			++b;
			--length_to_compare;
		}
		if (*b == 0)
			return false;
	}
}


static_assert(streq_up_to_space("foo bar", "foo"));
static_assert(streq_up_to_space("foo  bar", "foo"));
static_assert(streq_up_to_space("foo bar", "bar"));
static_assert(streq_up_to_space("foo  bar", "bar"));
static_assert(streq_up_to_space(" foo  bar", "foo"));
static_assert(streq_up_to_space(" foo  bar ree", "bar"));

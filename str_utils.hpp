#pragma once
#ifdef ASSERT_STR_UTILS
# include <cassert>
#endif


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
constexpr
char at(Char* str,  const unsigned i){
	return str[i];
}

constexpr
char at(const std::string_view v,  const unsigned i){
	return v.at(i);
}

template<typename Char>
constexpr
bool is_null(Char* str){
	return (str == nullptr);
}

constexpr
bool is_null(const std::string_view){
	return false;
}

template<typename Char>
constexpr
size_t len(Char* str){
	return std::char_traits<char>::length(str);
}

constexpr
size_t len(const std::string_view v){
	return v.size();
}

template<char terminator = 0,  typename Str>
constexpr
bool streq(const char* a,  Str const b){
	/*
	 * Compares the script-given string (a) to the HTML-given string (b)
	 * (a) definitely ends in a null byte
	 */
	if (is_null(a))
		return true;
	if (is_null(b))
		return false;
	const size_t b_len = len(b);
	if (b_len == 0){
		return (a[0] == 0);
	}
	for (size_t i = 0;  i < b_len;  ++i){
		const char _b = at(b, i);
		if (a[i] == 0){
			return (i == b_len - 1) or (_b == terminator);
		}
		if (_b != a[i])
			return false;
	}
	// If here, (a) has a prefix of (b)
	return (a[b_len] == 0);
}



constexpr
bool streq_up_to_space(const char* const a,  const std::string_view v){
	if (is_null(a))
		return true;
	unsigned length_to_compare = v.size();
	const char* b = v.data();
	if (b == nullptr)
		return false;
	while(length_to_compare){
		if (streq<' '>(a, std::string_view(b, length_to_compare)))
			return true;
		while((*b != 0) and (*b != ' ') and length_to_compare){
			++b;
			--length_to_compare;
		}
		while((*b == ' ') and length_to_compare){
			++b;
			--length_to_compare;
		}
	}
	return false;
}


#ifdef ASSERT_STR_UTILS
// Left is the specified string, right is the HTML string
assert(streq<' '>("", ""));
assert(not streq<' '>("", "foo"));
assert(not streq<' '>("foo", ""));
assert(streq<' '>("foo bar", "foo bar"));
assert(streq<' '>("foo", "foo bar"));
assert(streq<' '>("foo", "foo  bar"));
assert(not streq<' '>("bar", "foo  bar"));
assert(not streq<' '>("foo", " foo  bar"));
assert(not streq<' '>("foo bar", "foo"));
assert(not streq<' '>("foo  bar", "foo"));
assert(not streq<' '>("foo  bar", "bar"));
assert(not streq<' '>(" foo  bar", "foo"));

assert(streq<' '>("", std::string_view("")));
assert(not streq<' '>("", std::string_view("foo")));
assert(not streq<' '>("foo", std::string_view("")));
assert(streq<' '>("foo bar", std::string_view("foo bar")));
assert(streq<' '>("foo", std::string_view("foo bar")));
assert(streq<' '>("foo", std::string_view("foo  bar")));
assert(not streq<' '>("bar", std::string_view("foo  bar")));
assert(not streq<' '>("foo", std::string_view(" foo  bar")));
assert(not streq<' '>("foo bar", std::string_view("foo")));
assert(not streq<' '>("foo  bar", std::string_view("foo")));
assert(not streq<' '>("foo  bar", std::string_view("bar")));
assert(not streq<' '>(" foo  bar", std::string_view("foo")));

assert(not streq_up_to_space("", "foo"));
assert(not streq_up_to_space("foo", ""));
assert(streq_up_to_space("foo bar", "foo bar"));
assert(streq_up_to_space("foo", "foo bar"));
assert(streq_up_to_space("foo", "foo  bar"));
assert(streq_up_to_space("bar", "foo  bar"));
assert(streq_up_to_space("foo", " foo  bar"));
assert(not streq_up_to_space("foo bar", "foo"));
assert(not streq_up_to_space("foo  bar", "foo"));
assert(not streq_up_to_space("foo  bar", "bar"));
assert(not streq_up_to_space(" foo  bar", "foo"));
#endif

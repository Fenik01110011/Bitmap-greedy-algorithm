#include <iostream>
#include "Polskie_znaki.h"

using namespace std;

string pl(string s)
{
	char z[] = "\245\206\251\210\344\242\230\276\253\244\217\250\235\343\340\227\275\215¹æê³ñóœ¿Ÿ¥ÆÊ£ÑÓŒ¯";
	for (int i = 0; i < (int)s.length(); ++i)
		for (int j = 0; j < 18; ++j)
			if (s[i] == z[j + 18]) { s[i] = z[j]; break; }
	return s;
}
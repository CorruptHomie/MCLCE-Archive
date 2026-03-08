#include "stdafx.h"
#include <xhash>

#include "Hasher.h"

Hasher::Hasher(wstring &salt)
{
	this->salt = salt;
}

wstring Hasher::getHash(wstring &name)
{
	wstring s = wstring( salt ).append( name );
	std::hash<wstring> hasher;
	return _toString( hasher( s ) );
}
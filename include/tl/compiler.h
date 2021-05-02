#pragma once
#include "list.h"

namespace TL {

TL_API List<ascii> demangle(ascii const *function_name);

}

#ifdef TL_IMPL
#if COMPILER_MSVC

#pragma warning(push, 0)
#include <DbgHelp.h>
#pragma warning(pop)

#pragma comment(lib, "dbghelp")

#include "thread.h"

namespace TL {

Mutex dbg_mutex;

List<ascii> demangle(ascii const *function_name) {
	DWORD flags =
		  UNDNAME_32_BIT_DECODE
		| UNDNAME_COMPLETE
		//| UNDNAME_NAME_ONLY
		| UNDNAME_NO_ACCESS_SPECIFIERS
		| UNDNAME_NO_ALLOCATION_LANGUAGE
		| UNDNAME_NO_ALLOCATION_MODEL
		//| UNDNAME_NO_ARGUMENTS
		| UNDNAME_NO_CV_THISTYPE
		| UNDNAME_NO_FUNCTION_RETURNS
		| UNDNAME_NO_LEADING_UNDERSCORES
		| UNDNAME_NO_MEMBER_TYPE
		| UNDNAME_NO_MS_KEYWORDS
		| UNDNAME_NO_MS_THISTYPE
		| UNDNAME_NO_RETURN_UDT_MODEL
		| UNDNAME_NO_SPECIAL_SYMS
		| UNDNAME_NO_THISTYPE
		| UNDNAME_NO_THROW_SIGNATURES
	;

	List<ascii> result;
	result.reserve(256);
	scoped_lock(dbg_mutex);
	result.size = UnDecorateSymbolName(function_name, result.data, result.capacity, flags);
	return result;
}


}

#endif
#endif

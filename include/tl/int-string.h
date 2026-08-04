#pragma once
#include "int.h"
#include "string.h"

namespace tl {

template <umm bits, bool sign>
void append(StringBuilder &builder, Int<bits, sign> i) {
    return append(builder, FormattedInt{i});
}

}

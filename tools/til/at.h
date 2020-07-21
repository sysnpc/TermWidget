#pragma once

#include <stddef.h>

namespace til {
  // The at function declares that you've already sufficiently checked that your
  // array access is in range before retrieving an item inside it at an offset.
  // This is to save double/triple/quadruple testing in circumstances where you
  // are already pivoting on the length of a set and now want to pull elements
  // out of it by offset without checking again. gsl::at will do the check
  // again. As will .at(). And using [] will have a warning in audit.
  template <class T>
  constexpr auto at(T& cont, const size_t i) -> decltype(cont[cont.size()]) {
    return cont[i];
  }
}  // namespace til

#include <iostream>
#include "FlacErrorCodes.h"
#include "FlacSubframe.h"

namespace flac {
int FlacSubframe::write(std::ostream &) const
{
  return 0;
}
}

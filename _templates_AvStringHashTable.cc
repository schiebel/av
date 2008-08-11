#include "AvList.cc"
#include "AvStringHashTable.h"
#include "AvUnits.h"
template class AvList<AvStringHashTable<AvUnitDef *> *>;
template class AvListItem<AvStringHashTable<AvUnitDef *> *>;
#include "AvStringHashTable.cc"
template class AvStringHashTable<AvStringHashTable<AvUnitDef *> *>;
template class AvStringHashTable<AvUnitDef *>;
template class AvStringHashTable<AvUnitPrefixDef *>;
template class AvStringHashTable<AvString>;
template class AvStringHashTable<int>;

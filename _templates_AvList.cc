#include "AvUnits.h"
#include "AvImageViewItem.h"
#include "AvImageDataItem.h"
#include "AvList.cc"
template class AvList<AvUnitDef *>;
template class AvList<AvUnitPrefixDef *>;
template class AvListItem<AvUnitDef *>;
template class AvListItem<AvUnitPrefixDef *>;
template class AvListIter<AvUnitDef *>;
template class AvListIter<AvUnitPrefixDef *>;
template class AvList<AvImageViewItem *>;
template class AvListItem<AvImageViewItem *>;
template class AvListIter<AvImageViewItem *>;
template class AvList<AvImageDataItem *>;
template class AvListIndexIter<AvImageDataItem *>;
template class AvListItem<AvImageDataItem *>;
template class AvListIter<AvImageDataItem *>;

#include "stdafx.h"
#include <CppDb/details/AtomUtils.hpp>

#include <functional>

namespace CppAbstractDataBase { namespace Details {
  
  namespace {
    typedef std::hash<std::string> tStdStringHasher;
    tStdStringHasher const g_StdStringHasher;
  } /// end unnamed namespace
  
  tAtom CreateAtom(std::string const& text) {
    return g_StdStringHasher(text);
  }
}} /// end namespace CppAbstractDataBase::Details

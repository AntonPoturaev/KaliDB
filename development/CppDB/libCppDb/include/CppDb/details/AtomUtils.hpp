#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_ATOM_UTILS_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_ATOM_UTILS_HPP__

#include <CppDb/Atom.hpp>

#include <string>

namespace CppAbstractDataBase { namespace Details {
  tAtom CreateAtom(std::string const& text);
  
  constexpr tAtom NullAtom() noexcept {
    return tAtom(-1);
  }
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_ATOM_UTILS_HPP__

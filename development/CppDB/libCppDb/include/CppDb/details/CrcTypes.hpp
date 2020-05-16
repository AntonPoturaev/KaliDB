#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_CRC_TYPES_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_CRC_TYPES_HPP__

#include <boost/crc.hpp>

namespace CppAbstractDataBase { namespace Details {
  typedef boost::crc_32_type tCrcAlgorithm;
} /// end namespace Details
  typedef Details::tCrcAlgorithm::value_type tCrcValue;
} /// end namespace CppAbstractDataBase

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_CRC_TYPES_HPP__

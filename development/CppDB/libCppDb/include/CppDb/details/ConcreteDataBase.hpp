#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_CONCRETE_DATA_BASE_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_CONCRETE_DATA_BASE_HPP__

#include "ManagerFwd.hpp"
#include "BaseDataBase.hpp"
#include "FileSystemStructure.hpp"
#include "ControllInformation.hpp"
#include "Allocator.hpp"

#include <tuple>
#include <stdexcept>
#include <typeinfo>

#include <boost/format.hpp>

namespace CppAbstractDataBase { namespace Details {
    template<typename Container>
    class ConcreteDataBase final : public BaseDataBase {
      friend class CppAbstractDataBase::Manager;
      
      typedef std::atomic<Container*> tContainerPtr;
    public:
      
      virtual tAbstractContainerPtr GetAbstractContainer() override final {
        return m_Container;
      }
      
      virtual tAbstractContainerConstPtr GetAbstractContainer() const override final {
        return m_Container;
      }
      
    private:
      ConcreteDataBase(FileSystemStructure const& fsStruct, tControllInformationOptional&& ctrlInfo)
        : BaseDataBase(fsStruct, std::forward<tControllInformationOptional>(ctrlInfo))
        , m_Container(
            m_Memory.find_or_construct<Container>(GetDBName().c_str())(
              Allocator<Container>(m_Memory.get_segment_manager())
            )
          )
      {
      }
      
      virtual void OnGrow() override final {
        m_Memory = tManagedMappedFile(boost::interprocess::open_only, GetDBFilePath().string().c_str());
        
        size_t count = 0; 
        std::tie(m_Container, count) = m_Memory.find<Container>(GetDBName().c_str());
        if (count == 0) {
          throw std::runtime_error(boost::str(boost::format(
            "ConcreteDataBase<%1%>::OnGrow - filed to update internal data after last request to grow"
          ) % typeid(Container).name()));
        }
      }
      
    private:
      tContainerPtr m_Container;
    };
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_CONCRETE_DATA_BASE_HPP__

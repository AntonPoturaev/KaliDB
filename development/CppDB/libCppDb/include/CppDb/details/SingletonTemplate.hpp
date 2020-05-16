#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_SINGLETON_TEMPLATE_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_SINGLETON_TEMPLATE_HPP__

#include <memory>
#include <mutex>

namespace CppAbstractDataBase { namespace Details {
  template<typename InstanceT>
  class Singleton {
    typedef InstanceT tInstance;
    typedef std::unique_ptr<tInstance> tInstancePtr;
    
    typedef std::recursive_mutex tInstanceAccess;
    typedef std::lock_guard<tInstanceAccess> const tInstanceLock;
    
  public:
    static tInstance& Instance() {
      if (!m_Instance) {
        tInstanceLock lock(m_Access);
        
        if (!m_Instance) {
          m_Instance.reset(new tInstance);
        }
      }
      
      return *m_Instance;
    }
    
    static void KillInstance() {
      tInstanceLock lock(m_Access);
      m_Instance.reset();
    }
    
  protected:
    Singleton() = default;
    
  private:
    static tInstancePtr m_Instance;
    static tInstanceAccess m_Access;
  };
  
  template<typename InstanceT>
  typename Singleton<InstanceT>::tInstancePtr Singleton<InstanceT>::m_Instance;
  
  template<typename InstanceT>
  typename Singleton<InstanceT>::tInstanceAccess Singleton<InstanceT>::m_Access;
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_SINGLETON_TEMPLATE_HPP__

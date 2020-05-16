#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_PERIODIC_TIMER_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_PERIODIC_TIMER_HPP__

#include <functional>
//#include <thread>

#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>

namespace CppAbstractDataBase { namespace Details {
  class PeriodicTimer final {
  public:
    typedef std::function<void(boost::system::error_code const&)> tOnTick;
    
  public:
    ~PeriodicTimer();
    PeriodicTimer();
    
    void Cancel();
    void Wait(tOnTick&& tick, std::chrono::nanoseconds const& period);
    
  private:
    boost::asio::io_service m_IoSrv;
    boost::asio::steady_timer m_Timer;
    //std::thread m_Work;
  };
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_PERIODIC_TIMER_HPP__

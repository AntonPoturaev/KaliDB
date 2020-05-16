#include "stdafx.h"
#include <CppDb/details/PeriodicTimer.hpp>

namespace CppAbstractDataBase { namespace Details {
  PeriodicTimer::~PeriodicTimer() {
    Cancel();
  }
  
  PeriodicTimer::PeriodicTimer()
    : m_IoSrv()
    , m_Timer(m_IoSrv)
    //, m_Work(&boost::asio::io_service::run, &m_IoSrv)
  {
    m_IoSrv.run();
  }
  
  void PeriodicTimer::Cancel() {
    boost::system::error_code ec;
    m_Timer.cancel(ec);
  }
  
  void PeriodicTimer::Wait(tOnTick&& tick, std::chrono::nanoseconds const& period) {
    m_Timer.expires_from_now(period);
    m_Timer.async_wait(std::forward<tOnTick>(tick));
  }
}} /// end namespace CppAbstractDataBase::Details

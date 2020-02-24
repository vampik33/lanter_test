#include "frame.hpp"
#include <stdexcept>

using namespace frame;

CFrame::CFrame( size_t firstBall, size_t secondBall, CFrame* const prevFrame, bool lastFrame )
  : m_firstBall( [&firstBall] () { if ( firstBall > MAX_FRAME_SCORE ) throw std::range_error( "[CFrame] : firstBall > MAX_FRAME_SCORE" );  return firstBall; }( ) )
  , m_secondBall( [&firstBall, &secondBall] () { if ( firstBall + secondBall > MAX_FRAME_SCORE ) throw std::range_error( "[CFrame] : firstBall + secondBall > MAX_FRAME_SCORE" );  return secondBall; }( ) )
  , m_strike( m_firstBall == MAX_FRAME_SCORE )
  , m_spare( ( m_firstBall + m_secondBall == MAX_FRAME_SCORE ) && !m_strike )
  , m_prevFrame( prevFrame )
  , m_prevPrevFrame( ( nullptr != prevFrame ) ? prevFrame->m_prevFrame : nullptr )
  , m_lastFrame( lastFrame )
{
  //! Посчитать счёт
  m_score = m_firstBall + m_secondBall;

  //! Обновить счёт предыдущих фреймов
  if ( nullptr != m_prevFrame )
  {
    //! Предыдущий бросок - страйк
    if ( m_prevFrame->strike() )
    {
      if ( nullptr != m_prevPrevFrame )
      {
        //! Второй предыдущий бросок - страйк
        if ( m_prevPrevFrame->strike() )
          m_prevPrevFrame->m_score += m_firstBall;
      }
      
      //! Если это не последний бросок
      if( !m_lastFrame )
        m_prevFrame->m_score += m_firstBall + m_secondBall;
    }
    else if ( m_prevFrame->spare() ) //! Предыдущий бросок spare
      m_prevFrame->m_score += m_firstBall;
  }
}

size_t CFrame::score() const noexcept
{
  return m_score;
}

bool CFrame::strike() const noexcept
{
  return m_strike;
}

bool CFrame::spare() const noexcept
{
  return m_spare;
}

size_t CFrame::first_ball() const noexcept
{
  return m_firstBall;
}

size_t CFrame::second_ball() const noexcept
{
  return m_secondBall;
}
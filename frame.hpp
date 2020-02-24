#ifndef FRAME_HPP
#define FRAME_HPP

#include <iostream>
#include <memory>

namespace frame
{

const size_t MAX_FRAME_SCORE = 10;

//! Класс, описывающая фрейм
class CFrame
{
public:
  CFrame( size_t firstBall, size_t secondBall, CFrame* const prevFrame = nullptr, bool lastFrame = false );

  //! Получить счет фрейма
  size_t score() const noexcept;

  //! Получить флаги фрейма
  bool strike() const noexcept;
  bool spare() const noexcept;

  //! Получить значение первого и второго мяча
  size_t first_ball() const noexcept;
  size_t second_ball() const noexcept;

  //! Потоковый вывод
  friend std::ostream& operator<<( std::ostream& os, const CFrame& fr )
  {
    os << "[" << fr.score() << "]";
    return os;
  }

private:
  const size_t              m_firstBall;        //! Очки за первый мяч
  const size_t              m_secondBall;       //! Очки за второй мяч
  const bool                m_strike;           //! Флаг того, что в это фрейме был страйк
  const bool                m_spare;            //! Флаг того, что в это фрейме был спаре
  const bool                m_lastFrame;        //! Флаг того, что это последний фрейм
  CFrame* const             m_prevFrame;        //! Указатель на предыдущий фрейм
  CFrame* const             m_prevPrevFrame;    //! Указатель на фрейм за один до текущего
  size_t                    m_score;            //! Общий счет данного фрейма
};

}

#endif

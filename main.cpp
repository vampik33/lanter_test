#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

#include "nlohmann/json.hpp"

#include "frame.hpp"

const size_t MAX_FRAME_NUMBER = 10;
using players_map_type = std::map< std::string, std::vector< frame::CFrame > >;
using players_value_type = players_map_type::value_type;

//! FIXME - добавить проверки

//! Печатаем счет
void PrintScore( const players_map_type& players )
{
	//! Общий счет
	std::map<std::string, size_t> score;
	std::for_each( std::cbegin( players ), std::cend( players ),
								 [&score] ( const players_value_type& player )
	{
		score.insert( { player.first, 0 } );
	} );

	std::cout << "\n" << "!!!!!!!!!!" << std::endl;
	for ( auto player : players )
	{
		std::cout << "Player " << player.first << " score" << std::endl;
		for ( auto fr : player.second )
		{
			std::cout << fr << "\t";
			score[player.first] += fr.score();
		}
		std::cout << std::endl << "[Overal score] " << score[player.first] << std::endl;
	}
	std::cout << "!!!!!!!!!!" << "\n" << std::endl;
}

//! Сохраняем результат игры в json
void SaveJsonFile( const players_map_type& players )
{
	//! Создаем объект json
	nlohmann::json jsonFile;
	for ( auto player : players )
	{
		size_t frameI = 0;
		for ( auto fr : player.second )
		{
			jsonFile[player.first]["frames"][frameI]["first_ball"] = fr.first_ball();
			jsonFile[player.first]["frames"][frameI]["second_ball"] = fr.second_ball();
			jsonFile[player.first]["frames"][frameI]["strike"] = fr.strike();
			jsonFile[player.first]["frames"][frameI]["spare"] = fr.spare();
			jsonFile[player.first]["frames"][frameI]["score"] = fr.score();
			frameI++;
		}
	}

	//! Открываем файл и записываем
	std::ofstream out( "result.json" );
	out << jsonFile.dump();
}

int main(int, char const *[])
{
	try
	{
		//! Карта игроков и их фреймов
		players_map_type players;

		//! Заполняем данные по игрокам
		size_t playersNmb = 0;
		std::cout << "Enter players number: ";
		std::cin >> playersNmb;
		for ( auto i = 0; i < playersNmb; i++ )
		{
			std::string playerName;
			std::cout << "Enter " << i + 1 << " player name: ";
			std::cin >> playerName;

			//! Проверка что такой игрок уже есть
			while ( 0 != players.count( playerName ) )
			{
				std::cout << "This player already exist";
				std::cout << "Enter " << i + 1 << " player name: ";
				std::cin >> playerName;
			}
			
			players[playerName].reserve( MAX_FRAME_NUMBER + 2 );
		}

		//! Игра - цикл по фреймам
		auto playersEnd = std::end( players );
		for ( size_t frameNmb = 0; frameNmb < MAX_FRAME_NUMBER; frameNmb++ )
		{
			//! Для каждого игрока цикл
			for ( auto player = std::begin( players ); player != playersEnd; player++ )
			{
				size_t firstBall = 0, secondBall = 0;
				std::cout << "\n" << "Player " << player->first << " | Frame number " << frameNmb + 1 << std::endl;
				std::cout << "Enter first ball: ";
				std::cin >> firstBall;
				if ( frame::MAX_FRAME_SCORE != firstBall )
				{
					std::cout << "Enter second ball: ";
					std::cin >> secondBall;
				}

				//! Заполняем класс фрейма
				player->second.push_back( frame::CFrame( firstBall, secondBall, !player->second.empty() ? &player->second.back() : nullptr ) );

				//! Проверяем последний ли это бросок и проводим дополнительные броски
				if ( frameNmb == MAX_FRAME_NUMBER - 1 )
				{
					//! Последний бросок - страйк
					if ( player->second.back().strike() )
					{
						size_t firstAdBall = 0;
						std::cout << "Additional first ball: ";
						std::cin >> firstBall;
						player->second.push_back( frame::CFrame( firstBall, 0, !player->second.empty() ? &player->second.back() : nullptr, true ) );

						size_t secondAdBall = 0;
						std::cout << "Additional second ball: ";
						std::cin >> secondBall;
						player->second.push_back( frame::CFrame( secondBall, 0 ) );
					}
					else if ( player->second.back().spare() ) //! Последний бросок - spare
					{
						size_t firstAdBall = 0;
						std::cout << "Additional ball: ";
						std::cin >> firstBall;
						player->second.push_back( frame::CFrame( firstBall, 0 ) );
					}
				}
			}

			//! Распечатываем счёт за данный фрейм
			PrintScore( players );
		}

		//! Общий счет
		std::map<std::string, size_t> score;
		std::for_each( std::cbegin( players ), std::cend( players ),
									 [&score] ( const players_value_type& player )
		{
			score[player.first] = std::accumulate( std::cbegin( player.second ), std::cend( player.second ), size_t(0),
																						 [] ( size_t val1, const auto& val2 )
			{
				return val1 + val2.score();
			} );
		} );

		//! Победитель
		using pair_type = decltype( score )::value_type;
		auto winner = std::max_element( std::cbegin( score ), std::cend( score ), [] ( const pair_type& p1, const pair_type& p2 ) { return p1.second < p2.second; } );
		
		std::cout << std::endl << "WINNER " << winner->first << " | With score " << winner->second << std::endl;

		//! Сохраняем результаты игры
		SaveJsonFile( players );
	}
	catch ( const std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}
	catch ( ... )
	{
		std::cerr << "unknown error" << std::endl;
		return -2;
	}

  return 0;
}

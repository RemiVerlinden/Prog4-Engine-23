#pragma once
#include "Singleton.h"

class Gamemode : public engine::Singleton<Gamemode>
{
public:
	enum class GameType
	{
		SinglePlayer,
		Coop,
		Versus
	};
	GameType GetGamemode() const { return m_Type; }
	void SetGamemode(GameType type) { m_Type = type; }
private:
	GameType m_Type;
};
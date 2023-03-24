#pragma once
#include "Ship.h"


#define BOSS_SIDE_KICKS_STATE_TO_SHIP_CONTROL_INPUTS_MAP_B \
		{ State0, std::vector{ MoveUp, FireWeapon1, FireWeapon2}}, \
		{ State1, std::vector{ MoveRight }}, \
		{ State2, std::vector{ MoveLeft } }, \
		{ State3, std::vector{ MoveDown, FireWeapon1, FireWeapon2 } }

#define BOSS_SIDE_KICKS_STATE_TO_SHIP_CONTROL_INPUTS_MAP \
		{ State0, std::vector{ MoveUp, FireWeapon1, FireWeapon2}},\
		{ State1, std::vector{ MoveLeft }},\
		{ State2, std::vector{ MoveRight } },\
		{ State3, std::vector{ MoveDown, FireWeapon1, FireWeapon2 } }

#define BOSS_SIDE_KICKS_STATE_WITH_INPUT_TO_STATE_MAP \
		{ State0, { \
			{Hold,		State0},\
			{Input1,	State1},\
			{Input2,	State2} }\
		},\
		{ State1, {\
			{Hold,		State1},\
			{Input1,	State0}}\
		},\
		{ State2, {\
			{Hold,		State2},\
			{Input1,	State0}}\
		},\
		{ State3, {\
			{Hold,		State3},\
			{Input1,	State0}}\
		}



#define BOSS_STATE_WITH_INPUT_TO_STATE_MAP_CONFIG \
			{State0, {	\
				{Hold,	State0},								\
				{Input1,	State3},							\
				{Input2,	State1},							\
				{Input3,	State2} }							\
			},													\
			{ State1, {											\
				{Hold,		State0},							\
				{Input1,	State1}}							\
			},													\
			{ State2, {											\
				{Hold,		State0},\
				{Input1,	State2}}\
			},\
			{ State3, { \
				{Hold,		State3}, \
				{Input1,	State0}, \
				{Input2,	State4}, \
				{Input3,	State5}} \
			}, \
			{ State4, { \
				{Hold,		State4}, \
				{Input1,	State3}, \
				{Input2, State6}} \
			}, \
			{ State5, { \
				{Hold,		State5}, \
				{Input1,	State3}, \
				{Input2, State6}} \
			}, \
			{ State6, { \
				{Hold,		State0}, \
				{Input1,		State0}, \
				{Input2,		State0}, \
				{Input3,		State0}} \
			}


#define BOSS_STATE_TO_SHIP_CONTROL_INPUT_MAP_CONFIG \
		{ State0, std::vector{ MoveUp, FireWeapon1}},\
		{ State1, std::vector{ MoveLeft, FireWeapon1 }},	\
		{ State2, std::vector{ MoveRight, FireWeapon1 } },	\
		{ State3, std::vector{ MoveDown, FireWeapon1 } },	\
		{ State4, std::vector{ MoveUp, MoveRight } },		\
		{ State5, std::vector{ MoveUp, MoveLeft } },		\
		{ State6, std::vector{ Rotate } }

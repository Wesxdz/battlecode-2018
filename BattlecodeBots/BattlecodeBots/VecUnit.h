#ifndef VEC_UNIT_H
#define VEC_UNIT_H

#include "bc.h"
#include "Unit.h"
#include "Structure.h"
#include "Robot.h"
#include "Factory.h"
#include "Robot.h"
#include "Worker.h"
#include "Healer.h"
#include "Knight.h"
#include "Ranger.h"
#include "Mage.h"

namespace VecUnit
{
	/*
	@note deletes @bcUnits
	*/
	template <class T>
	static std::vector<T> Wrap(bc_VecUnit* bcUnits);

	template<>
	inline std::vector<units::Unit> Wrap(bc_VecUnit* bcUnits)
	{
		auto units = std::vector<units::Unit>();
		for (uintptr_t i = 0; i < bc_VecUnit_len(bcUnits); i++) {
			units.push_back(units::Unit(bc_VecUnit_index(bcUnits, i)));
		}
		delete_bc_VecUnit(bcUnits);
		return units;
	}

	template<>
	inline std::vector<units::Structure> Wrap(bc_VecUnit* bcUnits)
	{
		auto units = std::vector<units::Structure>();
		for (uintptr_t i = 0; i < bc_VecUnit_len(bcUnits); i++) {
			units.push_back(units::Structure(bc_VecUnit_index(bcUnits, i)));
		}
		delete_bc_VecUnit(bcUnits);
		return units;
	}

	template<>
	inline std::vector<units::Robot> Wrap(bc_VecUnit* bcUnits)
	{
		auto units = std::vector<units::Robot>();
		for (uintptr_t i = 0; i < bc_VecUnit_len(bcUnits); i++) {
			units.push_back(units::Robot(bc_VecUnit_index(bcUnits, i)));
		}
		delete_bc_VecUnit(bcUnits);
		return units;
	}

	template<>
	inline std::vector<units::Worker> Wrap(bc_VecUnit* bcUnits)
	{
		auto units = std::vector<units::Worker>();
		for (uintptr_t i = 0; i < bc_VecUnit_len(bcUnits); i++) {
			units.push_back(units::Worker(bc_VecUnit_index(bcUnits, i)));
		}
		delete_bc_VecUnit(bcUnits);
		return units;
	}

	template<>
	inline std::vector<units::Knight> Wrap(bc_VecUnit* bcUnits)
	{
		auto units = std::vector<units::Knight>();
		for (uintptr_t i = 0; i < bc_VecUnit_len(bcUnits); i++) {
			units.push_back(units::Knight(bc_VecUnit_index(bcUnits, i)));
		}
		delete_bc_VecUnit(bcUnits);
		return units;
	}

	template<>
	inline std::vector<units::Mage> Wrap(bc_VecUnit* bcUnits)
	{
		auto units = std::vector<units::Mage>();
		for (uintptr_t i = 0; i < bc_VecUnit_len(bcUnits); i++) {
			units.push_back(units::Mage(bc_VecUnit_index(bcUnits, i)));
		}
		delete_bc_VecUnit(bcUnits);
		return units;
	}

	template<>
	inline std::vector<units::Healer> Wrap(bc_VecUnit* bcUnits)
	{
		auto units = std::vector<units::Healer>();
		for (uintptr_t i = 0; i < bc_VecUnit_len(bcUnits); i++) {
			units.push_back(units::Healer(bc_VecUnit_index(bcUnits, i)));
		}
		delete_bc_VecUnit(bcUnits);
		return units;
	}

	template<>
	inline std::vector<units::Ranger> Wrap(bc_VecUnit* bcUnits)
	{
		auto units = std::vector<units::Ranger>();
		for (uintptr_t i = 0; i < bc_VecUnit_len(bcUnits); i++) {
			units.push_back(units::Ranger(bc_VecUnit_index(bcUnits, i)));
		}
		delete_bc_VecUnit(bcUnits);
		return units;
	}
};


#endif


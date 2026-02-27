#pragma once
#include <ProfessionData.h>

namespace rm::rule {

	/**
	 * @class TableColumnProfessionMatcher
	 * @brief Class to identify the column index in a table using ProfessionData objects as the header
	 *
	 * @implements column_row_matcher
	 */
	class TableColumnProfessionMatcher {
	public:

		/**
		 * @brief Get the index of the table column based on the profession ID
		 * @param prof ProfessionData object to match
		 * @return int table column index
		 */
		int column(const ProfessionData* prof) const {
			if (prof->id() == "PROFESSION_FIGHTER") return 0;
			if (prof->id() == "PROFESSION_THIEF") return 1;
			if (prof->id() == "PROFESSION_ROGUE") return 2;
			if (prof->id() == "PROFESSION_WARRIOR_MONK") return 3;
			if (prof->id() == "PROFESSION_LAYMAN") return 4;
			if (prof->id() == "PROFESSION_HEALER") return 5;
			if (prof->id() == "PROFESSION_MYSTIC") return 6;
			if (prof->id() == "PROFESSION_WARLOCK") return 7;
			if (prof->id() == "PROFESSION_SORCERER") return 8;
			if (prof->id() == "PROFESSION_RUNEMAGE") return 9;
			if (prof->id() == "PROFESSION_SUMMONER") return 10;
			if (prof->id() == "PROFESSION_MAGICIAN") return 11;
			if (prof->id() == "PROFESSION_ILLUSIONIST") return 12;
			if (prof->id() == "PROFESSION_ESSENCE_ALCHEMIST") return 13;
			if (prof->id() == "PROFESSION_MANA_MOLDER") return 14;
			if (prof->id() == "PROFESSION_CLERIC") return 15;
			if (prof->id() == "PROFESSION_ANIMIST") return 16;
			if (prof->id() == "PROFESSION_CHANNELING_ALCHEMIST") return 17;
			if (prof->id() == "PROFESSION_PRIEST_OF_AGRICULTURE") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_ANCESTORS") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_ANIMALS") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_ARTS") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_BIRTH_CHILDREN") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_COMMUNITY") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_COMPETITION") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_CRAFTS") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_DARKNESS_NIGHT") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_DAWN") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_DEATH") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_DISEASE") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_EARTH") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_FATE_DESTINY") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_FERTILITY") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_FIRE") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_FORTUNE_LUCK") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_GUARDIANSHIP") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_HEALING") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_HUNTING") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_JUSTICE_REVENGE") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_LIGHT") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_LIGHTNING") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_LITERATURE_POETRY") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_LOVE") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_MAGIC") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_MARRIAGE") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_MESSENGERS") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_METALWORK") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_MISCHIEF_TRICKERY") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_MOON") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_MUSIC_DANCE") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_NATURE") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_OCEANS_RIVERS") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_ORACLES_PROPHECY") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_SKY_WEATHER") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_STRENGTH") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_SUN") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_THUNDER") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_VEGETATION") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_WAR") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_WIND") return 18;
			if (prof->id() == "PROFESSION_PRIEST_OF_WISDOM") return 18;
			if (prof->id() == "PROFESSION_DëRNAKWMüN_PøR") return 18;
			if (prof->id() == "PROFESSION_DëRNAKWMüN_KHAD") return 18;
			if (prof->id() == "PROFESSION_MENTALIST") return 19;
			if (prof->id() == "PROFESSION_LAY_HEALER") return 20;
			if (prof->id() == "PROFESSION_MENTALISM_ALCHEMIST") return 21;
			if (prof->id() == "PROFESSION_ARCANIST") return 22;
			if (prof->id() == "PROFESSION_WIZARD") return 23;
			if (prof->id() == "PROFESSION_RANGER") return 24;
			if (prof->id() == "PROFESSION_PALADIN") return 25;
			if (prof->id() == "PROFESSION_MYTHIC") return 26;
			if (prof->id() == "PROFESSION_MONK") return 27;
			if (prof->id() == "PROFESSION_TAOIST_MONK") return 28;
			if (prof->id() == "PROFESSION_DABBLER") return 29;
			if (prof->id() == "PROFESSION_WARRIOR_MAGE") return 30;
			if (prof->id() == "PROFESSION_BARD") return 31;
			if (prof->id() == "PROFESSION_MAGENT") return 32;
			if (prof->id() == "PROFESSION_ZEN_MONK") return 33;
			if (prof->id() == "PROFESSION_CHAOTIC") return 34;
			if (prof->id() == "PROFESSION_MAGEHUNTER") return 35;
			if (prof->id() == "PROFESSION_NIGHTBLADE") return 36;
			if (prof->id() == "PROFESSION_DERVISH") return 37;
			if (prof->id() == "PROFESSION_SWORD_DANCER") return 38;
			if (prof->id() == "PROFESSION_SEER") return 39;
			if (prof->id() == "PROFESSION_ASTROLOGER") return 40;
			if (prof->id() == "PROFESSION_ENCHANTER") return 41;
			if (prof->id() == "PROFESSION_ARMSMASTER") return 42;
			if (prof->id() == "PROFESSION_BARBARIAN") return 43;
			if (prof->id() == "PROFESSION_OUTRIDER") return 44;
			if (prof->id() == "PROFESSION_SAGE") return 45;
			if (prof->id() == "PROFESSION_SWASHBUCKLER") return 46;
			if (prof->id() == "PROFESSION_SHAO_LIN_MONK") return 47;
			if (prof->id() == "PROFESSION_ELEMENTALIST") return 48;
			if (prof->id() == "PROFESSION_ELEMANTAL_PRIEST") return 49;
			if (prof->id() == "PROFESSION_ELEMENTAL_CHAMPION") return 50;
			if (prof->id() == "PROFESSION_ARCANE_ELEMENTALIST") return 51;
			if (prof->id() == "PROFESSION_ELEMENTAL_EXPLORER") return 52;
			if (prof->id() == "PROFESSION_PRIEST_CHAN_ESS") return 53;
			if (prof->id() == "PROFESSION_PRIEST_CHAN_MENT") return 54;
			if (prof->id() == "PROFESSION_PRIEST_CHAN_ARMS") return 55;
			if (prof->id() == "PROFESSION_PRIEST_ARMS") return 56;

			return -1; // Return -1 if no match is found
		}

		/**
		 * @brief Get the total number of profession types defined as columns in the table
		 * @return int total number of profession types
		 */
		int numColumns() const {
			return 57; // Total number of profession types defined as coumns in the table, this should be updated if any new profession types are added as columns in the table
		}
	};

} // namespace rm::rule
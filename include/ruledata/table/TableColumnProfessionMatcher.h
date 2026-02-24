#pragma once
#include <Profession.h>

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
		switch (prof->id()) {
		case "PROFESSION_FIGHTER": return 0;
		case "PROFESSION_THIEF": return 1;
		case "PROFESSION_ROGUE": return 2;
		case "PROFESSION_WARRIOR_MONK": return 3;
		case "PROFESSION_LAYMAN": return 4;
		case "PROFESSION_HEALER": return 5;
		case "PROFESSION_MYSTIC": return 6;
		case "PROFESSION_WARLOCK": return 7;
		case "PROFESSION_SORCERER": return 8;
		case "PROFESSION_RUNEMAGE": return 9;
		case "PROFESSION_SUMMONER": return 10;
		case "PROFESSION_MAGICIAN": return 11;
		case "PROFESSION_ILLUSIONIST": return 12;
		case "PROFESSION_ESSENCE_ALCHEMIST": return 13;
		case "PROFESSION_MANA_MOLDER": return 14;
		case "PROFESSION_CLERIC": return 15;
		case "PROFESSION_ANIMIST": return 16;
		case "PROFESSION_CHANNELING_ALCHEMIST": return 17;
		case "PROFESSION_PRIEST_OF_AGRICULTURE":
		case "PROFESSION_PRIEST_OF_ANCESTORS":
		case "PROFESSION_PRIEST_OF_ANIMALS":
		case "PROFESSION_PRIEST_OF_ARTS":
		case "PROFESSION_PRIEST_OF_BIRTH_CHILDREN":
		case "PROFESSION_PRIEST_OF_COMMUNITY":
		case "PROFESSION_PRIEST_OF_COMPETITION":
		case "PROFESSION_PRIEST_OF_CRAFTS":
		case "PROFESSION_PRIEST_OF_DARKNESS_NIGHT":
		case "PROFESSION_PRIEST_OF_DAWN":
		case "PROFESSION_PRIEST_OF_DEATH":
		case "PROFESSION_PRIEST_OF_DISEASE":
		case "PROFESSION_PRIEST_OF_EARTH":
		case "PROFESSION_PRIEST_OF_FATE_DESTINY":
		case "PROFESSION_PRIEST_OF_FERTILITY":
		case "PROFESSION_PRIEST_OF_FIRE":
		case "PROFESSION_PRIEST_OF_FORTUNE_LUCK":
		case "PROFESSION_PRIEST_OF_GUARDIANSHIP":
		case "PROFESSION_PRIEST_OF_HEALING":
		case "PROFESSION_PRIEST_OF_HUNTING":
		case "PROFESSION_PRIEST_OF_JUSTICE_REVENGE":
		case "PROFESSION_PRIEST_OF_LIGHT":
		case "PROFESSION_PRIEST_OF_LIGHTNING":
		case "PROFESSION_PRIEST_OF_LITERATURE_POETRY":
		case "PROFESSION_PRIEST_OF_LOVE":
		case "PROFESSION_PRIEST_OF_MAGIC":
		case "PROFESSION_PRIEST_OF_MARRIAGE":
		case "PROFESSION_PRIEST_OF_MESSENGERS":
		case "PROFESSION_PRIEST_OF_METALWORK":
		case "PROFESSION_PRIEST_OF_MISCHIEF_TRICKERY":
		case "PROFESSION_PRIEST_OF_MOON":
		case "PROFESSION_PRIEST_OF_MUSIC_DANCE":
		case "PROFESSION_PRIEST_OF_NATURE":
		case "PROFESSION_PRIEST_OF_OCEANS_RIVERS":
		case "PROFESSION_PRIEST_OF_ORACLES_PROPHECY":
		case "PROFESSION_PRIEST_OF_SKY_WEATHER":
		case "PROFESSION_PRIEST_OF_STRENGTH":
		case "PROFESSION_PRIEST_OF_SUN":
		case "PROFESSION_PRIEST_OF_THUNDER":
		case "PROFESSION_PRIEST_OF_VEGETATION":
		case "PROFESSION_PRIEST_OF_WAR":
		case "PROFESSION_PRIEST_OF_WIND":
		case "PROFESSION_PRIEST_OF_WISDOM":
		case "PROFESSION_DëRNAKWMüN_PøR":
		case "PROFESSION_DëRNAKWMüN_KHAD": return 18;
		case "PROFESSION_MENTALIST": return 19;
		case "PROFESSION_LAY_HEALER": return 20;
		case "PROFESSION_MENTALISM_ALCHEMIST": return 21;
		case "PROFESSION_ARCANIST": return 22;
		case "PROFESSION_WIZARD": return 23;
		case "PROFESSION_RANGER": return 24;
		case "PROFESSION_PALADIN": return 25;
		case "PROFESSION_MYTHIC": return 26;
		case "PROFESSION_MONK": return 27;
		case "PROFESSION_TAOIST_MONK": return 28;
		case "PROFESSION_DABBLER": return 29;
		case "PROFESSION_WARRIOR_MAGE": return 30;
		case "PROFESSION_BARD": return 31;
		case "PROFESSION_MAGENT": return 32;
		case "PROFESSION_ZEN_MONK": return 33;
		case "PROFESSION_CHAOTIC": return 34;
		case "PROFESSION_MAGEHUNTER": return 35;
		//case "PROFESSION_NIGHTBLADE": return 36;
		//case "PROFESSION_DERVISH": return 37;
		//case "PROFESSION_SWORD_DANCER": return 38;
		case "PROFESSION_SEER": return 39;
		case "PROFESSION_ASTROLOGER": return 40;
		case "PROFESSION_ENCHANTER": return 41;
		case "PROFESSION_ARMSMASTER": return 42;
		case "PROFESSION_BARBARIAN": return 43;
		case "PROFESSION_OUTRIDER": return 44;
		case "PROFESSION_SAGE": return 45;
		case "PROFESSION_SWASHBUCKLER": return 46;
		//case "PROFESSION_SHAO_LIN_MONK": return 47;
		//case "PROFESSION_ELEMENTALIST": return 48;
		//case "PROFESSION_ELEMANTAL_PRIEST": return 49;
		//case "PROFESSION_ELEMENTAL_CHAMPION": return 50;
		//case "PROFESSION_ARCANE_ELEMENTALIST": return 51;
		//case "PROFESSION_ELEMENTAL_EXPLORER": return 52;
		//case "PROFESSION_PRIEST_CHAN_ESS": return 53;
		//case "PROFESSION_PRIEST_CHAN_MENT": return 54;
		//case "PROFESSION_PRIEST_CHAN_ARMS": return 55;
		//case "PROFESSION_PRIEST_ARMS": return 56;

		default:
			return -1; // Return -1 for unknown profession types
		}
	}
}

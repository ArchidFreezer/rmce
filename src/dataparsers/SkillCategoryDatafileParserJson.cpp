#include <SkillCategoryDatafileParserJson.h>
#include <SkillProgressionTypeData.h>
#include <StatType.h>

void SkillCategoryDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	SkillCategoryData& game_data = cache().get<SkillCategoryData>(id);

	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("group", toString(game_data.group()));
	datum.put("use-realm-stats", game_data.useRealmStats());
	datum.put("skill-progression", game_data.defaultSkillProgression().name());
	datum.put("category-progression", game_data.skillCategoryProgression().name());

	pt::ptree pstats;
	for (auto& stat : game_data.stats()) {
		pt::ptree pstat;
		pstat.put("", toString(stat));
		pstats.push_back(std::make_pair("", pstat));
	}
	datum.push_back(std::make_pair("stats", pstats));
}

void SkillCategoryDatafileParserJson::parse(bool id_only) {}
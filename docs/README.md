# Overview
Description of the naming structure used for both json and the c++ class member functions

There are many repeated types of information for game data objects especially related to skills and how they may be modified, with changes being
definesd at the skill, skill category and skill group level. This page attempts to document some of these modifiers and gives a naming scheme to
provide consistency across both the c++ classes and corresponding json files.

## Skill Modifiers
There are many different types of skill modifier used in races, professions, training packages, etc. and for consistency a common naming structure
needs to be applied. 

### Grouping
Modifiers may be applied at three basic grouping levels
- Skill Groups: The highest level that are never directly modified but may apply a modifier to any category or skill within
- Skill Catagories: These may have a modifier applied to the category itself of one or more skills within
- Skill: The actual skill that may or may not have a specific subcategory. These should always be defined to allow for subcategories

### Types
At the lowest level there are 3 basic changes that are applied to skills:
- Type: The skill tytpe is being modifier, e.g. Occupational, Everyman, Restricted, etc.
- Bonus: An absolute modifier to a skill or skill category bonus value. These come in two types a normal bonues or a special bonus
- Ranks: A number of skill ranks gained in a skill or skill category

### Choices
In some cases the player gets to choose one item from a set of choices on which to apply the modifier.

## Json
The top level json element name in an object should be in the form:
[skill|category|group]-[multi-][skill|category|category-and-skill]-[rank|bonus|special-bonus|type][-choice][es|s]

This is built up as follows:
### Grouping
Define the grouping: [skill|category|group]

Inside the json element there will be a tag with a corresponding name.

```json
"skill": "Swimming"
"category": "Lore | General"
"group": "Weapon"
```

## Single or multiple targets
Define if there are multiple targets for the modifier: [multi-]

In some case, such as rank modifiers, the player may select one of more skills for the given number of ranks to be spread across.
If this is the case then the element name should have [multi-] included and a tag inside should define the maximum number:

```json
"max-targets": "4"
```

### Target
Define the targets: [skill|category|category-and-skill]

Next the actual target(s) of the modifier should be defined. There are three valid possibilities: skills, categories and both.
In the case of both this indicates that a single category, plus one skill within the category both receive the same modifier,
the contained json element in this case would be the category so there are only two valid element tags:

```json
"skill": "Swimming"
"category": "Lore | General"
```

### Modifier
Define what is being modified: [rank|bonus|special-bonus|type]

The modifier type should then be defined by an element with the same name. An exception is for modifiers to a special bonus where
the element tags should not include 'special':

```json
"ranks": "2"
"bonus": "15"
"type": "Occupational"
```

## Choices
Define whether the player may select one or more targets: [-choice]

If the grouping is either _group_ or _category_ and the target(s) are one or more contained within then there is the possiblity that
the player may be able to select the specific target from a set. If this is the case then it should be indicated.

## Postfix
The top level element should always be plural, using the appropriate postfix: [s|es]



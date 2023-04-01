import sys
import os
 
scenes = [
"Inside the Deku Tree",
"Dodongo's Cavern",
"Inside Jabu-Jabu's Belly",
"Forest Temple",
"Fire Temple",
"Water Temple",
"Spirit Temple",
"Shadow Temple",
"Bottom of the Well",
"Ice Cavern",
"Ganon's Tower",
"Gerudo Training Ground",
"Thieves' Hideout",
"Inside Ganon's Castle",
"Ganon's Tower (Collapsing)",
"Inside Ganon's Castle (Collapsing)",
"Treasure Box Shop",
"Gohma's Lair",
"King Dodongo's Lair",
"Barinade's Lair",
"Phantom Ganon's Lair",
"Volvagia's Lair",
"Morpha's Lair",
"Twinrova's Lair & Nabooru's Mini-Boss Room",
"Bongo Bongo's Lair",
"Ganondorf's Lair",
"Tower Collapse Exterior",
"Market Entrance (Child - Day)",
"Market Entrance (Child - Night)",
"Market Entrance (Ruins)",
"Back Alley (Child - Day)",
"Back Alley (Child - Night)",
"Market (Child - Day)",
"Market (Child - Night)",
"Market (Ruins)",
"Temple of Time Exterior (Child - Day)",
"Temple of Time Exterior (Child - Night)",
"Temple of Time Exterior (Ruins)",
"Know-It-All Brothers' House",
"House of Twins",
"Mido's House",
"Saria's House",
"Carpenter Boss's House",
"Back Alley House (Man in Green)",
"Bazaar",
"Kokiri Shop",
"Goron Shop",
"Zora Shop",
"Kakariko Potion Shop",
"Market Potion Shop",
"Bombchu Shop",
"Happy Mask Shop",
"Link's House",
"Back Alley House (Dog Lady)",
"Stable",
"Impa's House",
"Lakeside Laboratory",
"Carpenters' Tent",
"Gravekeeper's Hut",
"Great Fairy's Fountain (Upgrades)",
"Fairy's Fountain",
"Great Fairy's Fountain (Spells)",
"Grottos",
"Grave (Redead)",
"Grave (Fairy's Fountain)",
"Royal Family's Tomb",
"Shooting Gallery",
"Temple of Time",
"Chamber of the Sages",
"Castle Hedge Maze (Day)",
"Castle Hedge Maze (Night)",
"Cutscene Map",
"Dampé's Grave & Windmill",
"Fishing Pond",
"Castle Courtyard",
"Bombchu Bowling Alley",
"Ranch House & Silo",
"Guard House",
"Granny's Potion Shop",
"Ganon's Tower Collapse & Battle Arena",
"House of Skulltula",
"Hyrule Field",
"Kakariko Village",
"Graveyard",
"Zora's River",
"Kokiri Forest",
"Sacred Forest Meadow",
"Lake Hylia",
"Zora's Domain",
"Zora's Fountain",
"Gerudo Valley",
"Lost Woods",
"Desert Colossus",
"Gerudo's Fortress",
"Haunted Wasteland",
"Hyrule Castle",
"Death Mountain Trail",
"Death Mountain Crater",
"Goron City",
"Lon Lon Ranch",
"Ganon's Castle Exterior",
]

# getting the name of the directory
# where the this file is present.
current = os.path.dirname(os.path.realpath(__file__))
 
# Getting the parent directory name
# where the current directory is present.
parent = os.path.dirname(current)
 
# adding the parent directory to
# the sys.path.
sys.path.append(parent)

from LocationList import *
from version import __version__

print(__version__)
print("Location,Type,Scene,Dungeon/Overworld")
for location in location_table:
    loc_type, scene, default, address, item, categories = location_table[location]
    if loc_type not in ["Event", "HintStone", "Drop", "Hint"]:
        line = location + "," + loc_type + ","
        line += (scenes[scene] if scene is not None and scene < len(scenes) else "")
        line += ","
        if(categories):
            if ("Master Quest" in categories or "Master Quest Dungeons" in categories) and ("Vanilla" in categories or "Vanilla Dungeons" in categories):
                line += "Vanilla/MQ"
            elif "Master Quest" in categories or "Master Quest Dungeons" in categories:
                line += "MQ"
            elif "Vanilla" in categories or "Vanilla Dungeons" in categories:
                line += "Vanilla"
            else:
                line += "Overworld"
            
        print(line)
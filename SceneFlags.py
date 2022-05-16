from LocationList import location_table
from math import ceil
import logging

# Creates a list containing the highest flag used for each scene. 
# Returns: List of tuples (scene, max_freestanding_flag, max_drop_flag)
def get_scene_flag_table(world):
    scene_flags = []
    for i in range(0, 101):
        max_freestanding_flag = 0
        max_drop_flag = 0
        for location in world.get_locations():
            if(location.type == "Collectable"):
                if(location.scene == i):
                    if("Freestanding" in location.filter_tags or "ActorOverride" in location.filter_tags or "Silver Rupee" in location.filter_tags):
                        if location.default > max_freestanding_flag:
                            max_freestanding_flag = location.default
                    if("Crate" in location.filter_tags or "SmallCrate" in location.filter_tags or "Pot" in location.filter_tags or "FlyingPot" in location.filter_tags or "Beehive" in location.filter_tags or "Drop" in location.filter_tags or "RupeeTower" in location.filter_tags):
                        if location.default > max_drop_flag:
                            max_drop_flag = location.default
        scene_flags.append((i, max_freestanding_flag, max_drop_flag))
    return scene_flags

# Convert the scene flag table generated using get_scene_flag_table to byte arrays to be stored in the ROM
# Returns: tuple (freestanding_flag_table_bytes, drop_flag_table_bytes, num_freestanding_flags, num_drop_flags)
def get_scene_flag_table_bytes(scene_flag_table):
    #Create the byte arrays
    freestanding_flag_table_bytes = bytearray(101)
    drop_flag_table_bytes = bytearray(101)
    num_freestanding_flags = 0
    num_drop_flags = 0
    
    # Loop through each scene in the table, setting the value for each scene to the cumulative count of words used.
    for scene in scene_flag_table:
        freestanding_flag_table_bytes[scene[0]] = num_freestanding_flags
        drop_flag_table_bytes[scene[0]] = num_drop_flags
        num_freestanding_flags += 0 if scene[1] == 0 else ceil((scene[1] + 1)/32)
        num_drop_flags += 0 if scene[2] == 0 else ceil((scene[2] + 1)/32)
    return (freestanding_flag_table_bytes, drop_flag_table_bytes, num_freestanding_flags, num_drop_flags)
from LocationList import *
from os import path
from Utils import read_json

world_dir = 'data\\World'
files = ['Overworld.json', 'Deku Tree.json', 'Dodongos Cavern.json', 'Jabu Jabus Belly.json', 'Forest Temple.json', 'Fire Temple.json', 'Water Temple.json', 'Shadow Temple.json', 'Spirit Temple.json', 'Ganons Castle.json', 'Bottom of the Well.json', 'Gerudo Training Ground.json', 'Ice Cavern.json']

i = 0


locations = []

for file in files:
    file_path = path.join(path.curdir, world_dir, file)

    data = read_json(file_path)
    for region in data:
        if region.get("locations"):
            printed_region_name = False
            for location in region["locations"]:
                locations.append(location)
                if location_table.get(location):
                    if location_table.get(location)[0] == 'ActorOverride':
                        if not printed_region_name:
                            print(region.get("region_name"))
                            printed_region_name = True
                        print(str(i) + ": " + location + ": " + region["locations"].get(location))
                        i = i + 1
                    elif location_table.get(location)[5]:
                        if 'Freestanding' in location_table.get(location)[5]:
                            if not printed_region_name:
                                print(region.get("region_name"))
                                printed_region_name = True
                            print(str(i) + ": " + location + ": " + region["locations"].get(location))
                            i = i + 1

print("***")
for location in locations:
    if(locations.count(location) > 1):
        print(location)
from LocationList import *
from os import path
from Utils import read_json

world_dir = 'data\\World'
files = ['Overworld.json', 'Deku Tree.json', 'Dodongos Cavern.json', 'Jabu Jabus Belly.json', 'Forest Temple.json', 'Fire Temple.json', 'Water Temple.json', 'Shadow Temple.json', 'Spirit Temple.json', 'Ganons Castle.json', 'Bottom of the Well.json', 'Gerudo Training Ground.json', 'Ice Cavern.json']
mq_files = ['Deku Tree MQ.json', 'Dodongos Cavern MQ.json', 'Jabu Jabus Belly MQ.json', 'Forest Temple MQ.json', 'Fire Temple MQ.json', 'Water Temple MQ.json', 'Shadow Temple MQ.json', 'Spirit Temple MQ.json', 'Ganons Castle MQ.json', 'Bottom of the Well MQ.json', 'Gerudo Training Ground MQ.json', 'Ice Cavern MQ.json']
i = 0


new_check_file = open('freestanding_html', 'w')

locations = []
count = 0
new_check_file.write("***Freestanding Rupees/Recovery Hearts***\n")
for file in files:
    file_path = path.join(path.curdir, world_dir, file)
    data = read_json(file_path)
    for region in data:
        if region.get("locations"):
            i = 1
            printed_region_name = False
            for location in region["locations"]:
                locations.append(location)
                if location_table.get(location):
                    if location_table.get(location)[5]:
                        if 'ActorOverride' in location_table.get(location)[0] or 'Freestanding' in location_table.get(location)[5] or 'RupeeTower' in location_table.get(location)[5] or 'Beehive' in location_table.get(location)[5]:
                            if not printed_region_name:
                                print(region.get("region_name"))
                                new_check_file.write("== " + region.get("region_name") + " ==\n")
                                new_check_file.write('{| border="0" cellpadding="1" cellspacing="1" width=100%\n !width="40%"|Location!!width="30%"|Image!!style="text-align:center"|Logic Notes\n|-\n')
                                printed_region_name = True
                            print(str(i) + ": " + location + ": " + region["locations"].get(location))
                            new_check_file.write("|| " + location + " || IMAGE_URL_HERE ||style=\"text-align:center\"|" + region["locations"].get(location) + "\n|-\n")
                            i = i + 1
                            count += 1
            if printed_region_name:
                new_check_file.write("|}\n")
new_check_file.write("Total Count: " + str(count)+ "\n")

count = 0
new_check_file.write("***Pots/Crates***\n")
for file in files:
    file_path = path.join(path.curdir, world_dir, file)

    data = read_json(file_path)
    for region in data:
        if region.get("locations"):
            i = 1
            printed_region_name = False
            for location in region["locations"]:
                locations.append(location)
                if location_table.get(location):
                    if location_table.get(location)[5]:
                        if 'Pot' in location_table.get(location)[5] or 'Crate' in location_table.get(location)[5] or 'FlyingPot' in location_table.get(location)[5] or 'SmallCrate' in location_table.get(location)[5]:
                            if not printed_region_name:
                                print(region.get("region_name"))
                                new_check_file.write("== " + region.get("region_name") + " ==\n")
                                new_check_file.write('{| border="0" cellpadding="1" cellspacing="1" width=100%\n !width="40%"|Location!!width="30%"|Image!!style="text-align:center"|Logic Notes\n|-\n')
                                printed_region_name = True
                            print(str(i) + ": " + location + ": " + region["locations"].get(location))
                            new_check_file.write("|| " + location + " || IMAGE_URL_HERE ||style=\"text-align:center\"|" + region["locations"].get(location) + "\n|-\n")
                            i = i + 1
                            count += 1
            if printed_region_name:
                new_check_file.write("|}\n")
new_check_file.write("Total Count: " + str(count)+ "\n")

count = 0
print ("*** MQ ***")
for file in mq_files:
    file_path = path.join(path.curdir, world_dir, file)

    data = read_json(file_path)
    for region in data:
        if region.get("locations"):
            i = 1
            printed_region_name = False
            for location in region["locations"]:
                locations.append(location)
                if location_table.get(location):
                    if location_table.get(location)[5]:
                        if 'Freestanding' in location_table.get(location)[5] or 'Pot' in location_table.get(location)[5] or 'Crate' in location_table.get(location)[5] or 'FlyingPot' in location_table.get(location)[5] or 'SmallCrate' in location_table.get(location)[5]:
                            if not printed_region_name:
                                print(region.get("region_name"))
                                new_check_file.write("== " + region.get("region_name") + " ==\n")
                                new_check_file.write('{| border="0" cellpadding="1" cellspacing="1" width=100%\n !width="40%"|Location!!width="30%"|Image!!style="text-align:center"|Logic Notes\n|-\n')
                                printed_region_name = True
                            print(str(i) + ": " + location + ": " + region["locations"].get(location))
                            new_check_file.write("|| " + location + " || IMAGE_URL_HERE ||style=\"text-align:center\"|" + region["locations"].get(location) + "\n|-\n")
                            i = i + 1
                            count += 1
            if printed_region_name:
                new_check_file.write("|}\n")
new_check_file.write("Total Count: " + str(count)+ "\n")
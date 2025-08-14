# OoTRandomizer

This is a randomizer for _The Legend of Zelda: Ocarina of Time_ for the Nintendo 64.

WARNING: This branch is a modified version of the randomizer. It is not officially supported and may be very unstable. Please refrain from asking questions and from reporting issues in the main Randomizer Discord when using this branch. Instead, you can open an issue on this fork here or contact me directly on discord (RealRob) for any help, report or request.

This branch is based of of Roman971's fork of the randomizer and usually kept up to date with the latest changes in the main Dev fork, with the addition of the following developmental features:

Enemy Drop Shuffle - All (almost) of the enemies in the game will give a shuffled item when killed.
Wonderitem Shuffle - Shuffles wonderitems - which include invisible rupees and invisible hit markers.
Gossip Stone Shuffle - Play Song of Time to a gossip stone to receive a shuffled item.
Enemy Soul Shuffle - Shuffles enemy "souls" into the item pool. Enemies will not spawn into the world until their souls have been collected.
Grass Shuffle - Shuffles grass
Fishing Game Shuffle - Shuffle the fish from the fishing game. The fish in the pond become items that you will collect upon catching. 
    The fish themselves are shuffled as items that must be collected throughout the world. .
    Finding a 10 lb (child) 16 lb (adult) and a hylian loach may be turned in to the pond owner for a reward.
    The fishing rod becomes an item shuffled into the world which must be found before you can fish in the pond.

## Index

* [Installation](#installation)
* [General Description](#general-description)
  * [Getting Stuck](#getting-stuck)
  * [Settings](#settings)
  * [Known Issues](#known-issues)
* [Changelog](#changelog)

# Enemy Shuffle Changelog
### 8.3.17.Rob-E3
  * Merge Dev-Rob to fix ice trap crash

### 8.3.17.Rob-E2
  * Fix DC Hallway crash in enemizer

### 8.3.17.Rob-E1
  * Merge main dev
  * Fix GTG boulder room door opening on room clear

### 8.3.12.Rob-E8
  * Add deku tree hint scrubs and fire temple floor tile to shuffled enemy locations. 
  * Add white wolfos and large deku babas to enemy pool

### 8.3.12.Rob-E7
  * Fix enemy plando

### 8.3.12.Rob-E6
  * Don't commit Debug mode

### 8.3.12.Rob-E5
  * Forgot GTG

### 8.3.12.Rob-E4
  * Preliminary MQ Enemizer logic

### 8.3.12.Rob-E3
  * Fix Deku Theater misc. hints in enemizer

### 8.3.12.Rob-E2
  * Fix tailpasaran 0,0,0 bug.

### v160
  * Fix ganons tower access missing in enemizer

### v159
  * Disallow iron knuckles in DC lizalfos room

### v158
  * Fix actor glitch crashes

### v157
  * Fix Jabu Shabom Room patch being applied to MQ

### v156
  * Fix DMT summit boulder shuffle logic in enemizer

### v155
  * Improve enemizer logic processing
  * Add location specific logic for enemies in water temple boulder room
  * Improve skullwalltula behavior in enemizer :)

### v154
  * Fix generation error w/ boulder types in a list

### v153
  * Fix enemies shuffled onto Jabu pit room Baris

### v152
  * Fix Twinrova Iron Knuckle Logic

### v151
  * Make some iron knuckles not start moving immediately on spawn
  * Improve logic for Shadow Trial
  * Fix DMC bubble location specific logic for freezard

### v150
  * Fix spawning/reduce enemy restrictions in some locations

### v149
  * Fix generation error with no adult trade items selected

### v148
  * Fix redead infinite suns song loop when dying/unstunned while ToD changes after playing suns song

### v147
  * Fix Deku Tree baba logic

### v146
  * Golden Boulder setting

### v145
  * Fix Forest Temple Upper Stalfos spawning with soul shuffle + enemy shuffle
  * Add Child KF and Deku Tree Enemies

### v144
  * Fix tailpasaran spawning underwater.
  * Fix BOTW center room wallmaster.
  * Fix Shadow temple huge pit bubble.
  * Fix Fire temple stair case keese.
  * Add green indicator to soul menu for current room's present and obtained souls.

### v143
  * Fix some Jabu enemizer logic

### v142
  * Correct spelling of Dinolfos throughout codebase.
  * Fix water temple like like enemizer logic. 
  * Add location specific enemy logic system and add logic for DMC bubbles

### v141
  * Various enemizer logic and enemy restriction fixes

### v140
  * "Temporary" address bump to fix audio issues

### v139
  * Fix raycast check

### v138
  * Update to main dev 8.2.37
  * Additional enemy restrictions for some water-based locations
  * Allow weapons in adult market in enemizer
  * Add Blue Tektites
  * Remove flare dancer BGM

### v137
  * Fix crash when burning deku shield

### v136
  * Fix skull kid needles so that they don't get culled

### v135
  * Fix skull kid spawns

### v134
  * Remove position rounding on actor spawns - Should fix chu clips

### v133
  * Add missing enemy logic

### v132
  * Fix room clear logic

### v131
  * Fix missing anubis logic

### v130
  * Fix spear moblin soul

### v129
  * Fix Anubis not included in enemy table
  * Add Spear Moblins
  * Make Hammer Moblins spawn better, rotate towards player
  * Fix some enemies in MQ Child Spirit
  * Fix redead/gibdo spawn params
  * Restrict water stingers to water locations
  * Add filter functions to dynamic enemizer/is_enemy checks
  * Fix MQ Jabu falling like likes

### v128
  * Realign .rodata section to hopefully fix N64 console crashes

### v127
  * Prevent flare dancers from falling out of bounds all the time

### v126
  * Add separate drop logic to enemy list

### v125
  * Skip raycast down for certain locations

### v124
  * Make deku babas always attack

### v123
  * Force enemies to spawn on ground.
  * Reenable Deku Babas
  * Add outdoor requirement for flying peahats
  * Add enemy probability weights
  * Fix Dodongo's Cavern lizalfos room doors

### v122
  * Fix to prevent Bari from always falling through the floor

## RealRob Changelog
### 8.3.30.Rob-1
  * Update to latest main Dev

### 8.3.17.Rob-2
  * Fix a crash when opening an ice trap chest in a room that has ice traps in vanilla and quickly exiting the room.

### 8.3.17.Rob-1
  * Merge main dev

### 8.3.12.Rob-2
  * Fix Deku Tree MQ Upper Deku Baba logic

### 8.3.12.Rob-1
  * Update to latest main dev
  * Reset supplementary version to 1 every time we merge main dev from now on.

### v138
  * Add Deku Tree Compass Room Withered Deku Baba

### v137
  * Add missing deku baba enemy drops in BOTW, Deku MQ, DC MQ

### v136
  * Fix BGS/Giants Knife/DD advancement

### v135
  * Fix DMT Summit boulder logic

### v134
  * Fix missing Jabu Jabus Belly MQ Cow

### v133
  * Fix Forest Temple MQ Stalfos enemy drops

### v132
  * Fix Ganons Castle MQ Logic Error causing occassional generation error

### v131
  * Resolve compiler warnings.

### v130
  * Fix GC Maze Gossip Stone in wrong region... again

### v129
  * Fix generation error w/ no adult trade items shuffled

### v128
  * Fix generation w/ MQ

### v127
  * Add golden boulder option to boulder shuffle

### v126
  * Fix anubis not displaying on minimap tracker
  * Add Child KF Deku Babas and Deku Tree/MQ Deku Tree deku babas

### v125
  * Fix logic issue in MQ Deku Tree

### v124
  * Increase size of markers on enemy minimap tracker
  * Rename Like-like to Like Like across the code

### v123
  * Enemy drops souls logic review fixes
  * Rebuilt entire branch

### v122
  * Fix BOTW silver rupee softlock

### v121
  * Fix boomerang/bombchu trail corruption

### v120
  * Fix duplicate items on GUI
  * See you next year No Deku Nut November

### v119
  * Add key ring colors

### v118
  * Fix Shadow Temple MQ Spike Wall Skulltula logic
  * Fix GTG/MQ ceiling silver rupee wallmaster logic

### v117
  * Fix fix broken drops crash when changing rooms with deku shield in child spirit.

### v116
  * Stop breaking dark link

### v115
  * Add missing Jabu Jabu's Belly MQ Falling Like Like Room wonderitems

### v114
  * Fix Water Temple MQ After Dark Link Hookshot Wonderitem logic
  * Fire Temple MQ Hammer Steps Stalfos logic

### v113
  * Scrubs misc hint setting broken when disabled

### v111
  * Fix Custom Song Name display breaking when using the enemy drop minimap tracker
  * Update to main Dev 8.1.81

### v110
  * Various logic fixes

### v109
  * Fix Jabu Jabus Belly Invisible Enemies Room logic.

### v108
  * Fix MQ Spirit Child keese logic with fix_broken_actors
  * Fix MQ Spirit Big Mirror enemy logic.
  * Fix MQ Shadow Invisible Blades SoT blocks trick logic

### v107
  * Fix DC Upper Lizalfos Region soul logic

### v106
  * Fix ZF hidden cave logic

### v105
  * Merge main Dev
  * Fix duplicate region "Deku Tree Basement Rotating Spike Room"

### v104
  * Fix missing fish again

### v103
  * Merge the rest of Potsanity 3.0. Probably just breaking things
  * Fix GC Gossip Stone logic for boulder shuffle
  * Fix Deku Baba Sticks/Nut soul shuffle logic

### v102
  * (Hopefully) Fix generating boss soul shuffle with random settings
  * Ganons Castle/MQ enemy logic fixes
  * Water Temple enemy logic fixes

### v101
  * Reverting Dev merge

### v100
  * Fix crash in MQ DC hallway before lower lizalfos with boulder shuffle

### v99
  * Temp removing adult bunny hood and dampe all night

### v98
  * Fix crash when pressing dpad down on equipment screen

### v97
  * Fix KZ crash - wasn't my fault this time :)

### v96
  * Add dampe all night setting

### v95
  * Fix masks rendering when in first person view

### v94
  * Bunny hood dpad fixes

### v93
  * Update to main Dev 8.1.13
  * Add Adult Bunny Hood setting

### v92
  * Fix King Dobonko

### v91
  * Hopefully fix website custom music patching

### v90
  * Fix broken MQ Deku logic files

### v89
  * Boulder Shuffle and Soul Shuffle fixes

### v88
  * Fix a typo

### v87
  * Various Boulder Shuffle logic fixes

### v86
  * Fix disabled enemy drop locations giving shuffled items and appearing on minimap tracker

### v85
  * Fix King Zora boulder flag in boulder shuffle

### v84
  * Fix dark link in soul shuffle... again

### v83
  * Fix MQ DC failing to generate
  * Fix soul shuffle logic for Spirit Temple MQ Central Chamber Wallmaster

### v82
  * New Enemy Soul Shuffle option: Regional
  * Add MQ support for Boulder Shuffle
  * Fix Boulder Shuffle not obeying "All Locations Reacable"
  * Fix Fishing Shuffle progressive items drawing incorrect model after caught

### v81
  * Fishing LH Adult Fish 1 missing

### v80
  * Unfix the KZ unfreeze glitch

### v79
  * Fix boulder resizing hacks
  * Properly resize Water Trial red ice boulders
  * Some boulder shuffle logic fixes

### v78
  * Spirit Temple MQ Soul Shuffle Logic fixes

### v77
  * Fix Hylian loach fishing shuffle logic
  * Fix MQ DC Soul Shuffle Logic

### v76
  * Add King Zora red ice to boulder shuffle

### v75
  * Don't break the red ice walls...

### v74
  * Properly resize boulders that replace red ice walls
### v73
  * Fix sandstorm transitions occassionally crashing

### v72
  * Don't delete all of the boulders

### v71
  * Fix burning kak crash in boulder shuffle

### v70
  * Enemy drops/souls logic fixes:
    * MQ Forest Falling Room
    * Forest Temple Block Push Room Upper Bubbles
    * Forest Temple First Twisted Hallway Wallmaster
    * Spirit Temple MQ Child Stalfos Fight Pot
    * Spirit Temple MQ Central Chamber Flying Pots
    * MQ Water Dragon Statue Wonderitems
  * Add missing Deku Tree MQ Basement Larva Room Deku Scrub drop
  * Fix accidentally removed MQ Water wonderitems
### v69
  * Boulder shuffle logic fixes

### v68
  * Fix broken Great Fairy Fountains

### v67
  * New setting: Shuffle Boulders
  * New setting: Fix Broken Actors (replaces Fix Broken Drops)

### v66
  * Add Bunny Hood preset

### v65
  * Hopefully actually fix small key text boxes in fishing shuffle

### v64
  * Smaller fish in fishing game shuffle are no longer considered major items

### v63
  * Fix certain textboxes not displaying correctly when catching fish in fishing shuffle
  * Fix unshuffled Spirit Temple MQ Wonderitems

### v62
  * Fix fishing shuffle owner rewards not being given if you mash the text boxes

### v61
  * Update to latest upstream 8.0.10
  * New setting: Shuffle Fishing Game Fish
  * Fix missing Wallmaster drop in MQ Light Trial

### v60
  * Fix MQ Spirit Child room not clearing in enemy soul shuffle

### v59
  * Fix Small Crates in MQ Fire Temple not shuffled

### v58
  * Fix missing Deku Tree Rotating Spike Skulltula

### v57
  * Fix missing grass in Lake Hylia

### v56
  * Fix incorrect overrides spawning in soul shuffle

### v55
  * Fix anubis spawns in enemy drop shuffle

### v54
  * Update multiworld coop context version #

### v53
  * Add souls to plentiful and ludicrous item pools
  * Fix enemy minimap markers not displaying properly on some platforms.

### v52
  * Testing something that will hopeful improve seed generation

### v51
  * Add grass shuffle texture match contents
  * Add MQ dungeons to grass shuffle
  * Soul Logic fixes
    * DMC Bubble Below Bridge To Fire Temple
    * Deku Tree Basement Back Room Near GS Skulltula
  * Fix Spirit Temple MQ Child Stalfos Fight Pot 4 not shuffled

### v50
  * New setting - Grass Shuffle ;P
  * Add support for MMR custom music

### v49
  * Fix Temple of Time Gossip Stone Drops

### v48
  * Fix Forest Temple Upper Stalfos Drops
  * Fix Dark Link drop
  * Fix Skull Kids drops
  * Fix Spirit Temple Child Bridge Bubble logic

### v47
  * Fix Forest Trial enemy logic

### v46
  * Potsanity Changes:
    * Add new settings to shuffle empty pots/crates
    * All fairy pots are now shuffled
    * Update the animation when collecting a major item to spin above the player's head
    * Lots of stuff under the hood
  * Enemy drop shuffle tracker
    * Adds a setting to enable a minimap tracker that displays the enemies that haven't been checked. Enemies display as red dots on the map.
  * Soul Shuffle Logic improvements

### v45
  * Various soul shuffle logic fixes

### v44
  * Fix Shadow Temple GS Invisible Blades Room soul logic
  * Fix Spirit Temple Child Bridge Bubble soul logic

### v43
  * Fix HF Cow Grotto Skulltula soul logic

### v42
  * Fix Forest Temple Right Courtyard Balcony Skullwalltula

### v41
  * Add flame effect to stick/nut upgrade so they are distinguishable

### v40
  * Enemy soul shuffle logic fixes:
    * Forest Temple Map Chest
    * GTG Beamos Chest
    * GTG Stalfos Chest

### v39
  * Add MQ enemy soul shuffle logic

### v38
  * Fix souls and silver rupee save data not initializing properly on some platforms

### v37
  * Fix enemy souls not defaulting to enabled with only Bosses soul shuffle or starting with souls
  * Fix soul menu not displaying correctly on Parallel emu core.

### v36
  * Add enemy soul shuffle menu

### v35
  * Fix dark link room not clearing in soul shuffle

### v34
  * Enemy Soul Shuffle Fixes:
    * Fix logic for BOTW Dead Hand Chest
    * Fix text for Dark Link soul incorrectly saying Ganondorf Soul

### v33
  * Forest temple soul logic fixes

### v32
  * Fix ganondorf not spawning in enemy soul shuffle

### v31
  * Fix missing souls from enemy drop logic in Ganons Castle

### v30
  * Add Dark Link to enemy soul shuffle

### v29
  * Add "Bosses" option to enemy soul shuffle

### v28
  * Fix broken flags in enemy soul shuffle

### v27
  * Add new setting 'Enemy Soul Shuffle'
  * Enemy Drop logic improvements

### v26
  * Improve Fire Bubble logic in Enemy Drop Shuffle

### v25
  * Rebased onto main Dev version 7.1.139

### v24
  * Enemy Drop Shuffle Fixes:
    * Fix logic for Ganons Castle MQ Water Trial Keese

### v23
  * Rebased onto main Dev version 7.1.118
  * Enemy Drop Shuffle Fixes:
    * Fix broken logic in Jabu
    * Fix "Prevent Guays From Respawning" hack having weird invisible guay hitboxes on the ground.

### v22
  * New Cosmetic Option - Rainbow Tunic. Might also fix the weirdshot crash

### v21
  * New Setting - Individual Ocarina Note Shuffle
  * Enemy Drop Shuffle Fixes:
    * Fix broken Lost Woods Skullkid drop

### v20
  * New Ice Trap Settings:
    * Custom Ice Trap Count
      * Allows specifiying a number of junk items to be replaced w/ Ice Traps
    * Custom Ice Trap Percent
      * Allows specifying a percentage of junk items to be replaced w/ Ice Traps
  * Gossip Stone Shuffle Fixes:
    * Add MQ Dodongo's Cavern Gossip Stone

### v19
  * Enemy Drop Shuffle Fixes:
    * Fix MQ GTG Heavy Block Room enemy logic

### v18
  * New Setting - Key Appearance Matches Dungeon
    * When enabled, small keys, key rings, and boss keys will 
    be colored to match their respective dungeons.

### v17
  * Silver Rupee Shuffle Fixes:
    * Prevent items shuffled in silver rupee locations from being collected with boomerang
    * Fix "TEXT ID ERROR" message when collecting the final silver rupee from a chest or shop

### v16
  * New Setting: Gossip Stone Shuffle
  * Enemy Drop Shuffle Changes:
    * Add skull kid drops
  
### v15

  * Add Fast Armos Pushing

### v14

  * Silver Rupee Shuffle:
    * Added Silver Rupee Pouches
    * When silver rupees are collected, the text box will indicate how many rupees of that puzzle have been collected
  * Enemy Drop Shuffle:
    * Added setting under Misc. to prevent guays from respawning
  * Bug Fixes:
    * Fix bug where freestanding items would duplicate when reloading a room


## Installation

It is strongly suggested users use the web generator from here:

<https://ootrandomizer.com/>

If you wish to run the script raw, clone this repository and either run ```Gui.py``` for a
graphical interface or ```OoTRandomizer.py``` for the command line version. They both require Python 3.8+.
To use the GUI, [NodeJS](https://nodejs.org/download/release/v20.17.0/) (v20 LTS, with npm) will additionally need to be installed. NodeJS v14.14.0 and earlier are no longer supported.
The first time ```Gui.py``` is run it will need to install necessary components, which could take a few minutes. Subsequent instances will run much quicker.
Supported output formats are .z64 (N64/Emulator), .wad (Wii VC, channel IDs NICE/NRKE recommended), Uncompressed ROM (for developmental purposes, offline build only)
and .zpf/.zpfz (patch files, for sharing seeds with others).

This randomizer requires The Legend of Zelda: Ocarina of Time version ```1.0 NTSC-US```. This randomizer includes an in-built decompressor, but if
the user wishes a pre-decompressed ROM may be supplied as input. Please be sure your input ROM filename is either a .n64 or .z64 file. For users
playing via any means other than on real N64 hardware, the use of the "Compress patched ROM" flag is strongly encouraged as uncompressed ROMs are
impossible to inject for the Virtual Console and have random crashing problems on all emulators.

For general use, there are four recommended emulators: [Project64 (v3.0+)](https://wiki.ootrandomizer.com/index.php?title=Project64), [Bizhawk](https://wiki.ootrandomizer.com/index.php?title=Bizhawk), [RetroArch](https://wiki.ootrandomizer.com/index.php?title=Retroarch) and [Dolphin (latest beta)](https://wiki.ootrandomizer.com/index.php?title=Dolphin). All are race-legal when configured appropriately.
In a nutshell the differences are:
* Project64 is the lightest emulator and the easiest to setup, however, you will need the 3.0.0 version or later to run OoTR well (and earlier versions are not permitted for use in OoTR races).
* Bizhawk is the most resource-intensive, but easier to set up than RetroArch and the only race-legal emulator to support [Multiworld](https://wiki.ootrandomizer.com/index.php?title=Multiworld).
* RetroArch is less resource-intensive than Bizhawk and the only of the three N64 emulators to work on platforms other than Windows, but it can be frustrating to set up.
* Dolphin lets you emulate Wii VC, giving you access to faster pauses. It's also lightweight, available on other platforms than Windows, easy to setup (on Windows at least) and offers good native support with most Gamecube Controller Adapters. It does come with more lag, although that can be mitigated through overclocking (this is not permitted for racing).

Please follow [the guides on our wiki](https://wiki.ootrandomizer.com/index.php?title=Setup#Emulators) carefully to ensure a stable game experience and that
[the settings requirements for races](https://wiki.ootrandomizer.com/index.php?title=Racing#Emulator_Settings_Requirements) are met. OoTR can also be run on
an N64 using an [EverDrive](https://wiki.ootrandomizer.com/index.php?title=Everdrive), or on [Wii Virtual Console](https://wiki.ootrandomizer.com/index.php?title=Wii_Virtual_Console). For questions and tech support we kindly refer you to our [Discord](https://discord.gg/q6m6kzK).

## General Description

This program takes _The Legend of Zelda: Ocarina of Time_ and randomizes the locations of the items for a new, more dynamic play experience.
Proper logic is used to ensure every seed is possible to complete without the use of glitches and will be safe from the possibility of softlocks with any possible usage of keys in dungeons.

The randomizer will ensure a glitchless path through the seed will exist, but the randomizer will not prevent the use of glitches for those players who enjoy that sort of thing, though we offer no guarantees that all glitches will have identical behavior to the original game.
Glitchless can still mean that clever or unintuitive strategies may be required involving the use of things like Hover Boots, the Hookshot, or other items that may not have been as important in the original game.

Each major dungeon will earn you a random Spiritual Stone or Medallion once completed.
The particular dungeons where these can be found, as well as other relevant dungeon information can be viewed in the pause menu by holding the D-Pad buttons on the C-Item Menu.
Note, however, that the unlock conditions for dungeon information are settings-dependent.

As a service to the player in this very long game, many cutscenes have been greatly shortened or removed, and text is as often as possible either omitted or sped up. It is likely that someone somewhere will miss the owl's interjections; to that person, I'm sorry I guess?

### Getting Stuck

With a game the size of _Ocarina of Time_, it's quite easy for new Randomizer players to get stuck in certain situations with no apparent path to progressing. 
Before reporting an issue, please make sure to check out [our Logic wiki page](https://wiki.ootrandomizer.com/index.php?title=Logic). 
We also have many community members who can help out in our [Discord](https://discord.gg/8nmX7fa).

### Settings

The OoT Randomizer offers many different settings to customize your play experience.
A comprehensive list can be found [here](https://wiki.ootrandomizer.com/index.php?title=Readme).

#### Plandomizer

"Plan"-domizer is a feature that gives some additional control over the seed generation using a separate distribution file. In such a file you can:
* Place items at specific locations or restrict items from being placed at specific locations.
* Add or remove items from the item pool.
* Select items to start with.
* Set specific dungeons to be vanilla vs Master Quest.
* Set which trials are required.
* Set any regular settings.

Caveat: Plandomizer settings will override most settings in the main OoTR generator settings, particularly list-based settings like enabled tricks or starting inventory. For example, if the Plandomizer distribution file contains an empty list of starting items, and the generator settings include additional starting equipment, the player will start with none of them instead. You will have to edit the Plandomizer file to change such settings, or **delete** completely the line in the Plandomizer file with the given setting to allow the main generator to alter the setting.

See [the Plandomizer wiki page](https://wiki.ootrandomizer.com/index.php?title=Plandomizer) for full details.

#### Custom Models

Save the .ZOBJ file of the desired model in `data/Models/Adult` or `data/Models/Child`. The file must be in .ZOBJ format (the compressed .PAK files are not compatible), but most Modloader64 models will work. Exceptions are models which are larger than the base Link models (the randomizer will give an error message) and those created on the new pipeline (technically load but the textures get wonky). Please see notes regarding known model files that are floating around [in this spreadsheet](https://docs.google.com/spreadsheets/d/1xbJnYw8lGR_qAkpvOQXlzvSUobWdX6phTm1SRbXy4TQ/edit#gid=1223582726) before asking why a model doesn't work.

Once the models are saved, the program may be opened and the model(s) selected under the `Cosmetics` tab.

If the model's skeleton is similar enough to Link, the randomizer will use Link's skeleton. If it is substantially different, then a note will be placed on the pause screen to make it clear that the skeleton was changed.

### Known issues

Unfortunately, a few known issues exist. These will hopefully be addressed in future versions.

* The fishing minigame sometimes refuses to allow you to catch fish when playing specifically on Bizhawk. Save and Hard Reset (NOT savestate) and return to fix the
issue. You should always Hard Reset to avoid this issue entirely.
* Versions older than 3.0 of Project64 have known compatablity issues with OoTR. To avoid this either 
[update to v3.0 and follow the rest of our Project64 guide](https://wiki.ootrandomizer.com/index.php?title=Project64) or change to one of our other two supported emulators.
* Executing the collection delay glitch on various NPCs may have unpredictable and undesirable consequences.
* This randomizer is based on the 1.0 version of _Ocarina of Time_, so some of its specific bugs remain.

## Changelog

The changelog has moved to [CHANGELOG.md](CHANGELOG.md).

For an overview of settings additions by release, see [the wiki](https://wiki.ootrandomizer.com/index.php?title=Readme#Generation_Options).

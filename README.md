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

* [RealRob's Changelog](#realrob-changelog)
  * [v67](#v67)
  * [v66](#v66)
  * [v65](#v65)
  * [v64](#v64)
  * [v63](#v63)
  * [v62](#v62)
  * [v61](#v61)
  * [v60](#v60)
  * [v59](#v59)
  * [v58](#v58)
  * [v57](#v57)
  * [v56](#v56)
  * [v55](#v55)
  * [v54](#v54)
  * [v53](#v53)
  * [v52](#v52)
  * [v51](#v51)
  * [v50](#v50)
  * [v49](#v49)
  * [v48](#v48)
  * [v47](#v47)
  * [v46](#v46)
  * [v45](#v45)
  * [v44](#v44)
  * [v43](#v43)
  * [v42](#v42)
  * [v41](#v41)
  * [v40](#v40)
  * [v39](#v39)
  * [v38](#v38)
  * [v37](#v37)
  * [v36](#v36)
  * [v35](#v35)
  * [v34](#v34)
  * [v33](#v33)
  * [v32](#v32)
  * [v31](#v31)
  * [v30](#v30)
  * [v29](#v29)
  * [v28](#v28)
  * [v27](#v27)
  * [v26](#v26)
  * [v25](#v25)
  * [v24](#v24)
  * [v23](#v23)
  * [v22](#v22)
  * [v21](#v21)
  * [v20](#v20)
  * [v19](#v19)
  * [v18](#v18)
  * [v17](#v17)
  * [v16](#v16)
  * [v15](#v15)
  * [v14](#v14)

* [Installation](#installation)
* [General Description](#general-description)
  * [Getting Stuck](#getting-stuck)
  * [Settings](#settings)
  * [Known Issues](#known-issues)
* [Changelog](#changelog)
  * [8.0](#80)
  * [7.1](#71)
  * [7.0](#70)
  * [6.2](#62)
  * [6.1](#61)
  * [6.0](#60)
  * [5.2](#52)
  * [5.1](#51)
  * [5.0](#50)
  * [4.0](#40)

## RealRob Changelog
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

To run this program, clone this repository and either run ```Gui.py``` for a graphical interface or ```OoTRandomizer.py``` for the command line version. They both require Python 3.8+.
To use the GUI, [NodeJS](https://nodejs.org/download/release/v18.12.1/) (v18 LTS, with npm) will additionally need to be installed. NodeJS v14.14.0 and earlier are no longer supported.
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

### Dev

#### New Features
* **Settings**
  * `Minor Items in Big/Gold chests` has been converted into a multiselect so you may granularly make bombchus, shields, or stick/nut capacity appear in big chests.
  * New shuffle `Shuffle Wonderitems` which allows shuffling items the game refers to as Wonderitems. These items are obtained through a few ways: invisible items which drop when Link touches them (such as the rupees above the Hyrule Castle Town drawbridge), interactable switches (such as the torches on Hyrule Castle which drop items when shot with the slingshot), free multitag which gives an item when a certain set of points are touched (the stepping stones in Kokiri Forest), and ordered multitag where a set of points must be touched in a particular order (the grass stepping stones in Kokiri Forest). Wonderitems will indicate their prescence with sparkles color-coded to the type of wonderitem.
* **Hints**
  * The `Clearer Hints` option now provides clearer hints for the rainbow bridge text on the altar in the Temple of Time.
  * New option in hint distribution `combine_trial_hints` which combines multiple trials hints into one.

### 8.0

#### New Features
* **Settings**
  * New setting `Key Rings give Boss Keys` makes it so when picking up a key ring for a certain dungeon, you will also get the boss key for that dungeon, if applicable.
  * New ER setting `Shuffle Gerudo Valley River Exit` allows you to shuffle the one-way exit going down the river in Gerudo Valley.
  * New setting `Add Bombchu Bag and Drops` which, along with the previous changes of `Bombchus in Logic`, makes the first pack of Bombchus you find into a bag which then allows you to purchase bombchus from shops as well as find them hidden in grass or rocks like regular bombs.
  * New multiselects `Shuffled Child Trade Sequence Items` and `Adult Trade Sequence Items` along with toggle `Shuffle All Adult Trade Items` which have reworked the trade quests for both child and adult. Now you can own multiple trade items for each age at the same time and have multiple trade item locations as shufflable checks.
    * The Scrub grotto Mask of Truth check will no longer be an always hint if the mask of truth is shuffled.
  * Many new SFX shuffle options have been added.
  * New `Rainbow` tunic options.
  * New music option `Speed Up Music For Last Triforce Piece` speeds up the music when you only have 1 Triforce Piece left to find, to drive home the intensity.
  * New music option `Slow Down Music When Low HP` slows down the music when you're in critical health, to give the feeling of being near death.
  * New shuffle setting `Shuffle Hyrule Loach Reward` which shuffles the reward you get upon presenting the Hyrule Loach to the fishing hole owner. You can choose to retain the Loach's vanilla behavior or make it easier to catch in several ways.
  * New shuffle setting `Treasure Chest Game Keys` makes it so the keys from the Treasure Chest Game can be found anywhere in the world rather than just in the game itself, and the chests of the minigame can contain any item. It also makes the shopkeeper sell a random item.
  * New World setting `Shuffle Thieves' Hideout Entrances`, which requires first shuffling interior entrances, shuffles the Thieves' Hideout rooms into the interior entrance pool.
  * New shuffle setting `Shuffle Silver Rupees` shuffles the silver rupees into the item pool. To solve a silver rupee puzzle, you will need to find all the associated silver rupees around the world. You can also use the setting `Silver Rupee Pouches` to make some or all of the silver rupees grouped into pouches so that, similar to key rings, all silver rupees for any given puzzle will be found in a single item location.
  * New timesaver setting `Ruto Already at F1` which, in Vanilla Jabu Jabu, makes it so that Ruto is already present on the first floor of the dungeon (in the room with all the orifices you fall down) instead of having to carry her up there to set that flag.
  * New setting `Key Appearance Matches Dungeon` which changes the models for small keys and boss keys to indicate which dungeon the key belongs to.
  * New setting `Shuffle Individual Ocarina Notes`, which shuffles items representing each of the 5 ocarina notes into the item pool. You cannot play any note on the ocarina until you find its corresponding item.
  * New cosmetic setting `Uninvert Y-Axis in First Person Camera` which makes tilting the joystick up or down in first person look those respective directions, instead of the opposite direction.
  * The setting `Display D-Pad HUD` has been expanded so that the D-Pad can be set to the left of the screen.
* **Hints**
  * New `Important Checks` hint type which hints at how many major items are in a given region.
    * Double Defense and Biggoron's Sword are counted as major for this hint type.
  * New Misc. Hint `Frogs Ocarina Game` which tells you what item you'll get from the Zora's River frog minigame. The hint appears in the dialog box that's printed before they jump onto the log.
  * Add a goal category for the items required to open the Door of Time, if it's closed.
  * New `MW Season 3 (WotH)` hint distribution which is similar to the `MW Season 3` hint distribution except with Way of the Hero hints instead of goal hints.
  * New Misc. Hint for unique merchants: Bean Salesman, Medigoron, Wasteland Bombchu Salesman, Granny's Potion Shop.
  * Owls are now included in the Warp Songs misc hint and will tell you where they go when enabled.
  * New options for hint distributions, both of which are lists of gossip stones defined inside the entry for a hint type:
    * `remove_stones`: When defined, each gossip stone in the list will be excluded from receiving this hint type. For example, if `ToT (Left)` is inserted into the `remove_stones` list inside the dictionary for `always` hints, then no `always` hint will be placed on `ToT (Left)`.
    * `priority_stones`: When defined, gossip stones in this list will be given priority when placing the specified hint type, in the order they're defined. For example, if `ToT (Left)` and `ToT (Right)` are inserted into the `priority_stones` list, in that order, inside the dictionary for `always` hints, the first `always` hint will be placed on `ToT (Left)` and the second `always` hint will be placed on `ToT (Right)`.
  * New Dual hint for King Zora checks: Unfreezing him and trading the prescription for an eyeball frog.
  * The credits music can now be shuffled into the music pool.
  * Many hints had clearer hints added and/or were reworded.
  * New `Chaos!!! (no goal hints)` hint distribution which is the same as the normal "Chaos" distribution, but without goal hints.
* **Other**
  * When picking up a small key, the text box will now inform you how many you've found total.
  * The longstanding vanilla bug where using Din's Fire on a white bubble crashes the game has been fixed.
  * There are new chest, pot, and crate textures for Pieces of Heart and Heart Containers.
  * Custom music has moved to a new format. See `data/Music/README.md` for more details.
  * New and improved model for key rings.
  * Several new tricks have been added.
  * Multiworld plugins now have the ability to show the proper progressive item based on the inventory state of the receiving player.

#### Bug Fixes
* **Misc.**
  * Fix an issue where the `path of hearts` goal wasn't enabled in certain circumstances with a Ganon's Boss Key or LACS `Hearts` goal.
  * Fix an issue where CSMC chests containing ice traps were not moved correctly.
  * Fix the bingosync URL not being referenced correctly.
  * The ocarina can no longer be pulled out mid-hookshot via the D-Pad to cancel the hookshot pull.
  * Fix the change which makes graveyard grave hole ledges un-grabbable incorrectly applied to other ledges in the graveyard area.
  * Fix the file path for custom music on some Linux machines.
  * Fix "Spirit Temple MQ Beamos Room Chest" having an incorrect vanilla item.
  * Starting with Magic Beans will now properly start you with 10 beans instead of 9. 
  * A rare softlock when fighting Gohma present in the original game has been fixed.
  * A crash when diving and resurfacing in very shallow water present in the original game has been fixed.
  * Various miscellaneous logic issues have been addressed.
  * A minor bug related to checking time of day access has been corrected.
  * Fix a softlock caused by 8-note Sun's Songs when using `Randomize Ocarina Song Notes`, again.
  * Fix pause screen rendering causing graphical issues on some platforms.
  * Fix a long-standing bug where certain fanfares kill the currently-playing backgroud music.
  * Fix a bug which would cause the Lost Woods bridge ocarina check, if ocarinas are unshuffled and overworld Entrance Randomizer is disabled, to give a Fairy Ocarina when it should give the Ocarina of Time.
  * Disallow excluding the bombchu bowling bombchu refill locations, since this didn't do anything.
* **Hints**
  * Fix the cryptic hint for Ganon's Castle Boss Key.
  * Fix missing punctuation in dual hints.
  * Fix a bug in Multiworld Goal hints causing the incorrect path to be generated for the hint.
  * Fix a bug which would cause hints to the "Link's Pocket" goal to be generated.

#### Other Changes
* `Closed Forest` is no longer changed to `Closed Deku` when `Shuffle Boss Entrances` is enabled.
* A new model is now used for warp songs to differentiate from non-warp songs.
* The model used for Ruto's Letter is now rotated onto its side to better differentiate from other bottles.
* The dummy boss key chest on the wall of Forest Temple's twisted hallway now matches the real version of the chest in the straightened version of the room.
* The Kakariko Well water will no longer be up as adult to facilitate a glitch strategy for entering the well.
* The message table has been extended to allow further developments which require more added textboxes.
* New timesaver setting `Maintain Mask Equips through Scene Changes` which makes it so the currently worn mask will now be preserved when transitioning scenes or resetting.
* The ocarina can now be used in some rooms it cannot normally be used in to allow warping out of them.
* Link the Goron will now give you the item reward regardless of which dialog option you choose.
* Models created using the newer ModLoder64 pipeline are now compatible with the randomizer.
* The price of the shield discount after selling the Kakariko guard the Keaton mask is now set at patch time and thus consistent between different players of a seed.
* Add binaries to support Windows on ARM64.
* Priority placement of certain warp songs is now conditional if `Guarantee Reachable Locations` is set to `All Goals` or `Required Only`.
* Improve seed generation time.
* The key ring texture has been updated with the normal small key texture, making it shinier.
* D-Pad mask icons will now enlarge when the mask is equipped, similarly to D-Pad boots.
* Fix a vanilla bug where jumpslashing into Bongo Bongo's chamber on the last possible frame will not properly skip the cutscene when it should be skipped.
* Heart Containers and Pieces of Heart will no longer be directly hinted by Way of the Hero or Path hints, which is consistent with Triforce Pieces and Gold Skulltula Tokens.
* Junk items being sent to another world will now float up into the air to indicate this.
* An unnecessary polygon check function is skipped to increase game performance.
* In Triforce Hunt, your current and goal number of triforce pieces are now displayed on the file select screen.
* Python 3.6 and 3.7 are no longer supported.
* Some inescapable entrances are no longer valid targets for Overworld Spawn entrances.
* Using Farore's Wind to warp between dungeons is now considered in logic.
* Various text changes such as singular Temple of Time Altar bridge conditions, grammar changes to region hints, and adding apostrophes to shop items.
* Updated fill error message to add suggestions for resolving the error.
* Updated some trick tool tips for grammar and clarity.

#### New Speedups
* Various cutscenes removed or shortened, such as Water Temple and Gerudo Fortress gates and scarecrow spawn cutscenes.
* Skulltulas will now pop out of their soil patches instantly (once the bugs enter the hole) instead of waiting 3 seconds.
* The cutscene of the Gerudo's Fortress carpenters running out when you free them will be skipped.
* The cutscene that plays when shooting the sun over Lake Hylia has been skipped.
* The spinning room in the Forest Temple basement now moves faster, and does not lock Link in place while spinning.
* Link is now powerful enough to push armos statues more quickly.

#### Plandomizer
* Plandomizer now allows you to specify locations that are valid but do not exist in your current seed, for example, an MQ-specific location when that dungeon is Vanilla.
* Error messages for conflicting settings have been improved.
* Gossip stone text colors are no longer specified in reverse order.
* Music groups will now add to each other if you have multiple groups of the same name, instead of the last one simply overwriting the others.
* The legacy starting items dictionary has been renamed to `starting_inventory` to make it more distinct from the new `starting_items` which is part of the Settings dictionary.
* The `#Vanilla` item category is now usable for dungeon rewards.

### 7.1

#### Bug Fixes
* Fix freestanding items not spawning their overrides when an item with a shared base collectable flag is collected.
* Fix models drawing incorrectly when picking up a duplicated collectible, such as from the Goron City spinning pot.
* Fix junk items from pot/freestanding item locations not being sent to the proper player in multiworld.
* Fix Skulltula House Misc. Hints not hinting the proper locations in multiworld.

#### GUI
* `Disable Battle Music` has been moved to the main section of the SFX tab.
* Nonsense tags in the `Exclude Locations` list were cleaned up.
* New tags were added in the `Exclude Locations` and `Enable Tricks` lists.
* Upgrade to latest Nebular version 10.
* Upgrade to Angular 14.
* Minimum NodeJS version is now 14.15.0.
* Fix visibility issues when using `Randomize Main Rule Settings`.

#### Other Changes
* Removed some unnecessary duplication in spirit temple logic.
* In multiworld, outgoing items are now buffered.
  * Allows players to continue collecting items if the Multiworld plugin crashes.
  * Multiple freestanding/pot items can be sent on the same frame.
* Ice Arrows will now be referred to as Blue Fire Arrows in hints and shop text when `Blue Fire Arrows` is enabled.
* Logic now considers the possibility to melt the red ice walls in Ice Cavern with Blue Fire Arrows as adult, then coming back as child and collecting a skulltula with the boomerang.
* Beehives now wiggle depending on the renamed setting `Pot, Crate, & Beehive Appearance Matches Contents`.
* `Item Model Colors Match Cosmetics` is now enabled by default.

#### Plandomizer
* Due to Ice Arrows and Blue Fire Arrows being separate items in the code now, plandomizer authors must use `Ice_Arrows` or `Blue_Fire_Arrows` depending on if the setting is enabled.
* In cosmetics plando, empty or non-existant BGM groups will no longer error, instead simply displaying a warning in the cosmetics log.

### 7.0

#### New Features

* **Settings**
  * New setting `Dungeon Shortcuts` opens shortcuts in blue warp dungeons to the boss room. This is toggleable per-dungeon, and affects glitchless logic.
  * New setting `Shopsanity Prices` adds additional price range options when Shopsanity is enabled, including "Affordable" which sets all prices to 10 rupees (to match the same option in Scrub Shuffle).
  * `Chest Size Matches Content` has been replaced with `Chest Appearance Matches Content`. Unique textures are applied to chests containing major items, small keys, boss keys, skulltula tokens, and remaining items. An additional option also changes chest sizes like the previous setting.
  * New cosmetic setting `Disable Battle Music` turns off the battle music from being near enemies, allowing the background music to continue uninterrupted.
  * New setting `Plant Magic Beans` automatically plants all the Magic Beans from the start.
  * New setting `Key Rings` which can be enabled per-dungeon to replace all of its individual Small Keys into a singular Small Key Ring containing all the small keys for that dungeon. Key Rings also have their own model.
  * Setting `Randomize Ocarina Song Notes` can now be set to either "row" of songs individually, i.e. "Frog Songs" or "Warp Songs", in additional to the "Off" and "All Songs" options.
  * MQ Dungeon settings have been replaced with `MQ Dungeons Mode` which allows finer selection of which dungeons are MQ, or how many are at random.
  * New setting `Shuffle Boss Entrances` allows boss rooms to be shuffled between dungeons. This is not available in glitched logic.
  * `Misc. Hints` has been expanded to a multiple select setting allowing you to fine-tune which set of misc hints to enable or disable.
  * New setting `Shuffle Frog Song Rupees` allows you to shuffle the rupees you receive from the Zora's River frogs.
    * These locations are considered "sometimes" hints.
  * New setting `Show Seed Info on File Screen` which also allows a user-set message to be displayed.
  * New settings allow for Rainbow Bridge and Ganon's Boss Key to be obtained upon reaching a certain amount of total heart containers.
  * New setting `Easier Fire Arrow Entry` allows you to set the amount of torches that must be lit to open Shadow Temple.
  * The pause screen info menu has been split into 3 menus, which show icons on the D-Pad indicating which direction leads to which menu. In addition, the menu now tracks the total keys you've found for a dungeon, not just how many you have remaining. The old menu from pressing A still exists as well.
    * You can also make it so these menus only appear when holding A along with the D-Pad by disabling `Display D-Pad Dungeon Info`.
  * New setting `Invisible Chests` makes all chests in the game invisible.
  * New setting `Bonks Do Damage` will deal damage to Link when bonking, including `One Bonk KO` which will instantly kill him from bonking.
  * New hint type `Dual Hint` which allows multiple locations to be hinted in the same hint. Hint distros can also use new option `upgrade_hints` to upgrade some hints to Dual Hints.
  * New `Chaos` hint distribution which gives all hint types equal likelihood.
  * New setting `Blue Fire Arrows` gives Ice Arrows the power to melt red ice and mud walls to give them more utility.
  * New Misc. Hint `Dampé's Diary` which reveals the location of a hookshot.
  * New item pool setting `Ludicrous` makes it so every check will be a major item.
  * `Shuffle Dungeon Entrances` has new setting `Dungeons and Ganon` which puts Ganon's Castle into the pool of dungeons which can be shuffled.
  * New setting `Pre-Completed Dungeons` which allows some dungeons to be filled with junk and their dungeon rewards given as a starting item.
  * New setting `Regional` for dungeon items makes it so dungeon items such as keys will be placed in the same region as their dungeon. For example, if Small Keys are set to Regional, then Small keys for Forest Temple can be found in the temple itself, Sacred Forest Meadow, Lost Woods, Kokiri Forest, or Deku Tree.
  * `Skip Child Zelda` and `Shuffle Weird Egg` have been combined into one setting `Shuffle Child Trade Item`.
  * The Adult earliest and latest trade item settings have been combined into a multiselect `Adult Trade Sequence Item` which allows you to choose whatever items you wish as the starting item for the adult trade quest.
  * New setting `Minor Items in Big/Gold chests` places shields and bombchus in major item chests when any `Chest Appearance Matches Contents` setting is enabled along with it.
  * New setting `Fix Broken Drops` adds a magic jar drop in GTG and a pot that drops a deku shield in Spirit Temple. These spawns were present in the code already but would not actually spawn due to the respective objects not being loaded.
  * New settings `Adult Link Model` and `Child Link Model` allow you to select a .zobj model file to replace Link's look in-game. For more details see the [Custom Models section](#Custom-Models)
  * New section under SFX labeled `Link` has options `Adult Voice` and `Child Voice`, allowing you to choose to either silence Link or change the voice to feminine to match your chosen player model. Adult and child female voices provided by Maple and shiroaeli in the Discord, respectively.
  * New Misc. Hints for each of the Skulltula rewards makes it so each of the cursed family members who give a randomized reward will tell you what the reward is before being rescued. This precludes those locations from being conditional always hints.
  * New setting `Shuffle Rupees & Hearts` which allows you to shuffle freestanding rupees and recovery hearts in the world.
  * New settings `Shuffle Pots` and `Shuffle Crates` which allows you to shuffle items from pots and crates that normally drop an item.
  * New setting `Pot & Crate Appearance Matches Contents` which changes the pot/crate texture when using one of the above settings, similar to `Chest Appearance Matches Contents`.
  * New setting `Shuffle Beehives` which allows beehives found in certain grottos to drop an item.

* **Gameplay**
  * Shortened the animation for equipping magic arrows.
  * You can now use the child trade item (Zelda's Letter, masks, etc) as child using D-Pad Right.
  * Red Ice transparency is increased when any Chest Appearance setting is enabled.
  * Zelda's text after defeating Ganon now advances automatically.
  * Health and Magic potion models in shops will now match your health and magic colors when `Item Model Colors Match Cosmetics` is enabled.
  * The Gerudo guard on the Wasteland side of the gate will now spawn regardless of settings.
  * Several vanilla warp destination messages have been updated to keep consistency with our custom warp destination messages.
  * Removed the cutscenes when throwing a bomb at and blowing up the boulder in front of Dodongo's Cavern.
  * Certain switches in MQ dungeons have been moved down 1 unit so they are less difficult for Link to walk onto.
  * The "Truth Spinner" puzzle in the Shadow Temple's solution is now seeded per seed, so that the correct skull will be consistent across players racing the same seed.
  
* **GUI**
  * Migration to latest Angular and Nebular framework versions
  * Dark Mode toggle
  * Support for modern Node.JS versions (>= Node.JS v16)
  * Compatibility with newer Linux builds and ARM based Macs
  * Improved mobile design in web mode

#### Bug fixes
* Return the color of the "OK" and "No" options of the warp song prompts to their correct colors in Warp Song Shuffle.
* Horseback Archery will no longer delay ending based on the fanfare.
* Prevent Dead Hand from spawning outside the room collision.
* Magic arrows equipped as adult no longer revert to the Bow after traveling through time to child and back.
* Rainbow Bridge set to vanilla no longer spawns without Light Arrows.
* Fix MQ Shadow Temple making use of shared flags.
* Fix MQ Fire Temple missing the Hammer chest from the map and minimap.
* Add correct default Goals for Ganon's Boss Key when Light Arrow Cutscene is non-vanilla.
* Fix a softlock caused by 8-note Sun's Songs when using `Randomize Ocarina Song Notes`.
* Fix slow text when acquiring certain items (Hookshot, Bombchus, etc.)
* Using Farore's Wind as both ages should no longer result in the wrong temporary flags being loaded.
* Fixed issues where `Skip Child Zelda` could give a weapon with no ammo or overwrite starting Triforce Pieces.
* Fixed an issue where `Plentiful` item pool combined with `Starting Hearts` would result in more Heart Containers being in the item pool than intended.
* The Like Like in Master Quest Fire Temple will now return the player's shield when defeated.
* Junk can now be placed on excluded song locations when Songs are set to `Song Locations` and a song is started with.
* Fix `GC Medigoron` not being a hintable location.
* Fix 1/16th damage not always killing Link when `One Hit KO` is enabled.
* Fix ice traps cloaked as major items not giving the slow chest opening cutscenes when `Fast Chest Cutscenes` is disabled.
* Tunics and shields can now spawn in any room when the game attempts to spawn them.
* Fix a crash when certain settings related to Light Arrow hints are combined.
* Many minor logic bugs have been corrected.
* Fix a softlock when plandoing starting with less than 10 beans.
* Fix plando negative locations (ex. "!Gold Skulltula Token") choosing Buy items for non-Buy locations.
* Ice traps will no longer be sent to players in the Treasure Chest Game to prevent using deaths to circumvent the game.
* Fixed dragging list items in the GUI not working when the target is empty.
* Recovery hearts in several scenes in MQ dungeon rooms have been fixed to no longer be invisible.

#### Other changes
* Added an auto-tracker context area to memory, so auto-trackers can find certain symbols much easier.
* Improve some error messages when running the generator.
* Fix logic parser shortcuts not working right in newer Python versions.
* `Goron Bracelet` is no longer greyed out as adult on the Equipment subscreen of the pause menu.
* Plandomizer
  * You can now specify an arbitrary 4-character hexadecimal text ID as a "Gossip Stone" to overwrite text in the game.
  * Adjusted how `starting_items` works in order to remove some redundancy, where it is now placed within the settings dictionary.. Spoiler output now includes a section `:skipped_locations` instead which is not used by Plandomizer.
  * Gold Skulltula Token requirements can be increased above 100 (the pool must also have at least that many).
  * Plandomizer will now display an error and inform the user if they have specified conflicting settings within the plando.
  * New plando item groups available: `#Map`, `#Compass`, `#BossKey`, and `#SmallKey` for placing a random one of these dungeon items in a specific location.
  * Cosmetics plando has added three new groups which songs can be placed in `bgm_groups`:
    * `favorites`: Songs placed in this group will be given priority when placing songs.
    * `exclude`: Songs placed in this group will be ignored when placing songs.
    * `groups`: In here you can place a dictionary where the key is a custom group name and the value is a list of sequences, and use `#GroupName` similar to item groups in normal plando to place any song from that group in that location.
* Triforce Hunt changes
  * The number of Triforce pieces available per world, which was previously tied to the item pool setting, is now a separate setting.
* Replaced old output option `compress_rom` with four separate options for outputting a patch file, compressed ROM, uncompressed ROM, and a WAD file.
* Patch files from the web version of OoT Randomizer version 3.0 can now be used in the offline build. A `.zpf` patch file will be output in addition to the selected output types.
* On the randomizer dungeon info screen, a Gold Skulltula icon appears for dungeons where you have collected all the Gold Skulltulas.
* Maximum heart containers is now capped at 20. Pieces of Heart and Heart Containers collected after 20 will still restore your health but will not add to your hearts.
* Many more "Junk" hints have been added.
* `Song from Impa` is no longer restricted to a smaller set of items when `Skip Child Zelda` is enabled.
* On the File Select screen, trade items that will be reverted on load now show as the item they will revert to.
* Ganondorf will now say that the Light Arrows are in your pocket if they are an effective starting item (e.g. the item given from `Skip Child Zelda`).
* The "WINNER!" Piece of Heart will now show the proper player name in Multiworld.
* Hinted item and location for a Gossip Stone hint are now included in the spoiler log.
* One-way entrances are now restricted to one per hint area rather than one per scene.
* You can now receive starting ice traps, either from Impa's item with Skip Child Zelda or with plando.
* Common ER error messages are now more helpful to solving the issue.
* Corrected some goal hint colors.
* Triforce Piece model has been updated so that it is shinier and centered.
* Goal hints are now selected based on their category rather than their parent goal.
* Settings and tricks in the spoiler log have been re-ordered to more closely follow the order in the GUI and use more logical groupings, respectively.

### 6.2

#### Bug fixes

* Fix seed generation for multiworld with random trials.
* Fix seed generation for All Goals Reachable.
* Fix a minor optimization for counting needed Skulltula Tokens.
* Fix some erroneous category tags for locations.

#### Other changes
* Allow foolish hints to apply even if an area has an Always hint (but no other types).
* Renamed setting `Enable Useful Cutscenes` to `Enable Specific Glitch-Useful Cutscenes` for clarity.

### 6.1

#### New Features

* **Objective Settings**
  * New setting `Reachable Locations` replaces `All Locations Reachable` and adds a new option "All Goals" which ensures that all goal items for Ganon's Boss Key and the Rainbow Bridge are reachable, regardless of how many are required by the setting.
    * For example, if the Rainbow Bridge requires 4 medallions, all 6 medallions will be guaranteed reachable.
  * New setting `LACS Condition` to select what goal items are required for the Light Arrows Cutscene, separate from whether Ganon's Boss Key is placed there. (Additional settings are added to control how many.)
  * `Ganon's Boss Key` now allows you to set it to the Light Arrow Cutscene (and use `LACS Condition` to set its objective) or for it to be directly awarded when meeting a configurable target number of Spiritual Stones, Medallions, Dungeons completed, or Gold Skulltula Tokens. (Additional settings are added to control how many.)
  * New `Rainbow Bridge` option `Random` that will choose one of the other options at random (besides Skulltula Tokens), and require the maximum of that goal (if applicable).

* **Hints**
  * New option `Misc. Hints` controls whether the Temple of Time altar and Ganondorf give hints, defaulting on to preserve behavior. Hell Mode disables this setting.
  * New `goal` hint type for use in custom hint distributions. 
    * Default goals are included for the Rainbow Bridge, Ganon's Castle Boss Key, and Ganon's Trials settings.
    * Hints read as "They say that Kokiri Forest is on the path to Twinrova.", where the medallion or stone reward from defeating Twinrova can be used for the bridge or Ganon's Castle Boss Key. Twinrova is not necessarily required depending on other settings. For example, with 2 medallions for the bridge, all medallions accessible without entering Ganon's Castle, and Spirit Medallion on Twinrova, the hint only points to one possible path to building the rainbow bridge.
  * Hint distributions can now filter areas from being hinted as foolish, via putting the area names in `remove_locations`.
  * Various other hint distribution changes for named-item hints, including a new `vague_named_items` option for `hint_dist_user` that will name the location but not the item.
  * The dungeon name is now provided when hinting keys.
  * Allow special characters (such as the é in Dampé) to appear in in-game hints.
  * Certain Unicode characters and control characters like 'Ⓐ' and '⯅' can now be used in hint texts to refer to the corresponding in-game symbols.

* **Plandomizer**
  * New `#MajorItem` item category for Plandomizer allows placing a random major item. This won't include songs unless Song Shuffle is Anywhere.
  * New `#Vanilla` item category for Plandomizer allows placing the item normally at the location.
  * New Plandomizer support for defining custom item groups.
  * New Plandomizer support for defining custom song note patterns (rhythm is still random).
  * Allow Plandomizer to override otherwise randomized settings.

* **Tricks**
  * Added `King Zora Skip` - Adult Link can make a precise jump to climb atop the fence next to King Zora from the audience chamber.
  * Removed `Forest Temple MQ Twisted Hallway Switch with Hookshot` - This was determined to be a clip and therefore not allowed in glitchless rulesets.

* **Misc.**
  * Allow Giant's Knife as a starting item.
  * Added a file `data/custom_music_exclusion.txt` where one can specify which music files shouldn't be shuffled in with random custom music.
  * Ruto now knows what kind of medallion or stone she gets in Jabu Jabu.
  * Randomized songs' note patterns are now included in the spoiler log.

#### Bug Fixes

* **Gameplay**
  * Adjust the weather system to fix the fog glitch.
  * Disable Sandstorm transitions in certain cases in Entrance Randomizer. In particular, this prevents Sandstorm transitions from triggering strobe effects due to a bug.
  * Remove the Door of Time collision while the door is opening.
  * Cache chest sizes and colors in the actor to reduce some latency with Chest Size Matches Contents.

* **Entrance Randomizer**
  * Smarter replacement of required warp songs when warp songs are shuffled. 
  * Fix hint area validation to work with shuffled warp songs.
  * Fix dungeon entrance hints.

* **Plandomizer**
  * Correctly consider plando'ed prices in logic, and limit set prices to a range of -32768 to 999.
  * Fix duplicating placed shop items.
  * Properly randomize starting age and time of day when plando sets it to random.

* **Hints**
  * Make hints in Triforce Hunt function based on the required number of Triforce Pieces and not the total number (improves Way of the Hero hints as well as compatibility with Guaranteed Reachable Locations set to Required Only).
  * Prevent areas hinted as barren from having a location hint.
  * Fix trying to place hints when we've run out of stones.

* **Misc. Seed Generation**
  * Fixed a bug where importing from a settings string might not choose the correct hint distribution depending on platform or number of custom hint distributions in the Hints folder.
  * `Skip Child Zelda` in Multiworld (with Song Shuffle: Anywhere) now correctly provides items to the right player.
  * `Skip Child Zelda` in Triforce Hunt (with Song Shuffle: Anywhere) now avoids starting having already won (by preventing Impa from giving out Triforce Pieces at all if there are more players than Triforce Pieces needed to win).
  * Fix some potential failures for placing junk.
  * Fix disabled song locations getting a general junk item instead of a junk song.
  * Fix some spoiler entries for visible ice traps.
  * Fix error thrown on some operating systems for capitalized file extensions .N64/.Z64.
<!-- * Show the actual result settings in the spoiler instead of originally chosen settings (in case some settings like Closed Forest were modified). The settings string remains the original so the seed can be easily reproduced. -->


#### Other Changes

* Added Nayru's Love back to the minimal item pool on high damage settings.
* Allow settings with a 'Random' option to be different per-world. (This does not permit settings randomized only by Randomize Main Rules to be different per-world.)
* Updated sometimes hints.
* Renamed some regions, locations, items, etc to match vanilla names. This will make Plandomizer files incompatible between versions.
  * Gerudo Training **Grounds** -> Gerudo Training **Ground**
  * Gerudo Fortress -> Thieves' Hideout (when referring to the interior areas or the carpenter rescue quest)
  * Graveyard Composers' Grave -> Royal Family's Tomb
* Giant's Knife and Biggoron's Sword are considered useless in glitchless logic but not in glitched.
* Entrance Randomizer: Prelude of Light and Serenade of Water are considered useless in ER if the only ER option enabled is Random Spawns (which already cannot be placed in certain areas where any items are necessary to escape).
* Entrance Randomizer: Allow Kakariko Potion Shop and Impa's House to have their entrances appear in different areas if all hints are off.
* Entrance Randomizer: When warp songs are shuffled, the confirmation text now shows the actual destination.
* Text Shuffle: Expanded the "Shuffled except Hints and Keys" to include some shop and scrub text, and renamed it to "Shuffled except Important Text".
* Hid the trick list on the GUI when it's disabled.
* Changed internal names for the broken swords.
* Added validation of settings for plandomizer before attempting generation.
* Removed size check for the decompressor.
* Updated presets.
* Updated logic.
* Added internal tracking of what items were where in vanilla OoT.
* Added ability to provide settings through stdin (useful for shell scripting).
* Added a commandline flag to use a preset by name. This still allows supplying additional settings, e.g. to control cosmetics and ROM output.
* Added settings validation for unittests.
* Added unittests for settings presets.
* Use HTTPS for the version check to github.


### 6.0

#### New Features

##### Gameplay
* New save file screen
  * Relevant items are shown before hitting 'Yes' to load, instead of just the hearts, magic, dungeon rewards, and deaths. Icons are solid if the save has the item or faded if not.
  * Triforce pieces aren't shown unless the save has at least 1.
  * The death counter is now placed at the bottom next to a skull.
* Updated altar text in the Temple of Time
  * Now provides rainbow bridge requirements and the shuffle mode of Ganon's Castle Boss Key (info otherwise available in the seed settings). These are always available at the altar regardless of Maps/Compass settings.
  * Removed misleading vanilla text from the child altar.
* Various Quality of Life improvements
  * Speed up boulder lifting
  * Speed up Gold Gauntlet boulder lifting
  * Speed up learning Windmill song 
  * Speed up learning Malon's song
  * Speed up Kakariko gate opening and closing
  * Twinrova waits for player to reach the top platform before starting, preventing early snipes

##### 'ROM Options' settings
* New Cosmetic Plandomizer
  * Use a JSON file to set your cosmetics and sound settings.
  * We've added lots of new color options to pick from as well!
* Custom settings presets (must be json) can be placed in `data/Presets` to be automatically loaded in the GUI.

##### New 'Main Rules' settings and options
* New setting `Kakariko Gate`
  * Allows configuring how the Kakariko Gate and the Happy Mask Shop will open.
  * Default (vanilla) behavior requires showing Zelda's Letter to the guard to open the gate and the shop.
  * You can configure the gate to be always open or to open automatically upon obtaining the Letter. Both of these options will also open the Happy Mask Shop upon obtaining the Letter.
* Entrance Randomizer settings overhaul
  * `Entrance Shuffle` setting replaced with the other independent settings here.
  * `Shuffle Interior Entrances`: allows a choice of shuffling simple interiors, all interiors, or none.
  * `Shuffle Grotto Entrances`: allows shuffling grotto/grave entrances.
  * `Shuffle Dungeon Entrances`: allows shuffling dungeon entrances.
  * `Shuffle Overworld Entrances`: allows shuffling overworld connections.
  * `Randomize Owl Drops`: allows randomizing where the owl drops you from each owl spot.
  * `Randomize Warp Song Destinations`: allows randomizing (to any entrance, not just warp pads!) where each warp song takes you.
  * `Randomize Overworld Spawns`: allows randomizing (per age) where you start when loading a save in the Overworld.
  * All these shuffles and randomizations are fixed when the seed is generated; within a seed it will always be the same each time.
* New options for `Shuffle Songs`
  * This now allows selecting from three options: shuffling in **song** locations (previously 'off'), shuffling in **anywhere** (previously 'on'), and shuffling in **dungeon** reward locations (new).
  * The dungeon reward locations are: the 9 boss heart containers, the Lens of Truth chest (BotW), the Ice Arrows chest (GTG), the song reward in Ice Cavern, and the song from Impa in Hyrule Castle.
  * In multiworld, as before, only the "anywhere" setting will allow songs to be placed in other players' worlds.
* New setting `Shuffle Medigoron & Carpet Salesman`
  * Adds the Giant's Knife and a pack of Bombchus to the pool while Medigoron and Carpet Salesman each sell a randomly placed item once for 200 rupees.
* New options for Key and Map/Compass Shuffle settings
  * "Overworld Only" will place keys (or maps/compasses) outside of dungeons.
  * "Any Dungeon" will allow keys (or maps/compasses) to placed in any dungeon, not just the dungeon they belong to!
  * "Dungeon Only" is renamed "Own Dungeon" for clarity.
  * Gerudo Fortress Small Keys are configured in a separate setting.
* New options for `Rainbow Bridge` and `Ganon's Boss Key on Light Arrows Cut Scene`.
  * Sliders allow customizing the exact number of stones/medallions/dungeons/tokens required.
  * Ganon's BK on LACS can now be set to require Gold Skulltula Tokens.
  * `Randomize Main Rules` won't randomize slider values.
  * Conditional-always hints check for whether 2 or more dungeon rewards are required, as a backstop.

##### 'Other' settings
* New setting `Skip Child Zelda`
  * Skips the Hyrule Castle visit as child, returning Malon and Talon to Lon Lon Ranch and granting Zelda's Letter and the song that Impa provides at the start of the game.
  * Depending on the `Kakariko Gate` and `Complete Mask Quest` settings, may also start with the gate and shop open and masks available.
  * Removes the Weird Egg (and prevents `Shuffle Weird Egg`).
* New setting `Skip Some Minigame Phases`
  * Allows getting both rewards for Horseback Archery and Dampé Race in a single go!
  * Replaces the `Skip First Dampé Race` setting.
* New setting `Complete Mask Quest`
  * Marks all the mask sales complete so that the shop has all masks available to borrow as soon as it opens.
* New setting `Fast Bunny Hood`
  * Allows manual toggling on/off of the 1.5x speed boost from MM.
* New "Hint Distribution" customization options
  * Old hardcoded hint distributions are now defined by json files in `data/Hints`.
  * Custom hint distributions can be added to this folder, or defined directly in Plando files.
  * Many locations that did not previously have item hints now have hints, in case a custom hint distribution makes use of them.
  * Using the hint distribution "Bingo" allows setting a "Bingosync URL" to build hints for the specific OoTR Bingo board. Otherwise it's a generic hint distribution for OoTR Bingo.
* Hint distributions can configure groups of stones to all have the same hint, and can also disable stones from receiving useful hints (give them junk hints instead).
* Tournament hint distribution changes
  * Grotto stones are disabled and only provide junk hints.
  * Zelda's Lullaby is never considered for Way of the Hero hints.
  * Only "always", "Barren", and "WotH" hints have duplicates now.
  * "Barren" hints will typically be split evenly between dungeon and overworld areas.
  * Number of unique hints of each type are now (not counting seed-dependent hint types like 'always' and 'trial'): 4 WotH, 2 barren, 5(remainder) sometimes.
  * The previous Tournament hint distribution has been renamed "Scrubs Tournament".
* New setting `Hero Mode`
  * Allows playing without heart drops from enemies or objects. Good luck!!

##### Cosmetics/SFX
* New cosmetic settings for HUD button colors
  * These can all be set independently, defaulting to the N64 colors.
* New cosmetic setting `Item Model Colors Match Cosmetics`
  * Freestanding models like heart containers, gauntlets, and heart/magic drops will match their respective color settings.
  * Tunics are not affected, in order to keep freestanding tunics recognizable.
* Navi Colors section renamed "Misc. Colors"
  * Navi and sword trails options are now in this section, along with:
  * New "Rainbow" option in all Navi color settings.
  * New Boomerang trail inner & outer color settings, including a "Rainbow" option.
  * New Bombchu trail inner & outer color settings, including a "Rainbow" option.
  * New Mirror Shield Frame color setting.
* Added options to `Background Music` and `Fanfares` for randomly selecting only from [custom music](https://wiki.ootrandomizer.com/index.php?title=Readme#Custom_Music_and_Fanfares).
  
#### Updated Settings/Tricks
* `Lens of Truth` setting has been removed and replaced with several independent tricks.
  * `Lensless Wasteland`: assumes you can navigate the Wasteland to the Colossus without the Lens of Truth.
  * `<Area> without Lens of Truth`: assumes you can complete the given area without the Lens of Truth. Note that MQ and Vanilla dungeons have separate tricks.
  * Shadow Temples are split into two separate areas for these tricks.
  * Glitchless logic now requires Lens (or an appropriate trick) for some checks, particularly in Shadow Temple.
  * Glitched logic may sometimes assume you can do something without lens regardless of trick settings.
* New tricks
  * `Dodongo's Cavern Vines GS from Below with Longshot` - use the Longshot to avoid the staircase.
  * `Forest Temple First Room GS with Difficult-to-Use Weapons` - use a sword or Deku Sticks to jumpslash, or Bombs as child.
  * `Spirit Temple Main Room Jump from Hands to Upper Ledges` - make a precise jump without Hookshot or Hover Boots.
  * `Water Temple Falling Platform Room GS with Boomerang` - use the Boomerang from the very edge of the platform.
  * `Death Mountain Trail Climb with Hover Boots` - get past the boulders without destroying them.
  * `Zora's Domain GS with No Additional Items` - use only a jumpslash.
  * `Ice Cavern Block Room GS with Hover Boots` - reach the GS with the Hover Boots to jumpslash it.
  * `Hyrule Castle Storms Grotto GS with Just Boomerang` - make a precise throw with the Boomerang to send it behind the wall.
  * `Water Temple Central Pillar GS with Farore's Wind` - cast inside the pillar before raising the water level. *Previously assumed in logic!*
  * `Water Temple Central Pillar GS with Iron Boots` - unlock the door on the middle level before raising the water.
  * `Water Temple Dragon Statue Switch from Above the Water as Adult` - trigger the switch from dry land, then use Iron Boots, any Scale, or a jump dive coming from the river.
  * `Water Temple Dragon Statue Switch from Above the Water as Child` - same but for child. The Scale dive is very precise.
  * `Goron City Grotto with Hookshot While Taking Damage` - brave the heat, but be quick.
  * `Dodongo's Cavern Two Scrub Room with Strength` - position a block correctly and adult can bring a bomb flower to the wall.
  * `Shadow Temple Falling Spikes GS with Hover Boots` - make a precise move to get on the falling spikes, then another precise move to grab the token.
  * `Deku Tree MQ Roll Under the Spiked Log` - roll at the right time to shrink your hintbox. *Previously assumed in logic!*
  * `Bottom of the Well MQ Jump Over the Pits` - Use a sidehop or backflip to jump over the pits. *Previously assumed in logic!*
  * `Water Temple MQ Central Pillar with Fire Arrows` - Angled torches have hard-to-hit hitboxes. *Previously assumed in logic!*
  * `Forest Temple MQ Twisted Hallway Switch with Jump Slash` - Hit the switch from above with a jump slash, after getting in place with Hover Boots or some glass blocks. *Previously assumed in logic!*
  * `Fire Temple MQ Lower to Upper Lizalfos Maze with Hover Boots` - Hover Boots can get you up from a crate.
  * `Fire Temple MQ Lower to Upper Lizalfos Maze with Precise Jump` - You can even jump up from a crate without the Hover Boots!
  * `Fire Temple MQ Above Flame Wall Maze GS from Below with Longshot` - Point the Longshot at the right pointin the ceiling to obtain the token.
  * `Shadow Temple MQ Invisible Blades Silver Rupees without Song of Time` - Get a boost from a Like Like into a silver rupee, but don't die in the process.
  * `Deku Tree MQ Compass Room GS Boulders with Just Hammer` - Jump slash from the top of the vines.
  * `Spirit Temple MQ Sun Block Room as Child without Song of Time` - Throw a crate onto the switch to unbar the door briefly.
  * `Water Temple MQ North Basement GS without Small Key` - There's an invisible Hookshot target you can use.
  * `Death Mountain Trail Lower Red Rock GS with Hover Boots` - Kill the Skulltula, get on the fence, and then backflip onto the the rock.
  * `Ice Cavern MQ Red Ice GS without Song of Time` - Side-hop into the right place and you have a brief amount of time to use Blue Fire.
  * `Kakariko Rooftop GS with Hover Boots` - Some tricky movements with the Hover Boots can get you up onto Impa's House.
  * `Dodongo's Cavern MQ Light the Eyes with Strength` - You have to move very quickly to light the eyes with a Bomb Flower.
  * `Dodongo's Cavern MQ Back Areas as Child without Explosives` - Use pots, Armos, etc to progress through the room. Not relevant without "Light the Eyes with Strength" above, which is much harder for child.
  * `Fire Trial MQ with Hookshot` - Hit the target from a precise position with precise aim.
* Removed tricks
  * `Water Temple Boss Key Chest with Iron Boots`
  * `Water Temple Dragon Statue with Bombchu` - superseded by the new Dragon Statue tricks.
  * `Bottom of the Well Like Like GS without Boomerang` - the Like Like can be permanently killed, so this isn't logically valid.
* Changed Tricks
  * Burning the two vertical webs in the Deku Tree basement with bow is now default logic. The relevant trick has been renamed to `Deku Tree Basement Web to Gohma with Bow` to reflect that it now only applies to the web immediately before Gohma.
  * `Reach Forest Temple MQ Twisted Hallway Switch with Hookshot` - renamed `Forest Temple MQ Twisted Hallway Switch with Hookshot`.
  * `Fire Temple MQ Boulder Maze Side Room without Box` - renamed `Fire Temple MQ Lizalfos Maze Side Room without Box`.
  * `Fire Temple MQ Big Lava Room Blocked Door without Hookshot` - can be done without damage, so it's now allowed in OHKO.
  * `Forest Temple Scarecrow Route` - renamed `Forest Temple East Courtyard Door Frame with Hover Boots` and can be done in Vanilla or MQ.
* Tricks can be filtered in the GUI using a new dropdown.
* Easy and Hell Mode presets have been updated to add in the new features and/or tricks as relevant!

#### Bug Fixes
* Stealing Epona no longer crashes with the Fast Epona Race setting when you have Epona's Song but no ocarina.
* Bunny Hood speed bonus now applies correctly in cases other than child running at full speed.
* Avoid crashing on some systems when using child items as adult.
* Ensure Ice Traps have valid models if they can be seen.
* Limit Kokiri Tunic RGB values in Glitched Logic to prevent Weirdshot crashes.
* Prevent Gerudo guards from throwing child Link in jail.
* Fix hints not being readable on Mask of Truth setting.
* Prevent Collection Delay from the Carpenter Boss when mashing through the text with an item in hand.
* Gray note songs do not play back when learning them, adding consistency and preventing Sun's Song from causing bugs.
  With this set of changes to song learning, Sun's Song can now be played right after you learn it and it will function properly.
* Empty Bomb fix improved to work in all scenarios.
* Fast warp song hack now sets transition to white fade for consistency.
* Royal Family Tomb moves out of the way instantly.
* Fix Zelda from being frozen at the start of the final battle.
* Fix (hopefully) any remaining issues fishing on BizHawk.
* Drop Ruto before entering Big Octo room if the miniboss has been defeated.
* Prevent an errant `@` from showing up in Triforce Hunt.
* Move the Stone of Agony indicator above any small keys if both are present.
* Fix model/icon colors in `Item Model Colors Match Cosmetics` not returning to default with a cosmetic patch setting them to defaults.
* Ensure Ganondorf always hints one of the first reachable Light Arrows.
* Don't require that child can reach Ganondorf in order for Light Arrows not to be hinted WotH.
* Allow playthrough to collect a second 'Bottle with Letter' as its first empty bottle.
* Fix some issues with `Randomize Main Rules`:
  * Closed Forest implies starting as child.
  * Triforce Hunt won't accidentally place the Boss Key for Ganon's Castle.
  * Other conflicts are now prevented.
* Fix a rare issue in ER with using time-passing regions to gain new access.
* Fix a rare issue where settings strings weren't allocated enough bits.
* Fix the version number in the ROM header being potentially wrong after patching.
* Fix the CRC for uncompressed ROMs.
* The seed generator can now retry a few times in case of failure.
* Exclude a line from text shuffle so the Malon race is completable.
* Minor plandomizer fixes and improvements.
* Various logic fixes.

#### Other Changes
* Most locations and a few items have been renamed to improve spoiler output and standardize.
  * This will break settings and distribution files from previous versions that reference these locations and items.
* Reordered locations more naturally in the locations part of the spoiler.
* Default for `Shuffle Weird Egg` is now off.
* In-game hints overhaul.
* File 3 has been removed from generated ROMs to free up some space.
* The Zora Sapphire in Jabu Jabu's Big Octo room is now the actual dungeon reward.
* The number of Triforces available in Triforce Hunt now rounds to the nearest whole number instead of the nearest whole **even** number.
* "No Logic" seeds can now disable locations.
* Cosmetic logs contain the display names of SFX instead of their internal key names.
* Performance improvements to seed generation.
* Generator GUI updated to use node-sass 4.14.1.
* Updated development n64 compilation process to use latest available toolchain.
* Changed some C code to support GCC 10 in development n64 compilation.
* Added decompressor source and updated Decompress binaries.
* OoTRandomizer.py returns an error code on failure to interact better with user scripting.
* Plandomizer distribution files are copied to the Output directory next to the Spoiler and Cosmetics logs.
* Mweep.

### 5.2

#### New Features
* Triforce Hunt
  * Collect some number of Triforce Pieces to beat the game instead of beating Ganon.
  * Multiworld Triforce counts are collective, so once the total is reached across all players everyone wins.
  * If enabled via randomizing main rules, the count is always 20.
* Separate Double Defense model
  * Now appears as a color-shifted version of the Heart Container, with a transparent interior and prominent gold border.
* Visual Stone of Agony indicator
  * When the player has the Stone of Agony, it will appear on-screen above the rupee count when the player is near a hidden grotto.
  * The icon vibrates based on proximity to the grotto entrance, similar to the rumble pak.
  * A real rumble pak is not required.
* Starting Inventory
  * A new tab in the GUI allows setting initial inventory, without having to create a Plandomizer file.
  * Items are divided into sections in the GUI based on category.
  * Trade quest items, Gerudo Membership Card, Scarecrow Song not included.
    * To start with the Gerudo Membership Card, set `Gerudo Fortress` to `Open Fortress` and disable `Shuffle Gerudo Card` ('Main Rules' tab).
    * To start with the Scarecrow Song, enable `Free Scarecrow's Song` ('Other' tab).

#### Updated Settings 
* Open Zora Fountain now has an open only adult option.
* Added a new setting `Ice Trap Appearance` to select whether ice traps appear as major items (the default), junk items, or anything. This appearance can affect chest size with `Chest Size Matches Contents` enabled.
* Removed settings `Start with Fast Travel`, `Start with Tycoon's Wallet`, `Start with Deku Equipment`.
  * These have been replaced with settings in the "Starting Inventory" tab.
* New settings `Start with Consumables` (enable to start with max Sticks, Nuts, and ammo), `Starting Hearts` (changes starting max health).
* New list settings `Starting Equipment` (swords, shields, strength, etc.), `Starting Items` (c-items), `Starting Songs` (songs).
* Logic now requires Stone of Agony to access any hidden grotto.
  * A new trick `Hidden Grottos without Stone of Agony` will bypass this.
  * Stone of Agony is now only considered a useless item (for barren areas) when this trick is on and Gossip Stones do not use it.
* Added a new trick `Goron City Spinning Pot PoH with Strength`, which allows stopping the Spinning Pot using a bomb flower.
* Hell Mode preset includes both the above tricks.
* Tricks enabled/disabled in a Plandomizer file now take precedence over Tricks in 'Detailed Logic', even if the Plandomizer file has an empty list.
  * An empty list means the seed will be beatable without any tricks.
  * If there's no `allowed_tricks` item in the file, the Detailed Logic tricks apply instead.
  * If there is an `allowed_tricks` list in the file, it will not be possible to disable any of the enabled tricks (or enabling more) without editing the file.

#### Other Changes
* Cosmetic heart color setting now applies in the file select screen.
* Cosmetic tunic color setting now applies to the icons in the pause menu.
* Non-Always Location hints cannot be placed for an area that already has a Foolish hint.
  * If the location hint is placed first, then it can still appear in a foolish hinted area, however in Tournament hint distribution the Foolish hints are placed first so that cannot happen.
* The location containing Light Arrows will be considered a hinted location if Ganondorf's hint can be reached without them.
* Ganondorf no longer hints at his Boss Key chest contents, except when Light Arrows don't exist (only possible in Triforce Hunt).
* Improved Entrance Randomizer hints.
* Updated Compressor. The GUI progress bar is now granular. If for some reason, the rom won't fit into 32MB, then the compressor will increase the output size.
* Revised some settings tooltips.
* Refactored Logic once again. It now uses helper json rules and rules can reference other rules.
* Disabled settings don't show up in the spoiler.
* Plando will now accept JSON lists for `item` in the location dictionary to randomly choose between for placement.
  * Attempts to not exceed item pool values until all the pool counts for the items in the list are reached.
* Plando locations are matched without regard to case.
* "Start with" settings are now handled by the Plando library.
* Further seed generation speed improvements.
* The main search algorithm was renamed Search (from Playthrough) to avoid confusion with the spoiler playthrough.
* General code cleanup and typo fixes.
* Added more Plando unittests.

#### Bug Fixes
* Minor stability fix in Plando.
* Spoilers for plando'd seeds now correctly show the tricks enabled for the seed.
* Plando no longer occasionally attempts to place an item on a location where it's not allowed.
* Plando starting items and items set in specific locations now count toward the pool allocation. (Starting items are replaced with junk.)
* Plando now refuses to place more than the maximum amount of bottles, adult trade items, shop items, or total non-junk items.
* Plando no longer places Ice Traps as Junk if `Ice Traps` is set to 'off'.
* Other various Plando bug fixes.
* Starting items for adult that auto-equip should do so correctly now. (Non-Kokiri Tunics won't autoequip at the moment.)
* Fixed two chests in MQ Shadow Temple that had swapped names in plando and spoilers.
* Removed (unnecessarily) duplicated/overlapping hints.
* Hints that should come in multiples (duplicates) no longer come in singletons in certain corner cases.
* Randomizing main rules now works correctly.
* Removed a misleading random "trials" value from the non-randomized settings in the spoiler.
* Fix seed values with spaces no longer working.
* Removed a mispasted option description from Gauntlets colors tooltips.
* Major armips fix should prevent some crashes in Dev builds. (Devs: required armips version >= 0.10.0-68-g8669ffd)
* Miscellaneous logic fixes.
* Other bug fixes.

### 5.1

#### New Features
* `Skip First Dampé Race` 
  * Allows getting both rewards in one race if the 60 second target is cleared
* Rupee Icon Color changes based on your current wallet upgrade

#### Updated Settings 
* Improve `Ear Safe` to be less painful
* `Tokensanity: Overworld Only` 
  * Shuffles Gold Skultulla Tokens in the overworld to compliment `Dungeons Only`
* Configurable Skulltula target for the Bridge Requirement
* `Randomize Main Rule Settings` still allows setting the `MQ Dungeon Count`
* `Always Guaranteed Hints` are now determined conditionally based on settings
* `Default Presets` are updated to better reflect first time beginner settings
  * The previous `Beginner Preset` is renamed to `Easy Preset`

#### Bug Fixes
* Improve stability of music related features
* Fix "...???" textboxes at the entrance of Great Fairies
* In the unlikely event `Tournament Hints` runs out of hints, the remaining hints are filled with more "Sometimes Good" hints. If those run out as well then it will fill with "Random Locations" hints.
* The `Gerudo Valley Crate PoH as Adult with Hover Boots` trick now properly takes OHKO into account.
* Minor GUI tweaks
* Improve error feedback in GUI and Rules JSON


### 5.0

#### New Features
* New Electron GUI
  * New GUI now utilizes both Python and Node to bring you an even better interface
  * Now requires Node (with NPM), in addition to the Python requirement
* Glitched Logic
  * New Logic Rules option that takes movement glitches into consideration
  * Check out the Wiki for more information
* Entrance Randomizer
  * Randomize entrances/loading zones
  * Entrances are connected bidirectionally, and only shuffled with other entrances of the same pool
  * Ability to randomize entrances (loading zones) among multiple pools:
    * `Dungeons Only`: All Dungeons except Ganon's Castle
    * `Simple Indoors`: Dungeons; as well as Houses, Great Fairies, all Open and Hidden Grottos (including small Fairy Fountains and the Lost Woods Stage), and Graves.
    * `All Indoors`: Dungeons and Simple Indoors, as well as Link’s House, the Temple of Time, the Windmill, and Dampé’s Grave.
    * `All Indoors & Overworld Entrances`: Almost all loading zones connecting overworld areas, including Owls
  * Deku Tree, Fire Temple, and Bottom of the Well dungeon entrances are accessible as both ages
* Starting Age Option
  * Can now start as child, adult, or random
* Plan-domizer
  * Create a custom seed by placing items, hints and/or entrances where you want them
  * Customize starting items, item pools, active trials and Master Quest dungeons
  * Plandomizer files match the spoiler log JSON format
* Additional Customization
  * Additional Background Music Sequences can now be provided to be shuffled in
  * Fanfares randomization
  * Customizable Heart, Magic Meter, and Gauntlet colors
  * Separate inner and outer Navi colors
* Added `Randomize Main Rules` option
* Cow Sanity
  * Playing Epona's Song for a cow for the first time gives a randomized item.
* Shuffle Magic Beans
  * A pack of 10 beans is shuffled into the pool and the Bean Salesman sells a random item once for 60 rupees.
* Cucco Count
  * The number of cuccos to be gathered for Anju can be reduced or randomized, and Anju will tell you in-game the target number to catch (similar to 10 Big Poes).
* Enable Useful Cutscenes prevents some useful cutscenes from being skipped
  * Re-enables Poes at Forest Temple and Darunia at Fire Temple

#### Major Changes
* Seeds generation is significantly faster
* Major refactor of logic for performance and ER
* Spoiler log is now in JSON format
* Log files are produced in `Logs` during generation to record any errors.
* Major Logic Changes
  * Desert Colossus Hands are now logically part of Spirit Temple
  * Added the ability to enter drain the Lake Hylia water as Adult after beating Morpha using a new Gossip Stone near the Serenade Warp Pad. Entering Water Temple with Gold Scale and Longshot is now always in logic, however no locations are accessible without additional tricks, Keysanity/Keysy, or Iron Boots.
  * Disabled Locations will always hold Junk items (except song locations if songs are not shuffled with items)
* Gameplay Changes
  * Mechanically, Hot Rodder Goron no longer checks for Bomb Bag
  * Wearing Bunny Hood increases running speed to match backwalking speed
  * All Gerudo now check for Gerudo Membership Card instead of Carpenters being freed
    * This only affects when `Shuffle Gerudo Card` is enabled or in Entrance Randomizer
    * In the affected modes, a Gerudo is added behind the Wasteland gate who can open the gate
  * Removed RNG from Fishing Minigame
    * Note: The optimal strategy is to have the line stationary for the fish to bite
  * Can now cast Farore's Wind and play Warp Songs from within Gerudo Training Ground and all of Ganon's Castle
* Hint Changes
  * Every generic grotto gossip stone has their own hint.
  * The "Very Strong" hint setting can now give multiple Foolish dungeon hints.
  * The “Tournament” hint setting was revised to utilize all 40 hint stones.
    * Increased to 5 WOTH hints (with a new maximum of 2 Dungeon regions hinted); increased to 3 Foolish hints; Skull Mask added to Tournament hints’ Always hints; 5 Sometimes hints; all hints in this distribution are duplicated onto two Gossip Stones.
* Cutscene Changes
  * Burning Kakariko Cutscene can be triggered when entering Kakariko Village from any entrance.
  * Speedup Owl Flying cutscenes to be almost instant.
  * Enable Useful Cutscenes setting added (see above in New Features)

#### Updated Settings 
* Filter added to `Location Exclusion` dropdown
* More tricks added to the `Enable Tricks` dropdown
* Shuffle Gerudo Card can now be enabled alongside Open Gerudo Fortress.
* Forest Options
  * `Closed Deku`: Open Forest except Mido still blocks the Deku Tree
* Dungeon Item Options
  * Added `Vanilla` placement option for small keys, boss keys and maps/compasses
* Ganon's Boss Key 
  * Split Ganon’s Boss Key settings from the rest of the Boss Keys setting
  * Added LACS options that place the key there.
    * This allows playing with open bridge while still requiring dungeon completion
  * This replaces the Remove Ganon’s Boss Door Lock option
* Plentiful Item Pool
  * Duplicate Ruto's Letter added to plentiful item pool
* With `Start With Max Rupees` option enabled, wallet upgrades items now fill to max rupees

#### Bug Fixes
* No longer able to buy Bombchus with only bomb bag when Bombchus in logic
* Dampé freestanding Piece of Heart no longer requires opening the chest
* Buying Piece of Heart/Heart Container fully heals Link
* Learning Sun's Song from Malon no longer causes a softlock
* Castle and Gerudo guards can no longer cause softlock when catching you
* Vanilla shop items have correct price in spoiler log with shopsanity enabled
* Fixed Song of Storms not being usable in Sacred Forest Meadow immediately after learning it
* Improved Bottled Fairy logic rules for OHKO in ER
* Fixed `Starting Time of Day` times to better reflect their descriptions with the in-game state
  * `Night` options will spawn Skulltulas
* Add compatibility support for Python 3.8
* Improved Spoiler Logs for Multiworlds with differing Random settings between worlds
* Lab Dive now completable even with Eyedrops in your possession
* Great Fairy cutscene no longer plays on additional visits for health and magic refills.
* Running Man can now fill a Tycoon’s wallet when buying the Bunny Hood from the player


### 4.0

#### New Features

* Quick boot equips
  * Use D-pad left to equip Iron Boots if they're in the inventory, or D-pad right to equip Hover Boots if they're in the inventory.
  * Press the button again to equip Kokiri Boots.
* Quick Ocarina
  * Use D-pad down to pull out the Ocarina.
* Freestanding models
  * All freestanding item locations now display the model of the item you will receive.
* Ice traps now work from any location.
  * In freestanding locations, appears as a random major item
  * In shops, appears as a random major item with a misspelling
* Various speedups
  * No Talon cutscene when he runs away
  * Skip "Caught By Gerudo" cutscene
  * Shorten cutscene when getting Bullet Bag from the Deku Scrub in Lost Woods
  * Fast pushing and pulling
    * All types of blocks
    * Spinnable mirrors in Spirit Temple
    * Truth spinner in Shadow Temple
    * Puzzle in basement of Forest Temple
  * Ocarina minigame shortened to the first round
    * 5 notes instead of 8
  * Jabu-Jabu's Belly elevator spawns in a more convenient position
  * Kakariko carpenter position is offset so he is no longer in your way during the cucco route.
  * Warp songs now have a normal transition with no cutscene.
  * Pause screen starts on Inventory Screen instead of Map Screen
* Gold Skulltula textbox displays current number obtained
* Poe salesman tells point limit without needing to sell
* Patch files added so that generated seeds can be distributed legally

#### New Options

* Master Quest dungeon slider
  * Selects a number of Master Quest dungeons to appear in the game
    * Ex: 0 - All dungeons will have their default designs.
    * Ex: 6 - Half of all dungeons will have Master Quest redesigns.
    * Ex: 12 - All dungeons will have Master Quest redesigns.
* Damage multiplier
  * Changes the amount of damage taken
  * OHKO: Link dies in one hit
* Item pool
  * Replaces difficulty
  * Changes the number of bonus items that are available in the game
    * Plentiful: Extra major items are added
    * Balanced: The original item pool
    * Scarce: Some excess items are removed, including health upgrades
    * Minimal: Most excess items are removed
* Start with max rupees
  * The player begins the game with 99 rupees.
* Start with Deku equipment
  * The player begins the game with 10 Deku sticks and 20 Deku nuts.
  * If playing without shopsanity, the player begins with a Deku shield equipped.
* Start with fast travel
  * The player begins the game with the Prelude of Light and Serenade of Water songs learned and the Farore's Wind spell in the inventory.
  * These three items increase Link's mobility around the map, but don't actually unlock any new items or areas.
* Start with Tycoon wallet
* Open Zora's Fountain
  * King Zora is moved to the side from the start of the game.
  * Ruto's Letter is removed from the item pool and replaced with an Empty Bottle.
* Randomize starting time of day
* Ice traps setting
    * Off: All ice traps are removed
    * Normal: Only ice traps from base pool are placed
    * Extra ice traps: Chance to add extra ice traps when junk items are added to the item pool
    * Ice trap mayhem: All junk items added will be ice traps
    * Ice trap onslaught: All junk items will be ice traps, including ones in the base pool
* New Cosmetics
  * Added options for Sword Trail colors
    * Can set the length of the trails
    * Can set the inner and outer colors
    * Can set color to "Rainbow"
  * Additional SFX options

#### Updated Features

* Hints distribution
    * Changes how many useful hints are available in the game
    * Useless: Has nothing but junk hints
    * Balanced: Gives you a mix of useless and useful hints. Hints will not be repeated
    * Strong: Has some duplicate hints and no junk hints
    * Very Strong: Has only very useful hints
    * Tournament: Similar to strong but has no variation in hint types
* Frogs Ocarina Game added to always hints
* Hints are only in logic once you are able to reach the location of the gossip stone by logic.
    * Hints ignore logic if inaccessible
* Foolish choice hint added
  * Regions that are a foolish choice will not have any required items no matter what route is taken in logic.
* Big Poes location does not require a hint if the count set to 3 or less.
* Add medallion icons to the Temple of Time altar hint
* Add a hint for 0/6 trials if trial count is random
* Scrub Shuffle now updates the Business Scrubs' textboxes with the updated price for buying their item.

#### Updated Options

* Chest size matches contents updated
  * Boss keys appear in gold chests
  * Small keys appear in small gold chests
* Free Scarecrow's Song changes
  * Pulling out ocarina near a spot where Pierre can spawn will do so.
* Rainbow Bridge changes
  * Spiritual Stones added as bridge requirement
  * 100 Gold Skulltula tokens added as bridge requirement
* Any location can now be excluded from being required.
* Various advanced tricks has been split into individual tricks to be selected.
* Choose sound effects ocarina uses when played

#### Bug Fixes

* Deku and Hylian shields from chests no longer become Blue Rupees.
* Force game language to be English even if a Japanese rom is supplied
* Door of Time now opens when entering Temple of Time from all spawns.
* Fix empty bomb glitch
* Move item cost to after Player in the spoiler log
* Add Wasteland Bombchu Salesman to spoiler log when required for first Bombchus
* Fix message text table is too long error when using settings that add a lot of text to the ROM
* Kokiri Sword no longer required for fishing as child
* Fix Biggoron Sword collection delay
* Twinrova phase 2 textbox fix
* Switches in Forest and Fire Temple lowered by 1 unit to make it easier to press them
* Equipment menu will now show the name of the item you have in the first column
* Hover Boots will no longer show up as adult in the first equipment menu slot if a Fairy Slingshot was not gotten before becoming adult.
* Ammo items now use the correct item fanfare.
* Fix chest size matches contents to work for all chests
* Removed key for key logic
* Removed unused locked door in Water Temple
* Scarecrow's Song should no longer cause softlocks when played in laggy areas.
* Text error messages no longer display the Pocket Egg text.
* Ice traps added back to OHKO as the softlock appears fixed
* Ganon now says "my castle" instead of "Ganon's Castle" for light arrow hint.
* Fix various typos in text
  * Gerudo's Fortress
  * Zora's River
  * Red Rupee
  * Textbox about Dampé's grave
* "Ganon's Tower" is now just "Ganon's Castle".
* Dampé's Gravedigging Tour reward correctly flags collection on pickup.
* Castle Moat Bridge no longer closes when playing the Zelda Escape cutscene.
* Various logic fixes

#### Multiworld Changes

* Maximum player count increased from 31 to 255
* Ice traps can now be sent to other worlds.
* Ganon now tells you the location of your Light Arrows, not the location of some Light Arrows that may exist in your world.
* Item placement rebalanced so that an item for another player can only be placed on a location that both players can reach in logic

#### Development Version Changes

* Output patch file
  * Creates a binary patch file instead of a ROM which allows sharing a seed without sharing copyright protected material
* Patch ROM from file
  * Applies a generated patch file to the base ROM and creates a randomizer ROM which allows sharing a seed without sharing copyright protected material
* Settings presets
  * Adds a functionality to save settings strings for future use
  * Several presets are already provided.
* Create settings log if spoiler log is disabled
* File names no longer include a settings string.
  * Instead display a shortened SHA-1 hash of the settings string
* Add option for converting settings to a string and back
  * Only convert the specified settings to a settings string
  * If a settings string is specified output the used settings instead
* Python 3.5 is no longer supported.
  * You must have Python 3.6 or higher installed to run the randomizer code.
* Add option to only apply cosmetic patch without generating world data
* CLI uses a specified settings file instead of taking in each option.
  * Uses settings.sav as default if it exists
  * Uses default settings if no settings file is provided and no settings.sav exists
* Version check is no longer a dialog
  * Appears in a frame in the main randomizer window
* Copy settings string button
* The cosmetic versioning has been added to the ROM. Some cosmetics are only applied if they are safe to do so.
* Added ability to set the output file name and added file creation logs
* Major refactor of code

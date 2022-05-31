The **auto-tracker context** is a region of memory where a portion of the randomizer's configuration variables are stored in a versioned layout defined below. Its intent is to give some symbols added by the randomizer well-defined addresses so that auto-trackers can read them reliably without needing the symbols.json for the specific randomizer version that was used to generate the seed.

The starting address of the auto-tracker context is listed at address `0x8040_000c` (randomizer context + `0xc`). On versions of the randomizer before this feature was added, the starting address is given as zero. At that address, the following data can be found:

|Offset|Name|Min version|Size|Description|
|--:|---|--:|--:|---|
|`0x00`|`AUTO_TRACKER_VERSION`|1|`0x4`|Defines which entries in this table are available. Future versions may also change the layout of this table in an incompatible manner.|
|`0x04`|`CFG_DUNGEON_INFO_ENABLE`|1|`0x4`|`1` if pressing A on the inventory screen on the pause menu will display information about dungeons.|
|`0x08`|`CFG_DUNGEON_INFO_MQ_ENABLE`|1|`0x4`|`1` if the dungeon info in the pause menu should include info about which dungeons are in Master Quest mode.|
|`0x0c`|`CFG_DUNGEON_INFO_MQ_NEED_MAP`|1|`0x4`|`1` if the Master Quest info should only be displayed for dungeons whose maps have been obtained or which don't have maps.|
|`0x10`|`CFG_DUNGEON_INFO_REWARD_ENABLE`|1|`0x4`|`1` if the dungeon info in the pause menu should include info about which medallions and stones are in which dungeon.|
|`0x14`|`CFG_DUNGEON_INFO_REWARD_NEED_COMPASS`|1|`0x4`|`1` if the reward info should only be displayed for dungeons whose compasses have been obtained.|
|`0x18`|`CFG_DUNGEON_INFO_REWARD_NEED_ALTAR`|1|`0x4`|`1` if the reward info should only be displayed for rewards whose Temple of Time altar text boxes have been read.|
|`0x1c`|`CFG_DUNGEON_REWARDS`|1|`0xe`|A byte representing the medallion or stone for each dungeon. Dungeons without rewards are listed as `0xff`.|
|`0x2a`|`CFG_DUNGEON_IS_MQ`|1|`0xe`|A byte set to `1` for each dungeon in Master Quest mode.|
|`0x38`|`RAINBOW_BRIDGE_CONDITION`|2|`0x4`|The condition for spawning the rainbow bridge. `0` = open, `1` = medallions, `2` = dungeon rewards, `3` = stones, `4` = vanilla, `5` = tokens, `6` = hearts.|
|`0x3c`|`LACS_CONDITION`|2|`0x4`|The condition for triggering the light arrow cutscene. `0` = vanilla, `1` = medallions, `2` = dungeons, `3` = stones, `4` = tokens, `5` = hearts.|
|`0x40`|`RAINBOW_BRIDGE_COUNT`|2|`0x2`|The number of items (of the kind defined in `RAINBOW_BRIDGE_CONDITION`) required to spawn the rainbow bridge.|
|`0x42`|`LACS_CONDITION_COUNT`|2|`0x2`|The number of items (of the kind defined in `LACS_CONDITION`) required to trigger the light arrow cutscene.|
|`0x44`|`TRIFORCE_HUNT_ENABLED`|2|`0x2`|`1` if Triforce hunt is enabled.|
|`0x46`|`TRIFORCE_PIECES_REQUIRED`|2|`0x2`|In Triforce hunt, the total number of Triforce pieces (across all worlds) required to win the game.|
|`0x48`|`SPECIAL_DEAL_COUNTS`|2|`0x8`|A byte representing the number of special deal slots in each shop, in the following order: KF Shop, Market Bazaar, Market Potion Shop, Market Bombchu Shop, Kak Bazaar, Kak Potion Shop, GC Shop, ZD Shop.|

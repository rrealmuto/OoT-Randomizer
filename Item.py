from ItemList import item_table
from RulesCommon import allowed_globals, escape_name


class ItemInfo(object):
    items = {}
    events = {}
    bottles = set()
    medallions = set()
    stones = set()
    junk = {}

    solver_ids = {}
    bottle_ids = set()
    medallion_ids = set()
    stone_ids = set()

    def __init__(self, name='', event=False):
        if event:
            type = 'Event'
            progressive = True
            itemID = None
            special = None
        else:
            (type, progressive, itemID, special) = item_table[name]

        self.name = name
        self.advancement = (progressive is True)
        self.priority = (progressive is False)
        self.type = type
        self.special = special or {}
        self.index = itemID
        self.price = self.special.get('price')
        self.bottle = self.special.get('bottle', False)
        self.medallion = self.special.get('medallion', False)
        self.stone = self.special.get('stone', False)
        self.alias = self.special.get('alias', None)
        self.junk = self.special.get('junk', None)
        self.trade = self.special.get('trade', False)

        self.solver_id = None
        if name and self.junk is None:
            esc = escape_name(name)
            if esc not in ItemInfo.solver_ids:
                allowed_globals[esc] = ItemInfo.solver_ids[esc] = len(ItemInfo.solver_ids)
            self.solver_id = ItemInfo.solver_ids[esc]


for item_name in item_table:
    ItemInfo.items[item_name] = ItemInfo(item_name)
    if ItemInfo.items[item_name].bottle:
        ItemInfo.bottles.add(item_name)
        ItemInfo.bottle_ids.add(ItemInfo.solver_ids[escape_name(item_name)])
    if ItemInfo.items[item_name].medallion:
        ItemInfo.medallions.add(item_name)
        ItemInfo.medallion_ids.add(ItemInfo.solver_ids[escape_name(item_name)])
    if ItemInfo.items[item_name].stone:
        ItemInfo.stones.add(item_name)
        ItemInfo.stone_ids.add(ItemInfo.solver_ids[escape_name(item_name)])
    if ItemInfo.items[item_name].junk is not None:
        ItemInfo.junk[item_name] = ItemInfo.items[item_name].junk


class Item(object):

    def __init__(self, name='', world=None, event=False):
        self.name = name
        self.location = None
        self.event = event
        if event:
            if name not in ItemInfo.events:
                ItemInfo.events[name] = ItemInfo(name, event=True)
            self.info = ItemInfo.events[name]
        else:
            self.info = ItemInfo.items[name]
        self.price = self.info.special.get('price')
        self.world = world
        self.looks_like_item = None
        self.priority = self.info.priority
        self.type = self.info.type
        self.special = self.info.special
        self.alias = self.info.alias

        self.solver_id = self.info.solver_id
        # Do not alias to junk--it has no solver id!
        self.alias_id = ItemInfo.solver_ids[escape_name(self.alias[0])] if self.alias else None


    item_worlds_to_fix = {}

    def copy(self, new_world=None):
        if new_world is not None and self.world is not None and new_world.id != self.world.id:
            new_world = None

        new_item = Item(self.name, new_world, self.event)
        new_item.price = self.price

        if new_world is None and self.world is not None:
            Item.item_worlds_to_fix[new_item] = self.world.id

        return new_item


    @classmethod
    def fix_worlds_after_copy(cls, worlds):
        items_fixed = []
        for item, world_id in cls.item_worlds_to_fix.items():
            item.world = worlds[world_id]
            items_fixed.append(item)
        for item in items_fixed:
            del cls.item_worlds_to_fix[item]


    @property
    def index(self):
        idx = self.info.index
        if self.type == 'Shop':
            # Some shop items have the same item IDs as unrelated regular items. Make sure these don't get turned into nonsense.
            return idx
        # use different item IDs for items with conditional chest appearances so they appear according to the setting in the item's world, not the location's
        if idx == 0x005B and (self.world.settings.bridge == 'tokens' or self.world.settings.lacs_condition == 'tokens' or self.world.settings.shuffle_ganon_bosskey == 'tokens'):
            return 0x010C
        if idx in (0x003D, 0x003E, 0x0076) and (self.world.settings.bridge == 'hearts' or self.world.settings.lacs_condition == 'hearts' or self.world.settings.shuffle_ganon_bosskey == 'hearts'):
            return {0x003D: 0x010D, 0x003E: 0x010E, 0x0076: 0x010F}[idx]
        if idx in (0x0029, 0x002A) and 'shields' in self.world.settings.minor_items_as_major_chest:
            return {0x0029: 0x0110, 0x002A: 0x0111}[idx]
        if idx in (0x006A, 0x0003, 0x006B) and 'bombchus' in self.world.settings.minor_items_as_major_chest:
            return {0x006A: 0x0112, 0x0003: 0x0113, 0x006B: 0x0114}[idx]
        return idx


    @property
    def key(self):
        return self.smallkey or self.bosskey


    @property
    def smallkey(self):
        return self.type == 'SmallKey' or self.type == 'HideoutSmallKey'


    @property
    def bosskey(self):
        return self.type == 'BossKey' or self.type == 'GanonBossKey'


    @property
    def map(self):
        return self.type == 'Map'


    @property
    def compass(self):
        return self.type == 'Compass'


    @property
    def dungeonitem(self):
        return self.smallkey or self.bosskey or self.map or self.compass or self.type == 'SilverRupee'


    @property
    def unshuffled_dungeon_item(self):
        return ((self.type == 'SmallKey' and self.world.settings.shuffle_smallkeys in ('remove', 'vanilla', 'dungeon')) or
                (self.type == 'HideoutSmallKey' and self.world.settings.shuffle_hideoutkeys == 'vanilla') or
                (self.bosskey and self.world.settings.shuffle_bosskeys in ('remove', 'vanilla', 'dungeon')) or
                ((self.map or self.compass) and self.world.settings.shuffle_mapcompass in ('remove', 'startwith', 'vanilla', 'dungeon')) or
                (self.type == 'SilverRupee' and self.world.settings.shuffle_silver_rupees in ('remove', 'vanilla', 'dungeon')))


    @property
    def majoritem(self):
        if self.type == 'Token':
            return (self.world.settings.bridge == 'tokens' or self.world.settings.shuffle_ganon_bosskey == 'tokens' or
                (self.world.settings.shuffle_ganon_bosskey == 'on_lacs' and self.world.settings.lacs_condition == 'tokens'))

        if self.type in ('Drop', 'Event', 'Shop') or not self.advancement:
            return False

        if self.name.startswith('Bombchus') and not self.world.settings.free_bombchu_drops:
            return False

        if self.name == 'Heart Container' or self.name.startswith('Piece of Heart'):
            return (self.world.settings.bridge == 'hearts' or self.world.settings.shuffle_ganon_bosskey == 'hearts' or
                (self.world.settings.shuffle_ganon_bosskey == 'on_lacs' and self.world.settings.lacs_condition == 'hearts'))

        if self.map or self.compass:
            return False
        if self.type == 'DungeonReward' and self.world.settings.shuffle_dungeon_rewards in ('vanilla', 'reward', 'dungeon'):
            return False
        if self.type == 'SmallKey' and self.world.settings.shuffle_smallkeys in ('dungeon', 'vanilla'):
            return False
        if self.type == 'HideoutSmallKey' and self.world.settings.shuffle_hideoutkeys == 'vanilla':
            return False
        if self.type == 'BossKey' and self.world.settings.shuffle_bosskeys in ('dungeon', 'vanilla'):
            return False
        if self.type == 'GanonBossKey' and self.world.settings.shuffle_ganon_bosskey in ('dungeon', 'vanilla'):
            return False
        if self.type == 'SilverRupee' and self.world.settings.shuffle_silver_rupees in ('dungeon', 'vanilla'):
            return False

        return True


    @property
    def can_be_excluded(self):
        # these are items that can never be required
        return (
            self.name in self.world.item_added_hint_types['barren']
            or (
                self.name not in self.world.item_hint_type_overrides['barren']
                and (
                    (not self.majoritem)
                    or self.name in ('Double Defense', 'Ice Arrows')
                    or (
                        # Nayru's Love may be required to prevent forced damage
                        self.name == 'Nayrus Love'
                        and self.world.settings.damage_multiplier != 'ohko'
                        and self.world.settings.damage_multiplier != 'quadruple'
                        and self.world.settings.shuffle_scrubs == 'off'
                        and not self.world.settings.shuffle_grotto_entrances
                        and not self.world.full_one_ways
                    ) or (
                        # Stone of Agony skippable if not used for hints or grottos
                        self.name == 'Stone of Agony'
                        and self.world.settings.logic_grottos_without_agony
                        and self.world.settings.hints != 'agony'
                    ) or (
                        # Serenade and Prelude can only give access to new areas with certain forms of ER
                        self.name in ('Serenade of Water', 'Prelude of Light')
                        and not self.world.shuffle_special_interior_entrances
                        and not self.world.settings.shuffle_overworld_entrances
                        and self.world.settings.warp_songs == 'off'
                        and self.world.settings.shuffle_child_spawn != 'full'
                        and self.world.settings.shuffle_adult_spawn != 'full'
                    ) or (
                        # Both two-handed swords can be required in glitch logic, so only consider them foolish in glitchless
                        self.name in ('Biggoron Sword', 'Giants Knife')
                        and self.world.settings.logic_rules == 'glitchless'
                    ) or (
                        # Magic Beans are useless if beans are already planted
                        self.name in ('Magic Bean', 'Buy Magic Bean', 'Magic Bean Pack')
                        and self.world.settings.plant_beans
                    ) or (
                        # These silver rupees unlock a door to an area that's also reachable with lens
                        self.name in ('Silver Rupee (Bottom of the Well Basement)', 'Silver Rupee Pouch (Bottom of the Well Basement)')
                        and self.world.settings.logic_lens_botw
                    ) or (
                        # These silver rupees only unlock the map chest
                        self.name in ('Silver Rupee (Shadow Temple Scythe Shortcut)', 'Silver Rupee Pouch (Shadow Temple Scythe Shortcut)')
                        and self.world.dungeon_mq['Shadow Temple']
                        and self.world.settings.shuffle_mapcompass == 'vanilla'
                    ) or (
                        # With this trickset, these silver rupees are logically irrelevant
                        self.name in ('Silver Rupee (Spirit Temple Sun Block)', 'Silver Rupee Pouch (Spirit Temple Sun Block)')
                        and self.world.settings.logic_spirit_sun_chest_no_rupees
                        and not self.world.settings.logic_spirit_sun_chest_bow
                    ) or (
                        # These silver rupees only lock pots and the trial
                        self.name in (
                            'Silver Rupee (Ganons Castle Fire Trial)', 'Silver Rupee Pouch (Ganons Castle Fire Trial)',
                            'Silver Rupee (Ganons Castle Shadow Trial)', 'Silver Rupee Pouch (Ganons Castle Shadow Trial)',
                            'Silver Rupee (Ganons Castle Water Trial)', 'Silver Rupee Pouch (Ganons Castle Water Trial)',
                            'Silver Rupee (Ganons Castle Forest Trial)', 'Silver Rupee Pouch (Ganons Castle Forest Trial)',
                        )
                        and self.world.settings.shuffle_pots in ('off', 'overworld')
                        and self.world.settings.trials == 0
                    )
                )
            )
        )


    @property
    def goalitem(self):
        return self.name in self.world.goal_items


    @property
    def triforce_piece(self):
        from ItemPool import triforce_pieces

        return self.name in triforce_pieces


    @property
    def advancement(self):
        if self.name == 'Gold Skulltula Token' and self.world is not None and self.world.max_tokens == 0:
            return False
        return self.info.advancement


    def __str__(self):
        return str(self.__unicode__())


    def __unicode__(self):
        return '%s' % self.name


def ItemFactory(items, world=None, event=False):
    if isinstance(items, str):
        if not event and items not in ItemInfo.items:
            raise KeyError('Unknown Item: %s' % items)
        return Item(items, world, event)

    ret = []
    for item in items:
        if not event and item not in ItemInfo.items:
            raise KeyError('Unknown Item: %s' % item)
        ret.append(Item(item, world, event))

    return ret


def MakeEventItem(name, location, item=None):
    if item is None:
        item = Item(name, location.world, event=True)
    location.world.push_item(location, item)
    location.locked = True
    if name not in item_table:
        location.internal = True
    location.world.event_items.add(name)
    return item


def IsItem(name):
    return name in item_table


def ItemIterator(predicate=lambda loc: True, world=None):
    for item_name in item_table:
        item = ItemFactory(item_name, world)
        if predicate(item):
            yield item

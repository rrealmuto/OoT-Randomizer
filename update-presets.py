import sys

import json

import SettingsList

def complete_presets(new_presets, interactive):
    with open('data/presets_default.json', encoding='utf-8') as f:
        p = json.load(f)
    for preset_name, old_preset in p.items():
        if preset_name not in new_presets:
            new_presets[preset_name] = {}
        for setting_name, setting in SettingsList.si_dict.items():
            if setting.shared and setting_name not in new_presets[preset_name]:
                if setting_name in old_preset:
                    new_presets[preset_name][setting_name] = old_preset[setting_name]
                else:
                    if interactive:
                        new_presets[preset_name][setting_name] = json.loads(input(f'{preset_name}[{setting_name}]: ').strip())
                    else:
                        raise ValueError(f'Missing setting {setting_name!r} in preset {preset_name!r}')
        SettingsList.validate_settings(new_presets[preset_name], check_conflicts=False)

if __name__ == '__main__':
    new_presets = {}
    if '--hook' in sys.argv[1:]:
        complete_presets(new_presets, False)
        with open('data/presets_default.json', encoding='utf-8') as f:
            if f.read() != json.dumps(new_presets, indent=2) + '\n':
                raise ValueError('presets not formatted correctly, run .\\update-presets.py to fix')
    else:
        complete_presets(new_presets, True)
        with open('data/presets_default.json', 'w', encoding='utf-8') as f:
            json.dump(new_presets, f, indent=2)
            print(file=f)

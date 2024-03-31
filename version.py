__version__ = '8.1.13'

# This is a supplemental version number for branches based off of main dev.
supplementary_version = 93

# Pick a unique identifier byte for your fork if you are intending to have a long-lasting branch.
# This will be 0x00 for main releases and 0x01 for main dev.
branch_identifier = 0x45

# URL to your branch on GitHub.
branch_url = 'https://github.com/rrealmuto/OoT-Randomizer/tree/Dev-Rob'

# This is named __version__ at the top for compatability with older versions trying to version check.
base_version = __version__

# And finally, the completed version string. This is what is displayed and used for salting seeds.
__version__ = f'{base_version} Rob-{supplementary_version}'

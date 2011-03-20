# Configuration settings for controller

# Client settings
CLIENT_RUN_COMMAND = "RobotClient/build/release/robot.bin"
# the controller will cd to antix Path + this  directory and call "make":
CLIENT_BUILD_DIR = "RobotClient/"
CLIENT_BUILD_COMMAND = "make robot"

# Server settings
GRID_RUN_COMMAND = "GridServer/build/release/grid.bin"
GRID_PORT = "13337" # for simplicity, every subsequent grid will use port +1
# the controller will cd to antix Path + this  directory and call "make":
GRID_BUILD_DIR = "GridServer/"
GRID_BUILD_COMMAND = "make grid"

# Clock settings
CLOCK_RUN_COMMAND = "ClockServer/build/release/clock.bin"
CLOCK_PORT = "13437" # for simplicity, every subseqent clock will use port +1
# the controller will cd to antix Path + this  directory and call "make":
CLOCK_BUILD_DIR = "ClockServer/"
CLOCK_BUILD_COMMAND = "make clock"

# Drawer settings
DRAWER_RUN_COMMAND = "hostname"
# the controller will cd to antix Path + this  directory and call "make":
DRAWER_BUILD_DIR = ""
DRAWER_BUILD_COMMAND = ""

# Computers
CSIL_COMPUTERS = (
    'apple',
    'apricot',
    'avocado',
    'banana',
    'blackberry',
    'blueberry',
    'breadfruit',
    'cantelope',
    'cherry',
    'citron',
    'coconut',
    'cranberry',
    'currant',
    'durian',
    'fig',
    'granadilla',
    'grape',
    'guava',
    'honeydew',
    'jujube',
    'kiwi',
    'kumquat',
    'mandarin',
    'mango',
    'maypop',
    'medlar',
    'melon',
    'mulberry',
    'nectarine',
    'orange',
    'papaya',
    'peach',
    'pear',
    'pineapple',
    'plum',
    'pomelo',
    'rambutan',
    'salak',
    'tamarind',
    'tangerine',
    'amber',
    'aqua',
    'auburn',
    'azure',
    'beige',
    'black',
    'blue',
    'brown',
    'buff',
    'carmine',
    'cerise',
    'cerulean',
    'chartreuse',
    'cinnabar',
    'crimson',
    'ecru',
    'emerald',
    'gray',
    'green',
    'khaki',
    'magenta',
    'maize',
    'mauve',
    'ochre',
    'pink',
    'puce',
    'red',
    'russet',
    'scarlet',
    'sienna',
    'tan',
    'taupe',
    'teal',
    'turquoise',
    'vermillion',
    'violet',
    'white',
    'yellow',
)

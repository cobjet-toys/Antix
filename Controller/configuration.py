# Configuration settings for controller

START_LOCALLY = False # set to True to start all processes on localhost
                      # set to False to start all processes across CSIL machines
TYPE = "release" # can be replaced with debug
NUM_GRIDS = 3
NUM_ROBOT_CLIENTS = 3

# Server settings
GRID_RUN_COMMAND = "GridServer/build/" + TYPE + "/grid.bin"
GRID_PORT = "13337" # for simplicity, every subsequent grid will use port +1

# the controller will cd to antix Path + this  directory and call "make":
GRID_BUILD_DIR = "GridServer/"
GRID_BUILD_COMMAND = "make grid"

# Clock settings
CLOCK_RUN_COMMAND = "ClockServer/build/" + TYPE + "/clock.bin"
CLOCK_PORT = "13437" # for simplicity, every subseqent clock will use port +1

# the controller will cd to antix Path + this  directory and call "make":
CLOCK_BUILD_DIR = "ClockServer/"
CLOCK_BUILD_COMMAND = "make clock"

# Controller Client settings
CTRL_CLIENT_RUN_COMMAND = "ControllerClient/build/" + TYPE + "/controller.bin"

# the controller will cd to antix Path + this directory and call "make":
CTRL_CLIENT_BUILD_DIR = "ControllerClient/"
CTRL_CLIENT_BUILD_COMMAND = "make controller"

# Client settings
ROBOT_CLIENT_RUN_COMMAND = "RobotClient/build/" + TYPE + "/robot.bin"

# the controller will cd to antix Path + this  directory and call "make":
ROBOT_CLIENT_BUILD_DIR = "RobotClient/"
ROBOT_CLIENT_BUILD_COMMAND = "make robot"

# Drawer settings
DRAWER_RUN_COMMAND = "Drawer/build/" + TYPE + "/drawer.bin"
DRAWER_PORT = "13537"

# the controller will cd to antix Path + this  directory and call "make":
DRAWER_BUILD_DIR = "Drawer/"
DRAWER_BUILD_COMMAND = "make drawer"

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

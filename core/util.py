import os

def get_addon_path():
    return os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))

def get_addon_name():
    return __package__[:-len('.bxx.core')]
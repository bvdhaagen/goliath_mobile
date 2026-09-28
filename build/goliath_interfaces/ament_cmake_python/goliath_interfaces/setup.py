from setuptools import find_packages
from setuptools import setup

setup(
    name='goliath_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('goliath_interfaces', 'goliath_interfaces.*')),
)

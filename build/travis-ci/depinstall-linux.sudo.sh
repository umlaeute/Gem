#!/bin/sh

apt-get update -qq
apt-get build-dep gem -qq
apt-get install libglfw-dev libglfw3-dev libsdl2-dev -qq
apt-get install libvncserver-dev -qq

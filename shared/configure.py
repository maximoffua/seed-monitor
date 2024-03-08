#!/usr/bin/env python3
from os import getenv
import requests
import sys
from os.path import basename

Import('env')
Import("projenv")


def publish_firmware(source, target, env):
    print(f"src={source} | target={str(target)} | env=$PIOENV")
    env.Execute("upload $PIOENV")


# Custom upload command and program name
env.Replace(PROGNAME="firmware", UPLOADCMD=publish_firmware)
env.AddCustomTarget("remote", None, 'upload')

# Environment variable name and fallback value
fromEnv = [
    ("AP_SSID", "AgroMaranta-pilot"),
    ("AP_PASSWORD", "Vmaximoff"),
    ("WIFI_PASSWORD", None),
    ("WIFI_SSID", None),
    ("MQTT_ADDRESS", "10.10.100.22:1883"),
]

wifi_defs = [(var, env.StringifyMacro(val)) for var, val in map(lambda x: (x[0], getenv(*x)), fromEnv) if val]
# wifi_settings = [f"-D{var}={env.StringifyMacro(val)}" for var, val in map(lambda x: (x[0], getenv(*x)), fromEnv) if val]
# print(wifi_settings)

projenv.Append(CPPDEFINES=wifi_defs)
# HxD plugin for useful conversions

A screenshot for my favorite hex editor, [HxD](https://mh-nexus.de/en/hxd/).

![screenshot.png](screenshot.png)

## Features

 - Copy as string literal `"\x12\x34\x56\x78"`
 - Copy as Python array `[0x12, 0x34, 0x56, 0x78]`
 - To/from Base64

Based off the [example](https://github.com/maelh/hxd-plugin-framework) by the HxD author

# Installation

Download from releases and add to `C:\Program Files\HxD\Plugins`. Create the folder if it doesn't already exist

## Requirements

This is only supported on HxD 2.4.0.0. This is because it uses non-exported, reverse-engineered pointers and structures for performance reasons. It will still work on other versions, but it will limit the selection size to 0x1000.

# Compiling

Compile the solution in `Examples\C++` with VS2019

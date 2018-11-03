# Mp3Edit
A metadata tag editor for audio files. Mainly used to unify the existence of multiple tag formats in an audio file and to assist in searching for metadata.

## Features
Currently supports MP3, FLAC and Ogg Vorbis files. Unifies tags in audio files and supports the reading of some audio properties in each format as listed below:

| Audio Format | Unified Tag     | Audio Properties                     |
| ------------ | --------------- | ------------------------------------ |
| MP3          | ID3v1 + ID3v2.3 | Bitrate, Sampling Rate, Channel Mode |
| FLAC         | Vorbis Comments | - |
| Ogg Vorbis   | Vorbis Comments | Bitrate, Sampling Rate, Channel Mode |

Supported tags that can be either read or removed:
1. APE tags
2. ID3 tags (both v1 and v2)
3. Lyrics3 tags
4. Vorbis Comments (both for FLAC and Ogg formats).

Essential audio metadata that are kept:
1. Title
2. Artist
3. Album
4. Track Number
5. Track Total

## Build Dependencies
A recent GCC with C++17 support

gtkmm 3.0+

CMake 3.0+

pkg-config

## Installation
Ensure all prerequisites have been installed. In the main directory, run:

    mkdir build
    cd build
    cmake -DRELEASE_BUILD=1 ..
    make

The compiled binary will be placed in `bin/mp3edit`.

The flag `-DRELEASE_BUILD=1` can be changed to `-DRELEASE_BUILD=0` if it is intended for the binary file to use a GUI from an external Glade file (`bin/mp3edit.glade`). Otherwise, the GUI is embedded into the binary.

## Usage
Run:

    bin/mp3edit

## License
Mp3Edit is licensed under the MIT License.

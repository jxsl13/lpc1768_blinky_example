Import("env", "projenv")

import sys
import struct
import intelhex
import time


# from https://github.com/basilfx/lpc_checksum/blob/master/lpc_checksum.py by Bas Stottelaar
BLOCK_START = 0
BLOCK_COUNT = 7
BLOCK_SIZE = 4
BLOCK_TOTAL = (BLOCK_COUNT * BLOCK_SIZE)

def checksum(filename, format="bin", read_only=False):
    """
    Calculate the checksum of a given binary image. The checksum is written
    back to the file and is returned. When read_only is set to True, the file
    will not be changed.
    filename  -- firmware file to checksum
    format    -- input file format (bin or hex, default bin)
    read_only -- whether to write checksum back to the file (default False)
    """

    # Open the firmware file.
    handle = intelhex.IntelHex()
    handle.loadfile(filename, format=format)

    # Read the data blocks used for checksum calculation.
    block = bytearray(handle.gets(BLOCK_START, BLOCK_TOTAL))

    if len(block) != BLOCK_TOTAL:
        raise Exception("Could not read the required number of bytes.")

    # Compute the checksum value.
    result = 0

    for i in range(BLOCK_COUNT):
        value, = struct.unpack_from("I", block, i * BLOCK_SIZE)
        result = (result + value) & 0xFFFFFFFF

    result = ((~result) + 1) & 0xFFFFFFFF

    # Write checksum back to the file.
    if not read_only:
        handle.puts(BLOCK_START + BLOCK_TOTAL, struct.pack("I", result))
        handle.tofile(filename, format=format)

    # Done
    return result

def post_bin_file(source, target, env):
    print("==== Post building BIN file ====")
    target_firmware_bin = str(target[0])
    #print(source, target, env)
    print("Firmware path: %s" % target_firmware_bin)
    ret_check = checksum(target_firmware_bin)
    print("Wrote checksum 0x%x into binary." % ret_check)
    

# fix checksum
env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", post_bin_file)
# fix openocd target 
platform = env.PioPlatform()
env.Prepend(
    UPLOADERFLAGS=["-s", platform.get_package_dir("tool-openocd") or ""]
)
env.Append(
    UPLOADERFLAGS=["-c", "program {$SOURCE} verify reset; shutdown"]
)
env.Replace(
    UPLOADER="openocd",
    UPLOADCMD="$UPLOADER $UPLOADERFLAGS"
)
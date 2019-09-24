Import("env", "projenv")

from struct import unpack, pack


# from https://github.com/basilfx/lpc_checksum/blob/master/lpc_checksum.py by Bas Stottelaar
# removed intelhex dependency like https://github.com/ARMmbed/mbed-os/blob/master/tools/targets/LPC.py
BLOCK_START = 0
BLOCK_COUNT = 7
BLOCK_SIZE = 4
BLOCK_TOTAL = (BLOCK_COUNT * BLOCK_SIZE)

def patch(filename):
    """
    Calculate the checksum of a given binary image. The checksum is written
    back to the file and is returned. When read_only is set to True, the file
    will not be changed.
    filename  -- firmware file to checksum
    format    -- input file format (bin or hex, default bin)
    read_only -- whether to write checksum back to the file (default False)
    """

    with open(filename, 'r+b') as bin:
        # Read entries 0 through 6 (Little Endian 32bits words)
        vector = [unpack('<I', bin.read(BLOCK_SIZE))[BLOCK_START] for _ in range(BLOCK_COUNT)]

        # location 7 (offset 0x1C in the vector table) should contain the 2's
        # complement of the check-sum of table entries 0 through 6
        bin.seek(0x1C)
        result = (~sum(vector) + 1) & 0xFFFFFFFF
        bin.write(pack('<I', result))
        return result

def is_patched(bin_path):
    with open(bin_path, 'rb') as bin:
        # The checksum of the first 8 table entries should be 0
        return (sum([unpack('<I', bin.read(BLOCK_SIZE))[BLOCK_START] for _ in range(BLOCK_COUNT +1)]) & 0xFFFFFFFF) == 0

def post_bin_file(source, target, env):
    print("==== Post building BIN file ====")
    target_firmware_bin = str(target[0])
    #print(source, target, env)
    print("Firmware path: %s" % target_firmware_bin)
    checksum = patch(target_firmware_bin)
    print("Wrote checksum 0x%x into binary." % checksum)
    assert is_patched(target_firmware_bin), "The file is not patched correctly!"
    

# fix checksum
env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", post_bin_file)


import argparse
from time import sleep

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("file", type=str)
    parser.add_argument("-w", "--width", help = "Byte width to swap (default=4)", default=4, type=int)
    parser.add_argument("-o", "--output", help = "Output file", required=True, type=str)
    args = parser.parse_args()
    
    if not args.file:
        parser.print_help()

    infile = open(args.file, 'rb')
    outfile = open(args.output, 'wb')
    
    width = args.width
    while True:
        data = infile.read(width)
        if(len(data) == 0):
            break
        if(len(data) < width):
            # Pad with 0s
            data += bytearray(width-len(data))

        val = int.from_bytes(data, byteorder='big')
        swapped = val.to_bytes(4, 'little')
        outfile.write(swapped)
    infile.close()
    outfile.close()

if __name__ == "__main__":
    main()
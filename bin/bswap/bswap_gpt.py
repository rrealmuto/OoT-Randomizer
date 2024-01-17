import sys
import threading
from queue import Queue

def swap_byte_order(data, word_size):
    swapped_data = bytearray(data)

    for i in range(0, len(data), word_size):
        start = i
        end = i + word_size
        if end <= len(data):
            swapped_data[start:end] = reversed(swapped_data[start:end])

    return swapped_data

def worker(queue, output_file, word_size):
    while True:
        chunk = queue.get()
        if chunk is None:
            break
        swapped_chunk = swap_byte_order(chunk, word_size)
        output_file.write(swapped_chunk)

def main():
    if len(sys.argv) != 4:
        print("Usage: python swap_byte_order.py input_file word_size output_file")
        return

    input_file = sys.argv[1]
    word_size = int(sys.argv[2])
    output_file_name = sys.argv[3]

    try:
        input_file = open(input_file, "rb")
    except FileNotFoundError:
        print(f"Input file '{input_file}' not found.")
        return

    output_file = open(output_file_name, "wb")

    num_threads = 4  # Adjust the number of threads as needed
    queue = Queue()

    # Start worker threads
    threads = []
    for _ in range(num_threads):
        thread = threading.Thread(target=worker, args=(queue, output_file, word_size))
        thread.start()
        threads.append(thread)

    while True:
        chunk = input_file.read(16)  # Read in 1MB chunks (adjust as needed)
        if not chunk:
            break
        queue.put(chunk)

    # Signal worker threads to exit
    for _ in range(num_threads):
        queue.put(None)

    # Wait for all worker threads to finish
    for thread in threads:
        thread.join()

    input_file.close()
    output_file.close()

    print(f"Byte order swapped and saved to '{output_file_name}'.")

if __name__ == "__main__":
    main()

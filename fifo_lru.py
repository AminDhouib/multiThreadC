#########################################
## Nom: Yesmine Zribi & Amin Dhouib
## Numero d'etudiant: 8402454 & 300025259
#########################################

from queue import Queue
import random

# String that represents a free frame
free_frame_str = ""

# Perform FIFO algorithm
def performFifo(page_reference_list, frames):

    # Queue that will keep track of the first page that was referenced
    fifoQueue = Queue()
    pointer_next_cell = 0

    number_pagefaults = 0

    for reference in page_reference_list:

        # Case 1: Empty frames are available
        if frames[pointer_next_cell] == free_frame_str:

            # Case 1.1: Page reference already in memory
            if reference in frames:
                pass #do nothing

            # Case 1.2: Page replacement required (replace free space)
            else:
                frames[pointer_next_cell] = reference                           # Put new page reference on the cell
                number_pagefaults += 1                                          # Increment page fault
                fifoQueue.put(pointer_next_cell)                                # Also put that pointer on the queue
                pointer_next_cell = (pointer_next_cell + 1) % len(frames)       # Move pointer to the next cell

        # Case 2: Empty frames are not available
        else:
            # Case 2.1: Page reference already in memory
            if reference in frames:
                pass #do nothing

            # Case 2.2: Page replacement required
            else: #Page-replacement required
                oldest_page_index = fifoQueue.get()                             # Get the page that stayed the longest in the memory
                number_pagefaults += 1                                          # Increment page faults
                frames[oldest_page_index] = reference                           # Then replace the 'oldest page' with the new page
                fifoQueue.put(oldest_page_index)                                # Also put that pointer to the queue
                pointer_next_cell = (oldest_page_index + 1) % len(frames)       # Move pointer to the next cell

        print(frames)

    return number_pagefaults


# Perform LRU algorithm
def performLru(page_reference_list, frames):

    # List that will keep track of the usage of the pages. If the page is at the start of the list then it is the least frequentially used (vice-versa).
    reference_frequency_list = []
    pointer_next_cell = 0
    number_pagefaults = 0

    for reference in page_reference_list:

        # Case 1: Page already in memory
        if frames[pointer_next_cell] == free_frame_str:

            # Case 1.1: Page reference already in memory
            if reference in frames:
                reference_frequency_list.remove(reference)                      # Remove and append to put the page at the end of the list
                reference_frequency_list.append(reference)                      # as it is now the last page to be called

            # Case 1.2: Page replacement required (replace free space)
            else:
                frames[pointer_next_cell] = reference                           # Put new page reference on the cell
                number_pagefaults += 1                                          # Increment page faults
                reference_frequency_list.append(reference)                      # Add that reference to the end of the list
                pointer_next_cell = (pointer_next_cell + 1) % len(frames)       # Move pointer to the next cell

        # Case 2: Empty frames are not available
        else:
            # Case 2.1: Page reference already in memory
            if reference in frames:
                reference_frequency_list.remove(reference)                      # Remove and append to put the page at the end of the list
                reference_frequency_list.append(reference)                      # as it is now the last page to be called

            # Case 2.2: Page replacement required
            else:
                least_recently_used_index = reference_frequency_list[0]         # Take the least used page
                reference_frequency_list.remove(least_recently_used_index)      # Remove the least recently used page from the list
                memory_replace_index = frames.index(least_recently_used_index)  # Get the index of the least used page in the memory
                frames[memory_replace_index] = reference                        # Then replace the least used page with the new page
                number_pagefaults += 1                                          # Increment page faults
                reference_frequency_list.append(reference)                      # Add the new page at the end of the list
                pointer_next_cell = (memory_replace_index + 1) % len(frames)    # Point to the next cell

        print(frames)

    return number_pagefaults

# Main Code
def main():
    print("")

    # Generate a random page-reference string where page numbers range from 0 to 9.
    page_reference_length = 12
    page_reference_list = []

    print("Generated Page-Reference String:")
    for i in range(page_reference_length):
        page_reference_list.append(random.randint(0, 9))
    print(page_reference_list, end= '\n\n')

    # Generate random length of frames. Number of page frames can vary from 1 to 7.
    frame_length = random.randint(1, 7)
    frames_fifo = []
    for i in range(frame_length):
        frames_fifo.append(free_frame_str)

    # Make copy of frames for LRU
    frames_url = frames_fifo.copy()

    print("Performing FIFO Paging Algorithm:")
    fifo_page_faults = performFifo(page_reference_list,frames_fifo)
    print("Number of Page Faults: " + str(fifo_page_faults))

    print("\n")

    print("Performing LRU Paging Algorithm:")
    lru_page_faults = performLru(page_reference_list, frames_url)
    print("Number of Page Faults: " + str(lru_page_faults))


## CODE STARTS HERE ##
main()